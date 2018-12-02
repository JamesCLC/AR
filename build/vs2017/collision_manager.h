#pragma once

#include "m:\AppliedGamesTechnology\ar_app\game_object.h"

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void Update();


private:
	void GetRay(gef::Vector4&, gef::Vector4&, gef::Matrix44&, gef::Matrix44&);
	bool SphereToPlane(GameObject&);
	bool SphereToSphere(GameObject&, GameObject&);
	bool RayToSphere(GameObject&, gef::Vector4&, gef::Vector4&);
	bool PointInSphere(GameObject&, gef::Vector4&);

	// Move these to the game AI class
	void MoveGameObject(GameObject&, gef::Matrix44&, gef::Matrix44&);
	bool GameObjectFall(GameObject&, gef::Matrix44&);

	// Touch Input - To Be Moved
	Int32 active_touch_id;
	gef::Vector2 touch_position;

	// Raytracing - To Be Moved
	gef::Vector4 ray_start;
	gef::Vector4 ray_direction;
};

