#pragma once

#include <graphics/mesh_instance.h>
#include <graphics/scene.h>
#include <maths/matrix44.h>
#include <string>
#include <graphics/mesh.h>
//#include "M:\AppliedGamesTechnology\ar_app\GameObject\ai_component.h"


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
}



class GameObject : gef::MeshInstance	// Set to public, so I can remove some of the accessor functions.
{
public:
	enum State
	{
		Walk,
		Hold,
		Fall,
		Dead
	};


public:
	GameObject();
	GameObject(gef::Platform& platform_,std::string n_scene_filename_);
	~GameObject();

	void Update(gef::Matrix44& marker_transform); // frame time?

	// Transform
	void SetTransform(gef::Matrix44 n_transform) { transform_ = scale_matrix_ * n_transform; }
	const gef::Matrix44& GetTransform() { return transform_; }

	// Translation
	void SetTranslation(gef::Vector4 n_translation) { transform_.SetTranslation(n_translation); }
	gef::Vector4 GetTranslation() { return transform_.GetTranslation(); }

	// Collision Sphere
	const gef::Sphere GetCollisionSphere();

	// Mesh
	const gef::Mesh * GetMesh() { return mesh_; }
private:
	void ReadSceneFile(gef::Platform& platform_);

	// Behaviours
	void Execute_Walk(gef::Matrix44);
	void Execute_Hold(gef::Matrix44);
	void Execute_Fall(gef::Matrix44);
	void Execute_Die();

private:
	std::string scene_filename_;
	gef::Scene scene_;
	gef::Matrix44 scale_matrix_;
	gef::Vector4 velocity;

	// The current state this object is in
	State current_state = Walk;

	bool is_alive = true;
};



