#include "MainMenu.h"



MainMenu::MainMenu(gef::Platform& platform) :
	GameState(platform),
	font_(NULL),
	input_manager_(NULL),
	sprite_renderer_(NULL)
{
}


MainMenu::~MainMenu()
{
}

void MainMenu::Init()
{

	sprite_renderer_ = gef::SpriteRenderer::Create(platform);

	if (sprite_renderer_)
	{
		background_texture_ = CreateTextureFromPNG("MainMenu.png", platform);
		if (background_texture_)
		{
			background_sprite_->set_texture(background_texture_);
			background_sprite_->set_height(platform.height);
			background_sprite_->set_width(platform.width);
			background_sprite_->set_position(450.0f, 300.0f, 0.0f);
		}
	}

	font_ = new gef::Font(platform);
	font_->Load("comic_sans");

	// Initialise touch input
	input_manager_ = gef::InputManager::Create(platform);

	if (input_manager_ && input_manager_->touch_manager() && (input_manager_->touch_manager()->max_num_panels() > 0))
	{
		input_manager_->touch_manager()->EnablePanel(0);
	}
}

bool MainMenu::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;


	if (input_manager_)
	{
		input_manager_->Update();

		if (ProcessTouchInput())
		{

		}
	}

	return true;
}

void MainMenu::Render()
{

	RenderText();
}

void MainMenu::CleanUp()
{
	if (font_)
	{
		delete font_;
		font_ = NULL;
	}

	if (input_manager_)
	{
		delete input_manager_;
		input_manager_ = NULL;
	}

	if (sprite_renderer_)
	{
		delete sprite_renderer_;
		sprite_renderer_ = NULL;
	}
	

	
}

void MainMenu::RenderText()
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
	}

	sprite_renderer_->End();
}

bool MainMenu::ProcessTouchInput()
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
