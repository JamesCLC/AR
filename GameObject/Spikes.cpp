#include "Spikes.h"



Spike::Spike()
{
}

Spike::Spike(gef::Platform & platform_, std::string n_scene_filename_, int n_marker_id) :
	GameObject(platform_, n_scene_filename_),
	marker_id(n_marker_id)
{
	// Need to scale down the imported model. This is fairly arbitrary, so please forgive the magic numbers.
	scale_matrix_.Scale(gef::Vector4(0.025f, 0.025f, 0.025f));
}


Spike::~Spike()
{
}
