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
	level(NULL),
	current_state_(NULL)
{
	level = new Level(platform);
	current_state_ = level;
}

void ARApp::Init()
{
	
	if (current_state_)
	{
		current_state_->Init();
	}

	/*else if (level)
	{
		level->Init();
	}*/
}

void ARApp::CleanUp()
{
	if (current_state_)
	{
		current_state_->CleanUp();
	}

	/*else if (level)
	{
		level->CleanUp();
	}*/
}

bool ARApp::Update(float frame_time)
{
	if (current_state_)
	{
		return current_state_->Update(frame_time);
	}

	// TO DO - Call Cleanup and Init when the stat changes?
	
	/*if (level)
	{
		return level->Update(frame_time);
	}*/

	return false;
}

void ARApp::Render()
{
	if (current_state_)
	{
		current_state_->Render();
	}

	/*if (level)
	{
		level->Render();
	}*/
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
