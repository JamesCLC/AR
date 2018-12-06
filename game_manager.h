#pragma once

#include "game_object.h"
#include "../collision_manager.h"
#include "graphics\renderer_3d.h"
#include <input\input_manager.h>
#include <input\touch_input_manager.h>
#include <vector>


class GameManager
{
public:
	GameManager(gef::Platform& platform, gef::Renderer3D* renderer_3d);
	~GameManager();

	bool Update(float frame_time, gef::Matrix44& marker_transform);
	void Init(gef::Matrix44 projection, gef::Matrix44 view);
	void Render();
	void Cleanup();

	// Returns a reference to the Game Objects.
	std::vector<GameObject*>* GetGameObjects() { return &game_object_container; };

private:
	bool ProcessTouchInput();
	
	gef::Platform& platform_;
	gef::InputManager* input_manager_;
	class gef::Renderer3D* renderer_3d_;

	// Structure to store all the game objects.
	std::vector<GameObject*> game_object_container;

	// Class that handles collision detection between game objects.
	CollisionManager* collision_manager;

	// Touch Input
	Int32 active_touch_id;
	gef::Vector2 touch_position;

	int num_of_objects = 1;
};

