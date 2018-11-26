#include "ar_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <input/touch_input_manager.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/render_target.h>

#include <sony_sample_framework.h>
#include <sony_tracking.h>


ARApp::ARApp(gef::Platform& platform) :
	Application(platform),
	input_manager_(NULL),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL),
	primitive_builder_(NULL),
	active_touch_id(-1)
{
}

void ARApp::Init()
{
	input_manager_ = gef::InputManager::Create(platform_);
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	primitive_builder_ = new PrimitiveBuilder(platform_);


	/// 2D Camera Feed
		// Set up the Ortho Matrix for rendering the camera feed.
		ortho_matrix_.SetIdentity();	// Probably unneccesary.
		ortho_matrix_ = platform_.OrthographicFrustum(-1, 1, -1, 1, -1, 1);	// Numbers taken from tutorial sheet.
	
		// Calculate y-scaling factor (based on resolution of camera and resolution of screen.)
		scaling_factor_ = ((960.0f / 544.0f) / (640.0f / 480.0f));
	
		// Scale the camera feed to fit on the Vita's display.
		camera_feed_sprite_.set_width(2.0f);
		camera_feed_sprite_.set_height(2.0f * scaling_factor_);

		// Place the sprite at the back of the orthographic fustrum to prevent clipping.
		camera_feed_sprite_.set_position(0, 0, 1);

		// Texture the sprite with the camera feed.
		camera_feed_sprite_.set_texture(&camera_feed_texture_);

	/// 3D Elements
		// Create the initial projection matrix.
		unscaled_projection_matrix_.SetIdentity();
		unscaled_projection_matrix_ = platform_.PerspectiveProjectionFov(SCE_SMART_IMAGE_FOV, (SCE_SMART_IMAGE_WIDTH / SCE_SMART_IMAGE_HEIGHT), 0.1f, 100.0f);

		// Create the Scaling Matrix
		scaling_matrix_.SetIdentity();
		scaling_matrix_.set_m(1, 1, scaling_factor_);


		// Create the final scaled matrix. 
		scaled_projection_matrix_.SetIdentity();
		scaled_projection_matrix_ = unscaled_projection_matrix_ * scaling_matrix_;

		// Create an identity matrix.
		identity_.SetIdentity();

		// Create default box mesh for testing.
		box_scale_matrix.Scale(gef::Vector4(0.00125f, 0.00125f, 0.00125f));
		
		// Initialise the game objects
		test_ = new GameObject(platform_, "balls/ball1.scn");				// Need to do additional setup for rigged models. See animated_mesh for details.


		// Create a Debug Sphere
		debug_sphere.set_mesh(primitive_builder_->GetDefaultCubeMesh());
		
		
	//

	InitFont();
	SetupLights();


	// initialise sony framework
	sampleInitialize();
	smartInitialize();

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);

	// Initialise touch input
	if (input_manager_ && input_manager_->touch_manager() && (input_manager_->touch_manager()->max_num_panels() > 0))
	{
		input_manager_->touch_manager()->EnablePanel(0);
	}

}

void ARApp::CleanUp()
{
	delete primitive_builder_;
	primitive_builder_ = NULL;

	smartRelease();
	sampleRelease();

	CleanUpFont();
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;
}

bool ARApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	AppData* dat = sampleUpdateBegin();

	// use the tracking library to try and find markers
	smartUpdate(dat->currentImage);

	// check to see if a particular marker can be found
	if (sampleIsMarkerFound(marker_id))
	{
		// marker is being tracked, get it�s transform
		gef::Matrix44 marker_transform;
		sampleGetTransform(
			marker_id,
			&marker_transform);

		// set the transform of the 3D mesh instance to draw on
		// top of the marker
		test_->SetTransform(box_scale_matrix * marker_transform);
			
		
	}

	// Check for user input, process accordingly.
	if (input_manager_)
	{
		input_manager_->Update();

		// If there is currently touch input to process
		if (ProcessTouchInput())
		{
			// Create a ray from the touch postion into the scene
			GetRay(ray_start, ray_direction, scaled_projection_matrix_, identity_);

			// Test to see if the ray collided with the sphere.
			if (RayToSphere(*(test_), ray_start, ray_direction))
			{
				// Ray collision detection was successful!
				int foo = 0;
				
			}
		}
	}

	// DEBUG
	debug_matrix.SetIdentity();

	debug_matrix.Scale(gef::Vector4(0.1f, 0.1f, 0.1f));

	debug_matrix.SetTranslation(test_->GetTranslation() + test_->CollisionSpherePosition());

	debug_sphere.set_transform(debug_matrix);

	///

	sampleUpdateEnd(dat);

	// Current_state.Run

	return true;
}

