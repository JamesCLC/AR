#include "game_over.h"



GameOver::GameOver(gef::Platform& platform) :
	GameState(platform),
	level_(NULL),
	main_menu_(NULL),
	font_(NULL),
	input_manager_(NULL),
	sprite_renderer_(NULL),
	background_texture_(NULL)
{
}

GameOver::~GameOver()
{
}

void GameOver::Init()
{
	// Create the sprite renderer.
	sprite_renderer_ = gef::SpriteRenderer::Create(platform);

	// Set up the Ortho Matrix for 2D rendering.
	ortho_matrix_.SetIdentity();
	ortho_matrix_ = platform.OrthographicFrustum(0.0f, 960.0f, 0.0f, 544.0f, -1, 1);


	if (sprite_renderer_)
	{
		// load in the texture for the background image.
		background_texture_ = CreateTextureFromPNG("GameOver.png", platform);

		if (background_texture_)
		{
			// Set the scale and position of the background image for the Vita.
			background_sprite_.set_height(platform.height());
			background_sprite_.set_width(platform.width());
			background_sprite_.set_position((platform.width() * 0.5), (platform.height() *0.5), -0.5f);
			background_sprite_.set_texture(background_texture_);
		}
	}

	// load in the font.
	font_ = new gef::Font(platform);
	font_->Load("comic_sans");

	// Initialise touch input
	input_manager_ = gef::InputManager::Create(platform);

	// Enable the front touch screen.
	if (input_manager_ && input_manager_->touch_manager() && (input_manager_->touch_manager()->max_num_panels() > 0))
	{
		input_manager_->touch_manager()->EnablePanel(0);
		active_touch_id = -1;
	}

	// Initialise the buttons.
	InitButtons();
}

GameState * GameOver::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	if (input_manager_)
	{
		// Get up-to-date player input.
		input_manager_->Update();

		if (ProcessTouchInput())
		{
			int foo = 0;
			// Check to see if any of the buttons have been pressed.
			for (std::vector<Button*>::iterator it = buttons.begin(); it != buttons.end(); it++)
			{
				// If it has, go to the corresponding game state.
				return (*it)->IsPressed(touch_position);
			}
		}
	}

	return NULL;
}

void GameOver::Render()
{
	if (sprite_renderer_)
	{
		// Prepare for Orthographic rendering.
		sprite_renderer_->set_projection_matrix(ortho_matrix_);

		// Begin rendering sprites.
		sprite_renderer_->Begin(true);

		// Render the background image.
		sprite_renderer_->DrawSprite(background_sprite_);

		// Render the buttons.
		for (std::vector<Button*>::iterator it = buttons.begin(); it != buttons.end(); it++)
		{
			sprite_renderer_->DrawSprite(*(*it)->GetSprite());
		}

		// Stop rendering sprites.
		sprite_renderer_->End();
	}

	RenderText();
}

void GameOver::CleanUp()
{
	// Delete the font object.
	if (font_)
	{
		delete font_;
		font_ = NULL;
	}

	// Delete the input manager.
	if (input_manager_)
	{
		delete input_manager_;
		input_manager_ = NULL;
	}

	// Delete the sprite renderer.
	if (sprite_renderer_)
	{
		delete sprite_renderer_;
		sprite_renderer_ = NULL;
	}

	// Delete the buttons.
	for (std::vector<Button*>::iterator it = buttons.begin(); it != buttons.end(); ++it)
	{
		delete (*it);
	}

	buttons.clear();


}

void GameOver::SetUpStates(GameState * level, GameState * main_menu)
{
	level_ = level;
	main_menu_ = main_menu;
}

void GameOver::RenderText()
{
	//
	// render 2d hud on top
	//
	gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform.OrthographicFrustum(0.0f, platform.width(), 0.0f, platform.height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);

	if (font_)
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

		// Render the text on the buttons.
		font_->RenderText(sprite_renderer_, buttons[0]->GetPosition(), 1.0f, 0xffffffff, gef::TJ_CENTRE, "Replay");
		font_->RenderText(sprite_renderer_, buttons[1]->GetPosition(), 1.0f, 0xffffffff, gef::TJ_CENTRE, "Options");
		font_->RenderText(sprite_renderer_, buttons[2]->GetPosition(), 1.0f, 0xffffffff, gef::TJ_CENTRE, "Quit");
	}

	sprite_renderer_->End();
}

bool GameOver::ProcessTouchInput()
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

void GameOver::InitButtons()
{
	for (int i = 1; (i < num_of_buttons + 1); i++)
	{
		buttons.push_back(new Button());

		buttons.back()->Init(platform, gef::Vector4((platform.width()*0.5f), (i*150.0f), -0.6f), level_);
	}
}
