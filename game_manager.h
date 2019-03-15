#pragma once

#include "GameObject\Creature.h"
#include "GameObject\Spikes.h"
#include "GameObject\Marker.h"
#include "../collision_manager.h"
#include "GameStates\game_over.h"
#include "graphics\renderer_3d.h"
#include <input\input_manager.h>
#include <input\touch_input_manager.h>
#include <vector>

class GameState;

class GameManager
{
public:
	GameManager(gef::Platform& platform, gef::Renderer3D* renderer_3d, std::vector<Marker>& markers, 
		GameState* game_over, GameState* victory, int difficulty);
	~GameManager();

	GameState* Update(float frame_time);
	void Init(gef::Matrix44 projection, gef::Matrix44 view);
	void Render();
	void Cleanup();

	// Returns a reference to the Creatures.
	std::vector<Ball*>* GetCreatureObjects() { return &ball_container; };
	std::vector<Spike*>* GetSpikeObjects() { return &spike_container; };

	int GetPlayerScore() { return player_score_; };

private:
	bool ProcessTouchInput();
	
	gef::Platform& platform_;
	gef::InputManager* input_manager_;
	class gef::Renderer3D* renderer_3d_;

	// Structures to store all the game objects.
	std::vector<Ball*> ball_container;
	std::vector<Spike*> spike_container;

	// Class that handles collision detection between game objects.
	CollisionManager* collision_manager;

	// The markers
	std::vector<Marker>& markers_;

	// Game States
	GameState* victory_;
	GameState* game_over_;

	// Touch Input
	Int32 active_touch_id;
	gef::Vector2 touch_position;

	// Game Object variables.
	int num_of_balls = 5;
	int max_distance = 100;
	int min_distance = 30;

	// How close to the ground the objects have to be before the player dies.
	float safe_threshold = 0.1f;
	int difficulty_;
	int player_score_ = 0;
};

