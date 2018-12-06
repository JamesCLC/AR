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
	current_state_(NULL)
{
	level_ = new Level(platform);
	main_menu_ = new MainMenu(platform, level_);

	current_state_ = main_menu_;
}

void ARApp::Init()
{
	if (current_state_)
	{
		current_state_->Init();
	}
}

void ARApp::CleanUp()
{
	if (current_state_)
	{
		current_state_->CleanUp();
	}
}

bool ARApp::Update(float frame_time)
{
	GameState* return_state = NULL;

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

	// TO DO - ALLOW FOR QUIT

	return true;
}

void ARApp::Render()
{
	if (current_state_)
	{
		current_state_->Render();
	}
}


/*
So, we have
a) The Game Object's transform in world space
b) The Marker's transform in world space

We want to:
a) Calculate the Game Object's transform relative to the marker
	=> Record that somehwere.
		=> Relative_Transform
b) Redefine the Game Object's transform to be relative to the marker
	=> Game Object Transform = marker transform * relative transform
c) Have the Game Object "Fall" down to the "plane" specified by the marker (i.e. Z = 0)

Other things to take care of:
a) Currently, the game object's position is being set to the marker's transform every frame. Gotta change that.
b) Need a way to know when to call the "Falling" behaviour.

Things to bear in mind:
a) If I move the camera while "holding" an object (which is inevitable) I'll need to re-orient orientate it relative to the marker transform
	This only really becomes neccessary once I'm using characters that have a defined "upright" position

*/
