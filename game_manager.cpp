#include "game_manager.h"

GameManager::GameManager(gef::Platform& platform, gef::Renderer3D * renderer_3d) :
	platform_(platform),
	renderer_3d_(renderer_3d),
	input_manager_(NULL),
	collision_manager(NULL),
	//ai_manager(NULL),
	active_touch_id(-1)
{
}


GameManager::~GameManager()
{
}

void GameManager::Init(gef::Matrix44 projection, gef::Matrix44 view)
{
	// Initialise touch input
	input_manager_ = gef::InputManager::Create(platform_);

	// Enable the front touch screen.
	if (input_manager_ && input_manager_->touch_manager() && (input_manager_->touch_manager()->max_num_panels() > 0))
	{
		input_manager_->touch_manager()->EnablePanel(0);
	}

	// Create the GameObjects
	for (int i = 0; i < num_of_objects; i++)
	{
		game_object_container.push_back(new GameObject(platform_, "balls/ball1.scn"));
	}

	// Create the collision detection manager.
	collision_manager = new CollisionManager(platform_, game_object_container, projection, view);

	// Create the AI manager.
	//ai_manager = new AIManager(game_object_container);
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

	// Make sure the input manager is valid.
	if (input_manager_ && input_manager_->touch_manager())
	{
		input_manager_->Update();

		// Check to see if there's any touch input.
		if (ProcessTouchInput())
		{
			// Make sure the collision detection manager is valid.
			if (collision_manager)
			{
				// Perform a raytrace against all the game objects.
				// Function returns a pointer to that object if it's hit
				// Returns NULL if nothing is hit.
				if (collision_manager->Raytrace(touch_position) != NULL)
				{
					// The ray has hit something.
					int foo = 0;
				}
				else
				{
					// The ray has not hit anything.
					int bar = 0;
				}
			}
		}
	}
}

void GameManager::Render()
{
	for (std::vector<GameObject*>::iterator it = game_object_container.begin(); it != game_object_container.end(); it++)
	{
		GameObject* ptr = (*it);
		renderer_3d_->DrawMesh(*(gef::MeshInstance*)ptr); // NEED TO REPLACE THIS WITH Draw Skinned Mesh or something. See animated_mesh for details.
	
	}
}

void GameManager::Cleanup()
{
	// Delete the game objects.
	game_object_container.clear();

	// Delete the input manager
	if (input_manager_)
	{
		delete input_manager_;
		input_manager_ = NULL;
	}

	if (collision_manager)
	{
		collision_manager->CleanUp();
		delete collision_manager;
		collision_manager = NULL;
	}

	/*if (ai_manager)
	{
		ai_manager->CleanUp();
		delete ai_manager;
		ai_manager = NULL;
	}*/

	// Note: Platform and renderer3D objects don't need to be cleaned up 
	// as they aren't allocated from the heap here.
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
				}
			}
		}
	}

	return isTouch;
}