void ARApp::Render()
{
	AppData* dat = sampleRenderBegin();

	//
	// Render the camera feed
	//

	// REMEMBER AND SET THE PROJECTION MATRIX HERE
		sprite_renderer_->set_projection_matrix(ortho_matrix_);
		

	sprite_renderer_->Begin(true);

	// DRAW CAMERA FEED SPRITE HERE
		// check there is data present for the camera image before trying to draw it
		if (dat->currentImage)
		{
			camera_feed_texture_.set_texture(dat->currentImage->tex_yuv);
			sprite_renderer_->DrawSprite(camera_feed_sprite_);
		}

	sprite_renderer_->End();

	//
	// Render 3D scene
	//

	// SET VIEW AND PROJECTION MATRIX HERE
	renderer_3d_->set_projection_matrix(scaled_projection_matrix_);
	renderer_3d_->set_view_matrix(identity_);

	// Begin rendering 3D meshes, don't clear the frame buffer
	renderer_3d_->Begin(false);

	// DRAW 3D MESHES HERE
	renderer_3d_->DrawMesh(*(gef::MeshInstance*)test_);	// NEED TO REPLACE THIS WITH Draw Skinned Mesh or something. See animated_mesh for details.

	renderer_3d_->DrawMesh(debug_sphere);

	renderer_3d_->End();

	RenderOverlay();

	sampleRenderEnd();

	// Current State.Render
}


void ARApp::RenderOverlay()
{
	//
	// render 2d hud on top
	//
	gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform_.OrthographicFrustum(0.0f, platform_.width(), 0.0f, platform_.height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}


void ARApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void ARApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}


void ARApp::DrawHUD()
{
	if(font_)
	{
		// Degub - Display the position of the touch
		if (active_touch_id != -1)
		{
			font_->RenderText(
				sprite_renderer_,
				gef::Vector4(touch_position.x, touch_position.y, -0.9f),
				1.0f, 0xffffffff, gef::TJ_LEFT, "(%.1f, %.1f)",
				touch_position.x, touch_position.y);
		}

		// Display framerate text
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}


void ARApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}


bool ARApp::ProcessTouchInput()
{
	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();

	bool isTouch = false;

	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		// get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);

		// go through the touches
		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); touch++)
		{
			//if the active touch id is -1, then we are not currently processing a touch.
			if (active_touch_id == -1)
			{
				// check for the start of a new touch
				if (touch->type == gef::TT_NEW)
				{
					// Set the current active touch to be this new touch.
					active_touch_id = touch->id;

					// do any processing for  new touch here
					// we're just going to record the position of the touch
					touch_position = touch->position;

					isTouch = true;

				}
			}
			else if (active_touch_id == touch->id)
			{
				// we are processing touch data with a matching id to the one we are looking for
				if (touch->type == gef::TT_ACTIVE)
				{
					// update an active touch here
					// we're just going to record the position of the touch
					touch_position = touch->position;

					isTouch = true;
				}
				else if (touch->type == gef::TT_RELEASED)
				{
					// the touch we are tracking has been released
					// perform any actions that need to happen when a touch is released here
					// we're not doing anything her apart from restarting the active touch id
					active_touch_id = -1;

					isTouch = false;
				}
			}		
		}
	}

	return isTouch;
}


