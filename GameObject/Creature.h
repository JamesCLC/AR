#pragma once
#include "m:\AppliedGamesTechnology\ar_app\game_object.h"


class Creature : public GameObject
{
public:

public:
	enum State
	{
		Walk,
		Hold,
		Fall,
		Dead
	};

public:
	Creature();
	Creature(gef::Platform& platform_, std::string n_scene_filename_, gef::Vector4 starting_position);
	~Creature();

	void Update(gef::Matrix44& marker_transform); // frame time?

	// AI State
	void SetState(State new_state) { current_state = new_state; };
	State GetState() { return current_state; };

private:
	// Behaviours
	void Execute_Walk(gef::Matrix44&);
	void Execute_Hold(gef::Matrix44&);
	void Execute_Fall(gef::Matrix44&);
	void Execute_Die();

	// The current state this object is in
	State current_state = Walk;

	// This object's distance from the marker.
	gef::Vector4 position;

	float velocity = 0.00196f;

	bool is_alive = true;
};

