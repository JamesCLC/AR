#include "Level.h"



Level::Level(gef::Platform& platform) :
	GameState(platform),
	renderer_3d_(NULL),
	font_(NULL),
	sprite_renderer_(NULL),
	game_manager_(NULL)
{
}

Level::~Level()
{
}

void Level::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform);
	renderer_3d_ = gef::Renderer3D::Create(platform);
	//primitive_builder_ = new PrimitiveBuilder(platform_);


	/// 2D Camera Feed
	// Set up the Ortho Matrix for rendering the camera feed.
	ortho_matrix_.SetIdentity();	// Probably unneccesary.
	ortho_matrix_ = platform.OrthographicFrustum(-1, 1, -1, 1, -1, 1);	// Numbers taken from tutorial sheet.

	// Calculate y-scaling factor (based on resolution of camera and resolution of screen.)
	scaling_factor_ = ((platform.width() / platform.height()) / (SCE_SMART_IMAGE_WIDTH / SCE_SMART_IMAGE_HEIGHT));

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
	unscaled_projection_matrix_ = platform.PerspectiveProjectionFov(SCE_SMART_IMAGE_FOV, (SCE_SMART_IMAGE_WIDTH / SCE_SMART_IMAGE_HEIGHT), 0.1f, 100.0f);

	// Create the Scaling Matrix
	scaling_matrix_.SetIdentity();
	scaling_matrix_.set_m(1, 1, scaling_factor_);


	// Create the final scaled matrix. 
	scaled_projection_matrix_.SetIdentity();
	scaled_projection_matrix_ = unscaled_projection_matrix_ * scaling_matrix_;

	// Create the view matrix.
	view_matrix.SetIdentity();

	// Create and Initialise the Game Manager.
	game_manager_ = new GameManager(platform, renderer_3d_);
	game_manager_->Init(scaled_projection_matrix_, view_matrix);

	// Create a Debug Sphere
	//debug_sphere.set_mesh(primitive_builder_->GetDefaultCubeMesh());
																
	InitFont();
	SetUpLights();

	// initialise sony framework
	sampleInitialize();
	smartInitialize();

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);
}

bool Level::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	AppData* dat = sampleUpdateBegin();

	// use the tracking library to try and find markers
	smartUpdate(dat->currentImage);

	// check to see if a particular marker can be found
	if (sampleIsMarkerFound(marker_id))
	{
		// marker is being tracked, get it’s transform
		sampleGetTransform(marker_id, &marker_transform_);

		// Perfomr all gameplay/ collision code.
		game_manager_->Update(frame_time, marker_transform_);
	}


	/// DEBUG
	/*debug_matrix.SetIdentity();
	debug_matrix.Scale(gef::Vector4(0.1f, 0.1f, 0.1f));
	debug_matrix.SetTranslation(test_->GetTranslation());
	debug_sphere.set_transform(debug_matrix);*/
	/// END DEBUG


	sampleUpdateEnd(dat);

	return true;
}

void Level::Render()
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
		renderer_3d_->set_view_matrix(view_matrix);

		// Begin rendering 3D meshes, don't clear the frame buffer
		renderer_3d_->Begin(false);

		// The game manager renders all the game objects it contains.
		game_manager_->Render();

		renderer_3d_->End();

	RenderOverlay();

	sampleRenderEnd();
}

void Level::CleanUp()
{
	//delete primitive_builder_;
	//primitive_builder_ = NULL;

	if (game_manager_)
	{
		game_manager_->Cleanup();
		delete game_manager_;
		game_manager_ = NULL;
	}

	smartRelease();
	sampleRelease();

	CleanUpFont();

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;
}

void Level::RenderOverlay()
{
	//
	// render 2d hud on top
	//
	gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform.OrthographicFrustum(0.0f, platform.width(), 0.0f, platform.height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}

void Level::InitFont()
{
	font_ = new gef::Font(platform);
	font_->Load("comic_sans");
}

void Level::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void Level::DrawHUD()
{
	if (font_)
	{
		//// Degub - Display the position of the touch
		//if (active_touch_id != -1)
		//{
		//	font_->RenderText(
		//		sprite_renderer_,
		//		gef::Vector4(touch_position.x, touch_position.y, -0.9f),
		//		1.0f, 0xffffffff, gef::TJ_LEFT, "(%.1f, %.1f)",
		//		touch_position.x, touch_position.y);
		//}

		// Display framerate text
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

void Level::SetUpLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}
