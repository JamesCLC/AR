#pragma once

#include <graphics/mesh_instance.h>
#include <graphics/scene.h>
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
	void Initialise(gef::Platform& platform_);

	void SetVelocity(gef::Vector4 n_velocity) { velocity_ = n_velocity; }

private:
	gef::Vector4 velocity_;
	std::string scene_filename_;
	gef::Scene scene_;
};

