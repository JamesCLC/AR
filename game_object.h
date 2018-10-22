#pragma once

#include <graphics/mesh.h>
#include <graphics/scene.h>
#include <maths/matrix44.h>
#include <string>

namespace gef
{
	class Platform;
}

class GameObject
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
	const gef::Mesh* mesh_;
	gef::Vector4 velocity_;
	const gef::Matrix44 transform_;
	std::string scene_filename_;
	gef::Scene scene_;
};

/*
Hey James. Removing inheritance entirely may have been a reasonable idea, but it may be that switching back is prudent.
*/

