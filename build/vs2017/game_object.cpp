#include "game_object.h"



GameObject::GameObject()
{

}


GameObject::~GameObject()
{

}

void GameObject::Update()
{
	if (velocity_.Length() > 0.0f)
	{
		// update the object's position if it has a velocity.
		transform_.SetTranslation(transform_.GetTranslation() + velocity_);
	}


}
