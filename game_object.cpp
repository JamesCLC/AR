#include "game_object.h"



GameObject::GameObject()
{
	
}


GameObject::GameObject(gef::Platform& platform_, std::string n_scene_filename_) : 
	scene_filename_(n_scene_filename_)
{
	// Read relevent data from the scene file.
	if (!scene_filename_.empty())
	{
		ReadSceneFile(platform_);
	}

	// Need to scale down the imported model. This is fairly arbitrary, so please forgive the magic numbers.
	scale_matrix_.Scale(gef::Vector4(0.00125f, 0.00125f, 0.00125f));
}


GameObject::~GameObject()
{

}

void GameObject::Update(gef::Matrix44& marker_transform)
{
	// Get the next command from the AI Componenet
	ai_component.Update(marker_transform);

	// TO DO - Verify this command via the CDM.

	// TO DO - If this command is verified, execute it.
}

// Returns a collision sphere centred at the game object's current location.
const gef::Sphere GameObject::GetCollisionSphere()
{
	gef::Sphere transformed_sphere = mesh_->bounding_sphere().Transform(this->transform_);

	return transformed_sphere;
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




