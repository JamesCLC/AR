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

	//gef::Sphere new_bounds(gef::Vector4(0.0f, 0.0f, 0.0f), 50.5f);

	//mesh_->set_bounding_sphere(new_bounds);
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




