#include "game_object.h"



GameObject::GameObject()
{
	
}


GameObject::GameObject(gef::Platform& platform_, std::string n_scene_filename_, gef::Vector4 starting_position) :
	scene_filename_(n_scene_filename_),
	position(starting_position)
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
	// The game objects acts according to it's current state.
	switch (current_state)
	{
		case GameObject::State::Walk:
			Execute_Walk(marker_transform);
			break;
		case GameObject::State::Hold: // CHANGE THIS TO TOUCH POSITION WORLD
			Execute_Hold(marker_transform);
			break;
		case GameObject::State::Fall:// ADD TOUCH POSITION WORLD
			Execute_Fall(marker_transform);	
			break;
		case GameObject::State::Dead:
			Execute_Die();
			break;
		default:
			break;
	}
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
}

void GameObject::Execute_Walk(gef::Matrix44& marker_transfrom)
{
	//// Find the position of the marker in 3D space
	gef::Vector4 marker_position = marker_transfrom.GetTranslation();

	// get the marker's x-axis.
	gef::Vector4 x_axis = gef::Vector4(marker_transfrom.m(0,0),
		marker_transfrom.m(0, 1),
		marker_transfrom.m(0, 2));

	// get the marker's y-axis.
	gef::Vector4 y_axis = gef::Vector4(marker_transfrom.m(1, 0),
		marker_transfrom.m(1, 1),
		marker_transfrom.m(1, 2));
	
	// get the marker's z-axis.
	gef::Vector4 z_axis = gef::Vector4(marker_transfrom.m(2, 0),
		marker_transfrom.m(2, 1),
		marker_transfrom.m(2, 2));

	// The new position of this game object
	gef::Vector4 new_position;


	// Update the object's position based on it's velocity.
	// Upadate the X component.
	if (position.x() > 0)
	{
		position.set_x(position.x() - velocity);
	}
	else if (position.x() < 0)
	{
		position.set_x(position.x() + velocity);
	}
	// Update the Y component.
	if (position.y() > 0)
	{
		position.set_y(position.y() - velocity);
	}
	else if (position.y() < 0)
	{
		position.set_y(position.y() + velocity);
	}
	// Update the Z component.
	if (position.z() > 0)
	{
		position.set_z(position.z() - velocity);
	}
	else if (position.z() < 0)
	{
		position.set_z(position.z() + velocity);
	}

	// Update the object's position.
	new_position = marker_position + (x_axis * position.x()) + (y_axis * position.y()) + (z_axis * position.z());

	// Copy the marker transform to match marker rotation.
	SetTransform(marker_transfrom);

	// Override the position of the object transform 
	transform_.SetTranslation(new_position);
}

void GameObject::Execute_Fall(gef::Matrix44& marker_transfrom)
{
	// clalculate the object's height above the marker
	float height = transform_.GetTranslation().y() - marker_transfrom.GetTranslation().y();
	float gravity = 0.0098f;

	// Work out the object's new position after gravity has been applied.
	gef::Vector4 new_translation;
	new_translation = transform_.GetTranslation();
	new_translation.set_y(new_translation.y() - gravity);

	// Update the object's transform.
	transform_.SetTranslation(new_translation);
}

void GameObject::Execute_Hold(gef::Matrix44& touch_oposition_world)
{
	// Calculate the vector between this object and the touch's postion in world space.
	gef::Vector4 distance = transform_.GetTranslation() - touch_oposition_world.GetTranslation();

	// If the object isn't at the touch position
	if (distance.Length() >0)	// Replace with threshold value?
	{
		// calculate the unit vector of the distance
		distance.set_x(distance.x() / distance.LengthSqr());
		distance.set_y(distance.y() / distance.LengthSqr());
		distance.set_z(distance.z() / distance.LengthSqr());

		// Move the object by a given velocity along this unit vector.
		float velocity = 9.8f;
		gef::Vector4 new_translation;
		new_translation = transform_.GetTranslation() - (distance * velocity);

		// Update the object's transform.
		transform_.SetTranslation(new_translation);
	}
}

void GameObject::Execute_Die()
{
	is_alive = false;
}






