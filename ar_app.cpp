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
	primitive_builder_(NULL)
{
}

void ARApp::Init()
{
	input_manager_ = gef::InputManager::Create(platform_);
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	primitive_builder_ = new PrimitiveBuilder(platform_);

	///	Can probs move most of this to the constructor or header or something.

	/// 2D Camera Feed
		// Set up the Ortho Matrix for rendering the camera feed.
		ortho_matrix_.SetIdentity();
		ortho_matrix_ = platform_.OrthographicFrustum(-1, 1, -1, 1, -1, 1);	// Numbers taken from tutorial sheet.
	
		// Calculate y-scaling factor (based on resolution of camera and resolution of screen.)
		scaling_factor_ = ((960.0f / 544.0f) / (640.0f / 480.0f));
	
		// Set the texture's scale.
		camera_feed_sprite_.set_width(2.0f);
		camera_feed_sprite_.set_height(2.0f * scaling_factor_);

		// Place the sprite at the back of the orthographic fustrum.
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
			// Can replace this with a single function "Create Scale Matrix" or something.

		// Create the final scaled matrix.
		scaled_projection_matrix_.SetIdentity();
		scaled_projection_matrix_ = unscaled_projection_matrix_ * scaling_matrix_;

		// Create an identity matrix.
		identity_.SetIdentity();

		// Create default box mesh for testing.
		box_mesh_.set_mesh(primitive_builder_->GetDefaultCubeMesh());

		box_scale_matrix.Scale(gef::Vector4(0.1f, 0.1f, 0.1f));
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

	input_manager_->Update();

	AppData* dat = sampleUpdateBegin();

	// use the tracking library to try and find markers
	smartUpdate(dat->currentImage);

	///
	// check to see if a particular marker can be found
	if (sampleIsMarkerFound(marker_id))
	{
		// marker is being tracked, get it’s transform
		gef::Matrix44 marker_transform;
		sampleGetTransform(
			marker_id,
			&marker_transform);

		// set the transform of the 3D mesh instance to draw on
		// top of the marker
		box_mesh_.set_transform((box_scale_matrix*marker_transform));
	}

	///

	sampleUpdateEnd(dat);

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
	renderer_3d_->DrawMesh(box_mesh_);

	renderer_3d_->End();

	RenderOverlay();

	sampleRenderEnd();
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
