#include "Button.h"



Button::Button()
{
}


Button::~Button()
{
}

void Button::Init(gef::Platform& platform, gef::Vector4 pos, std::string text)
{
	button_text_ = text;

	button_position_ = pos;

	button_texture_ = CreateTextureFromPNG("button_texture", platform);

	if (button_texture_)
	{
		button_sprite_.set_height(button_height_);
		button_sprite_.set_width(button_width_);
		button_sprite_.set_position(button_position_);
		button_sprite_.set_texture(button_texture_);
	}	
}

void Button::CleanUp()
{
	if (button_texture_)
	{
		delete button_texture_;
		button_texture_ = NULL;
	}
}

void Button::SetPosition(float x, float y, float z)
{
	button_position_.set_x(x);
	button_position_.set_y(y);
	button_position_.set_z(z);
}


