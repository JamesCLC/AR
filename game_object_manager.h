#pragma once

#include "m:\AppliedGamesTechnology\ar_app\game_object.h"
#include "graphics\renderer_3d.h"

class gef::Platform;

class GameObjectManager
{
public:
	GameObjectManager(gef::Platform& platform, gef::Renderer3D* renderer_3d);
	~GameObjectManager();

	void Update(float frame_time, gef::Matrix44& marker_transform);
	void Init();
	void Render();
	void Cleanup();

private:
	class gef::Renderer3D* renderer_3d_;
	gef::Platform& platform_;

	// Structure to store all the game objects.
	GameObject* game_object_container[1];
};

// tie the number of game objects initialised into the difficulty?
//Like if difficulty is from 1-5, multiply that by 2 or 3 for the number of game objects spawned.

