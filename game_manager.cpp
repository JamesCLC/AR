#include "game_manager.h"

GameManager::GameManager(gef::Platform& platform, gef::Renderer3D * renderer_3d) :
	platform_(platform),
	renderer_3d_(renderer_3d),
	input_manager_(NULL)
{
}


GameManager::~GameManager()
{
}

void GameManager::Init()
{
	input_manager_ = gef::InputManager::Create(platform_);

	// Initialise touch input
	if (input_manager_ && input_manager_->touch_manager() && (input_manager_->touch_manager()->max_num_panels() > 0))
	{
		input_manager_->touch_manager()->EnablePanel(0);
	}

	// Create the GameObjects
	for (int i = 0; i < num_of_objects; i++)
	{
		GameObject* ptr = new GameObject(platform_, "balls/ball1.scn");

		game_object_container.push_back(ptr);
	}
}

void GameManager::Update(float frame_time, gef::Matrix44& marker_transform)
{
	// Update each of the game objects.
	for (std::vector<GameObject*>::iterator it = game_object_container.begin(); it != game_object_container.end(); it++)
	{
		// Placeholder. Replace with Collision Update.
		(*it)->SetTransform(marker_transform);
	}

	//for (int i = 0; i < sizeof(game_object_container); i++)
	//{
	//	// For now, just put my object on the marker.
	//	game_object_container[i]->SetTransform(marker_transform);
	//}

	if (input_manager_)
	{
		input_manager_->Update();

		if (ProcessTouchInput())
		{
			int foo = 0;
		}
	}
}

void GameManager::Render()
{
	for (std::vector<GameObject*>::iterator it = game_object_container.begin(); it != game_object_container.end(); it++)
	{
		GameObject* ptr = (*it);

		renderer_3d_->DrawMesh(*(gef::MeshInstance*)ptr);
	}

	//for (int i = 0; i < sizeof(game_object_container); i++)
	//{
	//	renderer_3d_->DrawMesh(*(gef::MeshInstance*)game_object_container[i]);
	//}
}

void GameManager::Cleanup()
{

	if (input_manager_)
	{
		delete input_manager_;
		input_manager_ = NULL;
	}
}

bool GameManager::ProcessTouchInput()
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

					// game object fall
				}
			}
		}
	}

	return isTouch;
}
