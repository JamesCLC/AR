#include "game_object.h"



GameObject::GameObject()
{

}


GameObject::GameObject(gef::Platform& platform_, std::string n_scene_filename_) : 
	scene_filename_(n_scene_filename_)
{
	Initialise(platform_);	// Consider changing this. Keep the class interface uniform!
}


GameObject::~GameObject()
{

}

void GameObject::Initialise(gef::Platform& platform_)
{
	// Read relevent data from the scene file.
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
	else
	{
		// There's no data in the scene file!
	}
}


void GameObject::SetTranslation(gef::Vector4 n_translation)
{
	transform_.SetTranslation(n_translation);
}

gef::Vector4 GameObject::GetTranslation()
{
	return transform_.GetTranslation();
}

// Returns the position of the collision sphere in world coordinates.
const gef:: Vector4 GameObject::CollisionSpherePosition()
{
	return (GetCollisionSphere()->position());
}





