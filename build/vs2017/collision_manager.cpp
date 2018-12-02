#include "collision_manager.h"



CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::Update()
{
}



void CollisionManager::GetRay(gef::Vector4 & start_point, gef::Vector4 & direction, gef::Matrix44 & projection, gef::Matrix44 & view)
{
	//// Make sure the input manager and touch manager have been initialised.
	//if (input_manager_ && input_manager_->touch_manager())
	//{
	//	gef::Vector2 normalised_device_coordinates;

	//	float half_width = platform.width() * 0.5f;
	//	float half_height = platform.height() * 0.5f;

	//	// Calculate Normalised Device Cordinates (https://stackoverflow.com/questions/46749675/opengl-mouse-coordinates-to-space-coordinates/46752492)
	//	normalised_device_coordinates.x = (static_cast<float>(touch_position.x) - half_width) / half_width;
	//	normalised_device_coordinates.y = (half_height - static_cast<float>(touch_position.y)) / half_height;

	//	// Since we're working from the uniform world cordinates back to the trapezoid veiw frustrum, we need an inverse projection matrix.
	//	gef::Matrix44 projectionInverse;
	//	projectionInverse.Inverse(view * projection);

	//	// Define the start and end point of the ray (based on the frustrum of the camera.)
	//	gef::Vector4 near_point, far_point;

	//	// PS Vita
	//	// The frustrum on the Vita runs from -1 to 1.
	//	near_point = gef::Vector4(normalised_device_coordinates.x, normalised_device_coordinates.y, -1.0f, 1.0f).TransformW(projectionInverse);
	//	far_point = gef::Vector4(normalised_device_coordinates.x, normalised_device_coordinates.y, 1.0f, 1.0f).TransformW(projectionInverse);

	//	// Homogenise the vectors.
	//	near_point /= near_point.w();
	//	far_point /= far_point.w();

	//	// Work out the start point of the ray and it's direction.
	//	start_point = gef::Vector4(near_point.x(), near_point.y(), near_point.z());
	//	direction = far_point - near_point;
	//	direction.Normalise();
	//}
}

bool CollisionManager::SphereToPlane(GameObject &)
{
	// TO DO - Will need to

	return false;
}

bool CollisionManager::RayToSphere(GameObject& game_object, gef::Vector4& ray_start, gef::Vector4& ray_direction)
{
	//gef::Sphere transformed_sphere = game_object.GetMesh()->bounding_sphere().Transform(game_object.GetTransform());

	//First, let's see if the point is inside the sphere. If so, return true
	if (PointInSphere(game_object, ray_start))
	{
		return true;
	}

	//Create a vector from the ray's start to the sphere's center
	//gef::Vector4 vecV1(transformed_sphere.position() - ray_start);
	gef::Vector4 vecV1(game_object.GetCollisionSphere().position() - ray_start);

	//gef::Vector4 vecV1(game_object.GetTranslation() - ray_start);	// DEBUG

	//Project this vector onto the ray's direction vector
	float fD = vecV1.DotProduct(ray_direction);

	//If the ray is pointing away
	if (fD < 0.0f)
	{
		return false;
	}

	//Calculate the closest point to the sphere
	gef::Vector4 vecClosestPoint(ray_start + (ray_direction * fD));

	//Check if that point is inside the sphere
	return (PointInSphere(game_object, vecClosestPoint));
}

bool CollisionManager::SphereToSphere(GameObject& obj_1, GameObject& obj_2)
{
	//Calculate the squared distance between the centers of both spheres
	gef::Vector4 vecDist(obj_1.GetCollisionSphere().position() - obj_2.GetCollisionSphere().position());

	float fDistSq(vecDist.DotProduct(vecDist));

	//Calculate the squared sum of both radii
	float fRadiiSumSquared(obj_1.GetCollisionSphere().radius() + obj_2.GetCollisionSphere().radius());
	fRadiiSumSquared *= fRadiiSumSquared;

	//Check for collision
	//If the distance squared is less than or equal to the square sum
	//of the radii, then we have a collision
	if (fDistSq <= fRadiiSumSquared)
		return true;

	//I not, then return false
	return false;
}

