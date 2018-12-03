#pragma once

#include "m:\AppliedGamesTechnology\ar_app\game_object.h"
#include "graphics\renderer_3d.h"
#include <input\input_manager.h>
#include <input\touch_input_manager.h>


class GameManager
{
public:
	GameManager(gef::Platform& platform, gef::Renderer3D* renderer_3d);
	~GameManager();

	void Update(float frame_time, gef::Matrix44& marker_transform);
	void Init();
	void Render();
	void Cleanup();
	GameObject** GetGameObjects() { return game_object_container; };

private:
	bool ProcessTouchInput();
	
	class gef::Renderer3D* renderer_3d_;
	gef::Platform& platform_;
	gef::InputManager* input_manager_;

	// Structure to store all the game objects.
	GameObject* game_object_container[1];

	// Touch Input - To Be Moved
	Int32 active_touch_id;
	gef::Vector2 touch_position;
};

// tie the number of game objects initialised into the difficulty?
//Like if difficulty is from 1-5, multiply that by 2 or 3 for the number of game objects spawned.

