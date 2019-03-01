#include "Spikes.h"



Spike::Spike()
{
}

Spike::Spike(gef::Platform & platform_, std::string n_scene_filename_) :
	GameObject(platform_, n_scene_filename_)
{
	// Need to scale down the imported model. This is fairly arbitrary, so please forgive the magic numbers.
	scale_matrix_.Scale(gef::Vector4(0.025f, 0.025f, 0.025f));
}


Spike::~Spike()
{
}

void Spike::Update(gef::Matrix44 & marker_transform)
{
	SetTransform(marker_transform);
}
