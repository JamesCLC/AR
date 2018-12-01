#pragma once

#include "m:\AppliedGamesTechnology\ar_app\game_object.h"
#include "graphics\renderer_3d.h"

class gef::Platform;

class GameObjectManager
{
public:
	GameObjectManager(gef::Platform& platform, gef::Matrix44& marker_origin);
	~GameObjectManager();

	void Update();
	void Render(gef::Renderer3D& renderer_3D_);

private:
	gef::Platform& platform_;

	// Structure to store all the game objects.
	GameObject* game_object_container[1];

	// The world space transform of marker 1, treated as the origin of the scene.
	gef::Matrix44 marker_origin_;
};

// tie the number of game objects initialised into the difficulty?
//Like if difficulty is from 1-5, multiply that by 2 or 3 for the number of game objects spawned.

