#pragma once
#include <platform/vita/system/platform_vita.h>
#include "GameObject\Creature.h"

class CollisionManager
{
public:
	CollisionManager(gef::Platform& platform_, std::vector<Creature*>&, gef::Matrix44, gef::Matrix44);
	~CollisionManager();

	// If the raytrace hits an object, it returns a pointer to that object.
	GameObject* Raytrace(gef::Vector2);
	void Update();
	void CleanUp();

private:
	void GetRay(gef::Vector4&, gef::Vector4&);
	bool RayToSphere(GameObject&);

	bool SphereToPlane(GameObject&);
	bool SphereToSphere(GameObject&, GameObject&);
	bool PointInSphere(GameObject&, gef::Vector4&);

	// Move these to the game AI class
	void TouchPositionWolrd(GameObject&);
	bool GameObjectFall(GameObject&, gef::Matrix44&);

	gef::Platform& platform_;

	// All the game objects in the game. Obtained from GameManager.
	std::vector<GameObject*>& game_object_container;

	// The game object the player is currently "holding" with their finger.
	GameObject* selected_object;

	// Raytracing
	gef::Vector4 ray_start;
	gef::Vector4 ray_direction;

	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;

	// Touch Input
	Int32 active_touch_id;
	gef::Vector2 touch_position;

	//gef::Matrix44 marker_origin_;
};