bool CollisionManager::PointInSphere(GameObject& game_object, gef::Vector4& point)
{
	// Get a collision sphere that's in world space. MOVE TO INIT OR CONSTRUCTOR? UPDATE? Add this as a component of GameObject, and update it's transform whenever the game object is transformed?
	//gef::Sphere transformed_sphere = game_object.GetMesh()->bounding_sphere().Transform(game_object.GetTransform());

	//Calculate the squared distance from the point to the center of the sphere
	//gef::Vector4 vecDist(transformed_sphere.position() - point);
	gef::Vector4 vecDist(game_object.GetCollisionSphere().position() - point);


	//float fDistSq( D3DXVec3Dot( &vecDist, &vecDist) );
	float fDistSq(vecDist.DotProduct(vecDist));

	//Calculate if the squared distance between the sphere's center and the point
	//is less than the squared radius of the sphere
	if (fDistSq < (game_object.GetCollisionSphere().radius() * game_object.GetCollisionSphere().radius()))
	{
		return true;
	}


	//If not, return false
	return false;
}

void CollisionManager::MoveGameObject(GameObject& game_object, gef::Matrix44 & projection, gef::Matrix44 & view)
{
	//// Placeholder function. For now, I just want to be able to move the ball mesh around.
	//// First, get the touch's position in world space.
	//gef::Vector2 normalised_device_coordinates;

	//float half_width = platform.width() * 0.5f;
	//float half_height = platform.height() * 0.5f;

	//// Calculate Normalised Device Cordinates (https://stackoverflow.com/questions/46749675/opengl-mouse-coordinates-to-space-coordinates/46752492)
	//normalised_device_coordinates.x = (static_cast<float>(touch_position.x) - half_width) / half_width;
	//normalised_device_coordinates.y = (half_height - static_cast<float>(touch_position.y)) / half_height;

	//// Since we're working from the uniform world cordinates back to the trapezoid veiw frustrum, we need an inverse projection matrix.
	//gef::Matrix44 projectionInverse;
	//projectionInverse.Inverse(view * projection);

	//// Define the start and end point of the ray (based on the frustrum of the camera.)
	//gef::Vector4 near_point, far_point;

	//// PS Vita
	//// The frustrum on the Vita runs from -1 to 1.
	//near_point = gef::Vector4(normalised_device_coordinates.x, normalised_device_coordinates.y, -1.0f, 1.0f).TransformW(projectionInverse);
	//far_point = gef::Vector4(normalised_device_coordinates.x, normalised_device_coordinates.y, 1.0f, 1.0f).TransformW(projectionInverse);

	//// Homogenise the vectors.
	//near_point /= near_point.w();
	//far_point /= far_point.w();

	//// Work out the touch's position in world space.
	//gef::Vector4 touch_position_world = gef::Vector4(near_point.x(), near_point.y(), near_point.z());


	//// Next, compare the touch's current position with it's previous position.


	//// Apply the translation to the game object.
	////game_object.SetTranslation(game_object.GetTranslation() + gef::Vector4(0.0f, 0.05f, 0.05f));

	//game_object.SetTranslation(touch_position_world);
}

bool CollisionManager::GameObjectFall(GameObject & game_object, gef::Matrix44 & marker_transform)
{
	// Another Dummy Function. This time, I want the game object to fall back to the plane defined by the marker.


	gef::Matrix44 new_relative_transform = marker_transform;

	// Calculate the Game Object's translation relative to the marker. We can ignore rotation for now. Dunno about scale.
	gef::Vector4 relative_translation = (game_object.GetTranslation() - marker_transform.GetTranslation());

	// Redefine the Game Object's transform to be relative to the marker
	new_relative_transform.SetTranslation(relative_translation);
	//game_object.SetTransform(new_relative_transform);

	// If the object is currently "above" the marker plane
	if (relative_translation.z() > 0.0f)
	{
		gef::Vector4 gravity;
		gravity.set_z(-0.09f);

		//Have the Game Object "Fall" down to the "plane" specified by the marker(i.e.Z = 0)
		game_object.SetTranslation(game_object.GetTranslation() + gravity);

		return true;
	}

	return false;
}
