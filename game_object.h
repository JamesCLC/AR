#pragma once

#include <graphics/mesh_instance.h>
#include <graphics/scene.h>
#include <maths/matrix44.h>
#include <string>

///
#include <graphics/mesh.h>
///

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
}

class GameObject : gef::MeshInstance
{

public:
	GameObject();
	GameObject
	(
		gef::Platform& platform_,
		gef::Vector4 n_velocity,
		std::string n_scene_filename_
	);
	~GameObject();

	void UpdatePosition();
	void SetPosition(gef::Matrix44 n_position) { transform_ = n_position; }
	void SetVelocity(gef::Vector4 n_velocity) { velocity_ = n_velocity; }
	const gef::Mesh * GetMesh() { return mesh_; };
	const gef::Sphere * GetCollisionSphere() { return  &(mesh_->bounding_sphere()); };

	// Return's the Game Object's World Transform
	const gef::Matrix44& transform() { return transform_; }

private:
	void Initialise(gef::Platform& platform_);
	void ReadSceneFile(gef::Platform& platform_);

private:
	std::string scene_filename_;
	gef::Scene scene_;

	gef::Vector4 velocity_;


	// The Game Object's transform relative to the marker
	gef::Matrix44 local_transform;
};



