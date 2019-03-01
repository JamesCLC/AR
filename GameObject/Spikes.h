#pragma once
#include "m:\AppliedGamesTechnology\ar_app\game_object.h"


class Spike : public GameObject
{
public:
	Spike();
	Spike(gef::Platform& platform_, std::string n_scene_filename_);
	~Spike();
};