// Here's the code that Grant emiled to me. I've changed a couple of variable names for consistency within my own code.
void ARApp::GetRay(gef::Vector4 & start_point, gef::Vector4 & direction, const gef::Matrix44 & projection, const gef::Matrix44 & view)
{
	// Make sure the input manager and touch manager have been initialised.
	if(input_manager_ && input_manager_->touch_manager())
	{
		gef::Vector2 mouse_position = input_manager_->touch_manager()->mouse_position();	// TO DO - Swap out with touch position.
				

		gef::Vector2 normalised_device_coordinates;

		float half_width = platform_.width() * 0.5f;
		float half_height = platform_.height() * 0.5f;

		// Calculate Normalised Device Cordinates (https://stackoverflow.com/questions/46749675/opengl-mouse-coordinates-to-space-coordinates/46752492)
		normalised_device_coordinates.x = (static_cast<float>(touch_position.x) - half_width) / half_width;
		normalised_device_coordinates.y = (half_height - static_cast<float>(touch_position.y)) / half_height;

		// Since we're working from the uniform world cordinates back to the trapezoid veiw frustrum, we need an inverse projection matrix.
		gef::Matrix44 projectionInverse;
		projectionInverse.Inverse(view * projection);

		// Define the start and end point of the ray (based on the frustrum of the camera.)
		gef::Vector4 near_point, far_point;

		// direct 3D
		// In Direct3D, the frustrum runs from 0 to 1.
		// near_point = gef::Vector4(normalised_device_coordinates.x, normalised_device_coordinates.y, 0.0f, 1.0f).TransformW(projectionInverse);

		// PS Vita
		// The frustrum on the Vita runs from -1 to 1.
		near_point = gef::Vector4(normalised_device_coordinates.x, normalised_device_coordinates.y, -1.0f, 1.0f).TransformW(projectionInverse);
		far_point = gef::Vector4(normalised_device_coordinates.x, normalised_device_coordinates.y, 1.0f, 1.0f).TransformW(projectionInverse);

		//
		near_point /= near_point.w();
		far_point /= far_point.w();

		// Work out the start point of the ray and it's direction.
		start_point = gef::Vector4(near_point.x(), near_point.y(), near_point.z());
		direction = far_point - near_point;
		direction.Normalise();
	}
}

bool ARApp::RayToSphere(GameObject& game_object, gef::Vector4& ray_start, gef::Vector4& ray_direction)
{
	//First, let's see if the point is inside the sphere. If so, return true
	if (PointInSphere(game_object, ray_start))
	{
		return true;
	}

	//Create a vector from the ray's start to the sphere's center
	gef::Vector4 vecV1(game_object.GetCollisionSphere()->position() - ray_start);

	//Project this vector onto the ray's direction vector
	float fD = vecV1.DotProduct(ray_direction);

	//If the ray is pointing away
	if (fD < 0.0f)
	{
		return false;
	}

	//Calculate the closest point to the sphere
	gef::Vector4 vecClosestPoint(ray_start + (ray_direction * fD));

	//Check if that point is inside the sphere
	return (PointInSphere(game_object, vecClosestPoint));
}

bool ARApp::PointInSphere(GameObject& game_object, gef::Vector4& point)
{
    //Calculate the squared distance from the point to the center of the sphere
    //gef::Vector4 vecDist(tSph.m_vecCenter - vecPoint);
	gef::Vector4 vecDist(game_object.GetCollisionSphere()->position() - point);

    //float fDistSq( D3DXVec3Dot( &vecDist, &vecDist) );
	float fDistSq(vecDist.DotProduct(vecDist));

    //Calculate if the squared distance between the sphere's center and the point
    //is less than the squared radius of the sphere


	if (fDistSq < (game_object.GetCollisionSphere()->radius() * game_object.GetCollisionSphere()->radius()))
	{
		return true;
	}

    //If not, return false
    return false;
}

/*
Hey James.

So the current problem I'm facing is this:
The position of the collision sphere is in local space relative to the mesh.
What I need is the collision sphere's position in world coordinates.
	=> collision_sphere_position*game object position
The next problem is that I have a function for 

*/