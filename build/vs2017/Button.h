#pragma once
#include "maths\vector4.h"
#include "graphics\sprite.h"
#include "graphics\texture.h"

class Button
{
public:
	Button();
	~Button();

private:
	gef::Vector4 position_;
	gef::Texture* button_texture_;
	gef::Sprite button_sprite_;
	
};

