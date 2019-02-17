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
	level_(NULL),
	main_menu_(NULL),
	game_over_(NULL),
	victory_(NULL),
	current_state_(NULL)
{
	// Create the game states.
	game_over_ = new GameOver(platform_);
	level_ = new Level(platform);
	main_menu_ = new MainMenu(platform);
	victory_ = new Victory(platform);

	// Give the states references to the other states they can transition to.
	game_over_->SetUpStates(level_, main_menu_);
	level_->SetUpGameStates(game_over_, victory_);
	main_menu_->SetUpGameStates(level_);
	victory_->SetUpStates(level_, main_menu_);

	// Begin the application on the main menu.
	current_state_ = main_menu_;
}

void ARApp::Init()
{
	// Initialise the current game state.
	if (current_state_)
	{
		current_state_->Init();
	}
}

void ARApp::CleanUp()
{
	// Clean up the current game state.
	if (current_state_)
	{
		current_state_->CleanUp();
	}
}

bool ARApp::Update(float frame_time)
{
	GameState* return_state = NULL;

	// Update the current game state.
	if (current_state_)	
	{
		return_state = current_state_->Update(frame_time);

		// If the state changes, clean up the previous state and initialise the new one.
		if (return_state != NULL)
		{
			current_state_->CleanUp();
			current_state_ = return_state;
			current_state_->Init();
		}
	}

	return true;
}

void ARApp::Render()
{
	// Render the current game state.
	if (current_state_)
	{
		current_state_->Render();
	}
}

