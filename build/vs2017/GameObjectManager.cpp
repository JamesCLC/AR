#include "GameObjectManager.h"



GameObjectManager::GameObjectManager(gef::Platform& platform, gef::Matrix44& marker_origin) :
	platform_(platform),
	marker_origin_(marker_origin)
{
	// Reserve a certain amount of space in the varray?
	for (int i = 0; i < sizeof(game_object_container); i++)
	{
		game_object_container[i] = new GameObject(platform_, "balls/ball1.scn");
	}
}


GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::Update()
{

	for (int i = 0; i < sizeof(game_object_container); i++)
	{
		// perform AI check?


		// If  the game object is in the air
		// apply velocity?

		// For now, just render my object on the marker.
		game_object_container[i]->SetTransform(marker_origin_);
	}
}

void GameObjectManager::Render(gef::Renderer3D & renderer_3D_)
{

	for (int i = 0; i < sizeof(game_object_container); i++)
	{
		renderer_3D_.DrawMesh(*(gef::MeshInstance*)game_object_container[i]);
	}
}
