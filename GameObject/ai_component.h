#pragma once

class GameObject;

class AIComponent
{
public:
	AIComponent(GameObject&);
	~AIComponent();

	enum AIState {
		Walking,
		Pickup,
		Held,
		Falling,
		Dead
	};

	void Update(gef::Matrix44&);

private:
	AIState current_state;

	// The GameObject this componenet is attatched to.
	GameObject& owner;
};

