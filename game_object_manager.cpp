#include "game_object_manager.h"

GameObjectManager::GameObjectManager(gef::Platform& platform, gef::Renderer3D * renderer_3d) :
	platform_(platform),
	renderer_3d_(renderer_3d)
{
}


GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::Init()
{


	// Reserve a certain amount of space in the varray? b

	// load in the data from the scene files.
	for (int i = 0; i < sizeof(game_object_container); i++)
	{
		game_object_container[i] = new GameObject(platform_, "balls/ball1.scn");
	}
}

void GameObjectManager::Update(float frame_time, gef::Matrix44& marker_transform)
{
	for (int i = 0; i < sizeof(game_object_container); i++)
	{
		// perform AI check?


		// If  the game object is in the air
		// apply velocity?

		// For now, just put my object on the marker.
		game_object_container[i]->SetTransform(marker_transform);
	}
}

void GameObjectManager::Render()
{
	for (int i = 0; i < sizeof(game_object_container); i++)
	{
		renderer_3d_->DrawMesh(*(gef::MeshInstance*)game_object_container[i]);
	}
}

void GameObjectManager::Cleanup()
{
}
