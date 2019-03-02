#pragma once
#include "m:\AppliedGamesTechnology\ar_app\game_object.h"


class Spike : public GameObject
{
public:
	Spike();
	Spike(gef::Platform& platform_, std::string n_scene_filename_, int n_marker_id);
	~Spike();

	int GetID() { return marker_id; };
private:
	// The I.D. of the marker this spike is being placed on.
	int marker_id;
};

