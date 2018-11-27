#pragma once

#include <graphics/mesh_instance.h>
#include <graphics/scene.h>
#include <maths/matrix44.h>
#include <string>
#include <graphics/mesh.h>


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
		std::string n_scene_filename_
	);
	~GameObject();

	// Transform
	void SetTransform(gef::Matrix44 n_transform) { transform_ = n_transform; }	
	const gef::Matrix44& GetTransform() { return transform_; }

	// Translation
	void SetTranslation(gef::Vector4 n_translation) { transform_.SetTranslation(n_translation); }
	gef::Vector4 GetTranslation() { return transform_.GetTranslation(); }


	// Collision Sphere
	const gef::Sphere * GetCollisionSphere(){ return &(mesh_->bounding_sphere()); }
	const gef::Vector4 GetCollisionSpherePosition() { return (GetCollisionSphere()->position()); }	// Optimise this?

	// Mesh
	const gef::Mesh * GetMesh() { return mesh_; }

	

private:
	void Initialise(gef::Platform& platform_);
	void ReadSceneFile(gef::Platform& platform_);

private:
	std::string scene_filename_;
	gef::Scene scene_;

};



