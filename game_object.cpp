#include "game_object.h"



GameObject::GameObject()
{

}


GameObject::GameObject(gef::Platform& platform_, gef::Vector4 n_velocity, std::string n_scene_filename_)
{
	velocity_ = n_velocity;
	scene_filename_ = n_scene_filename_;

	Initialise(platform_);
}


GameObject::~GameObject()
{

}

void GameObject::Initialise(gef::Platform& platform_)
{
	if (!scene_filename_.empty())
	{
		ReadSceneFile(platform_);
	}
}

void GameObject::ReadSceneFile(gef::Platform & platform_)
{
	// Read the specified scene file.
	scene_.ReadSceneFromFile(platform_, scene_filename_.data());

	// Extract the materials from the scene file.
	scene_.CreateMaterials(platform_);

	// If there's mesh data in the scene, generate a mesh from the scene file.
	if (scene_.mesh_data.size() > 0)
	{
		mesh_ = scene_.CreateMesh(platform_, scene_.mesh_data.front());
	}
}

void GameObject::UpdatePosition()
{
	if (velocity_.Length() > 0.0f)
	{
		// update the object's position if it has a velocity.
		transform_.SetTranslation(transform_.GetTranslation() + velocity_);
	}


}



