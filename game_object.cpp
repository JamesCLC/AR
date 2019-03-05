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


}

GameObject::~GameObject()
{

}

void GameObject::Update(gef::Matrix44& marker_transform)
{
	SetTransform(marker_transform);
}

// Returns a collision sphere centred at the game object's current location.
const gef::Sphere GameObject::GetCollisionSphere()
{
	gef::Sphere transformed_sphere = mesh_->bounding_sphere().Transform(this->transform_);

	// The default collision sphere is too big for this game,
	// So reduce it's radius by half.
	transformed_sphere.set_radius(transformed_sphere.radius() / 2);

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
}






