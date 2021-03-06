#include "Level.h"



Level::Level(gef::Platform& platform) :
	GameState(platform),
	game_over_(NULL),
	victory_(NULL),
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

	// Create the Game Manager.
	game_manager_ = new GameManager(platform, renderer_3d_, markers, game_over_, 
		victory_, difficulty);

	// Initialise the Game Manager.
	game_manager_->Init(scaled_projection_matrix_, view_matrix);											
																
	InitFont();
	SetUpLights();

	// initialise sony framework
	sampleInitialize();
	smartInitialize();

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);

	// Create the structures to store marker data.
	// Need a marker for each spike + 1 for the cratrue's target.
	for (int i = 0; i < difficulty + 1; ++i)
	{
		Marker new_marker;
		new_marker.id = i;
		markers.push_back(new_marker);
	}
}

GameState* Level::Update(float frame_time)
{
	// If a change in state is required, this is used to return that next state.
	GameState* return_pointer = NULL;

	fps_ = 1.0f / frame_time;

	AppData* dat = sampleUpdateBegin();

	// use the tracking library to try and find markers
	smartUpdate(dat->currentImage);

	// Get the positions of all the neccesary markers.
	are_markers_visible = true;
	for (std::vector<Marker>::iterator it = markers.begin(); it != markers.end(); ++it)
	{
		// check to see if a particular marker can be found
		if (sampleIsMarkerFound(it->id))
		{
			// marker is being tracked, get it�s transform
			sampleGetTransform(it->id, &it->transform);
		}
		else // If any of the markers aren't visible, halt the game.
		{
			are_markers_visible = false;
			break;
		}
	}

	// Update the game only if all the neccesary markers are visible.
	if (are_markers_visible)
	{
		// Perform all gameplay & collision code.
		// If game logic dictates a state change (i.e. game over) this function returns a pointer to that state.
		return_pointer = game_manager_->Update(frame_time);
	}

	sampleUpdateEnd(dat);

	return return_pointer;
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

		// Render the gameplay objects if all the neccessary markers are visible.
		if (are_markers_visible)
		{
			// Begin rendering 3D meshes, don't clear the frame buffer
			renderer_3d_->Begin(false);

			// The game manager renders all the game objects it contains.
			game_manager_->Render();

			renderer_3d_->End();
		}

	RenderOverlay();

	sampleRenderEnd();
}

void Level::CleanUp()
{
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

void Level::SetUpGameStates(GameState * game_over, GameState* victory)
{
	game_over_ = game_over;
	victory_ = victory;
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
		// Display framerate text
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}

	// If the markers aren't present, tell the player to relocate them.
	if (!are_markers_visible)
	{
		// Render the text at the centre of the screen.
		font_->RenderText(sprite_renderer_, gef::Vector4((platform.width()/2), (platform.height() / 2), -0.9f), 1.0f, 0xffffffff, gef::TJ_CENTRE, "Markers not found!");
		font_->RenderText(sprite_renderer_, gef::Vector4((platform.width()/2), (platform.height() / 2) + 50, -0.9f), 1.0f, 0xffffffff, gef::TJ_CENTRE, "Please locate markers 1 to %d", difficulty + 1);
	}

	// Display the player's score.
	if (game_manager_ != nullptr)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4((platform.width() / 2), 100.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_CENTRE, "Score: %d", game_manager_->GetPlayerScore());
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
