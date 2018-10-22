#pragma once

#include <graphics/mesh_instance.h>


class GameObject : gef::MeshInstance
{

public:
	GameObject();
	~GameObject();

	void Update();

protected:
	gef::Vector4 velocity_;
	
};

