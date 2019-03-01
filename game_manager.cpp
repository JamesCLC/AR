#include "game_manager.h"

GameManager::GameManager(gef::Platform& platform, gef::Renderer3D * renderer_3d, GameState* game_over, GameState* victory) :
	platform_(platform),
	renderer_3d_(renderer_3d),
	game_over_(game_over),
	victory_(victory),
	input_manager_(NULL),
	collision_manager(NULL),
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

	// Seed the random number generator with time.
	srand(time(NULL));

	// Create the Spikes
	for (int k = 0; k < 10; ++k)
	{
		spike_object_containter.push_back(new Spike(platform_, "spikes/spikes.scn"));
	}

	// Create the Cratures
	for (int i = 0; i < num_of_objects; ++i)
	{
		// Give the game object a random starting position within a given range.
		gef::Vector4 starting_position;

		starting_position.set_y((rand() % max_distance + min_distance) / 50);
		starting_position.set_x((rand() % max_distance + min_distance) / 50);
		starting_position.set_z(0.0f);

		creature_object_container.push_back(new Creature(platform_, "balls/ball1.scn", starting_position));
	}

	// Create the collision detection manager.
	collision_manager = new CollisionManager(platform_, creature_object_container, spike_object_containter, projection, view);
}

GameState* GameManager::Update(float frame_time, gef::Matrix44& marker_transform)
{
	// Declarations.
	gef::Vector4 touch_position_world;
	gef::Vector4 distance_from_marker;
	GameState* return_state = NULL;

	// Over here, James
	// Make sure the input manager and touch input has been iniialised.
	//if (input_manager_ && input_manager_->touch_manager())
	//{
	//	// Check to see if there's any touch input.
	//	input_manager_->Update();	
	//	if (ProcessTouchInput())
	//	{
	//		// Make sure the collision detection manager is valid.
	//		if (collision_manager)
	//		{
	//			// Perform a raytrace against all the game objects.
	//			// If an object is hit, a pointer to that object is returned.
	//			// Returns NULL if nothing is hit.
	//			hit_object = collision_manager->Raytrace(touch_position);
	//			///
	//			// Problem: How can I tell when the ray is hitting a Creature, and not a spike?
	//			// Do I even need to bother?
	//			///
	//			if (hit_object && (hit_object->GetState() != Creature::Dead))
	//			{
	//				// The ray has hit something.
	//				// Tell that game object to die.
	//				hit_object->SetState(Creature::Dead);
	//				// Return the next game state.
	//				//return_state = victory_;
	//			}
	//		}
	//	}
	////}

	// Update the Spikes.
	for (std::vector<Spike*>::iterator it = spike_object_containter.begin(); it != spike_object_containter.end(); ++it)
	{
		(*it)->Update(marker_transform);
	}


	// Update the Creature Objects.
	for (std::vector<Creature*>::iterator it = creature_object_container.begin(); it != creature_object_container.end(); ++it)
	{
		(*it)->Update(marker_transform);

		// Check to see if they reached the central marker
		distance_from_marker = ((*it)->GetTranslation() - marker_transform.GetTranslation());

		if (distance_from_marker.Length() <= death_threshold)
		{
			// The player has died. Notify the level.
			//return_state = game_over_;
		}
	}

	// Perform general collision detection.
	collision_manager->Update();

	return return_state; // Should move this to here the return state is set. Better yet, ditch the return sate variable altogether.
}

void GameManager::Render()
{
	// Redender the spikes.
	for (std::vector<Spike*>::iterator it = spike_object_containter.begin(); it != spike_object_containter.end(); ++it)
	{
		renderer_3d_->DrawMesh(*(gef::MeshInstance*)(*it));
	}

	// Render the creatures that are still alive.
	for (std::vector<Creature*>::iterator it = creature_object_container.begin(); it != creature_object_container.end(); it++)
	{
		if ((*it)->GetState() != Creature::Dead)
		{
			renderer_3d_->DrawMesh(*(gef::MeshInstance*)(*it));
		}	
	}
}

void GameManager::Cleanup()
{
	// Delete the game objects.
	creature_object_container.clear();
	spike_object_containter.clear();

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
