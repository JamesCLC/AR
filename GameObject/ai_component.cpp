#include "ai_component.h"
#include "m:\AppliedGamesTechnology\ar_app\game_object.h"



AIComponent::AIComponent(GameObject& owner_) : 
	owner(owner_)
{
}


AIComponent::~AIComponent()
{
}

void AIComponent::Update(gef::Matrix44& marker_transform)
{

}
