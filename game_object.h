#pragma once

#include <graphics/mesh_instance.h>
#include <graphics/scene.h>
#include <maths/matrix44.h>
#include <string>

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
	const gef::Matrix44& transform() { return transform_; }

private:
	void Initialise(gef::Platform& platform_);
	void ReadSceneFile(gef::Platform& platform_);

private:
	gef::Vector4 velocity_;
	std::string scene_filename_;
	gef::Scene scene_;
};



