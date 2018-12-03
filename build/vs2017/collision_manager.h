#pragma once
#include <platform/vita/system/platform_vita.h>
#include "m:\AppliedGamesTechnology\ar_app\game_manager.h"


class CollisionManager
{
public:
	CollisionManager(gef::Platform& platform_,GameManager& game_manager);
	~CollisionManager();

	GameObject& Raytrace();

private:
	void GetRay(gef::Vector4&, gef::Vector4&, gef::Matrix44&, gef::Matrix44&);
	bool SphereToPlane(GameObject&);
	bool SphereToSphere(GameObject&, GameObject&);
	bool RayToSphere(GameObject&, gef::Vector4&, gef::Vector4&);
	bool PointInSphere(GameObject&, gef::Vector4&);

	// Move these to the game AI class
	void MoveGameObject(GameObject&, gef::Matrix44&, gef::Matrix44&);
	bool GameObjectFall(GameObject&, gef::Matrix44&);

	gef::Platform& platform_;

	GameManager& game_manager_;

	// Touch Input
	Int32 active_touch_id;
	gef::Vector2 touch_position;

	// Raytracing
	gef::Vector4 ray_start;
	gef::Vector4 ray_direction;
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
};

