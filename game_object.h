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

	void SetTransform(gef::Matrix44 n_transform) { transform_ = n_transform; }	
	const gef::Matrix44& GetTransform() { return transform_; }					// Return's the Game Object's World Transform

	void SetTranslation(gef::Vector4 n_translation) {}
	gef::Vector4 GetTranslation();

	const gef::Mesh * GetMesh() { return mesh_; };

	const gef::Sphere * collision_sphere() { return &(mesh_->bounding_sphere()); }
	const gef::Vector4 CollisionSpherePosition();;

	

private:
	void Initialise(gef::Platform& platform_);
	void ReadSceneFile(gef::Platform& platform_);

private:
	std::string scene_filename_;
	gef::Scene scene_;

	// The Game Object's transform relative to the marker
	gef::Matrix44 local_transform;
};



