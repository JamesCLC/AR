#pragma once
#include "m:\AppliedGamesTechnology\ar_app\game_object.h"

class AIManager
{
public:
	AIManager(std::vector<GameObject*>&);
	~AIManager();

	void Init();
	void Update();
	void CleanUp();

private:
	// All the game objects in the game. Obtained from GameManager.
	std::vector<GameObject*>& game_object_container;

};

