#include "Button.h"



Button::Button()
{
}


Button::~Button()
{
}

bool Button::Init(gef::Platform& platform, gef::Vector4 pos, std::string text)
{
	button_texture_ = CreateTextureFromPNG("button_texture", platform);

	if (button_texture_)
	{
		button_sprite_.set_height(button_height_);
		button_sprite_.set_width(button_width_);
		button_sprite_.set_position(button_position_);
	}

	button_text_ = text;

	button_position_ = pos;

	return false;
}


gef::Vector4 Button::GetPosition()
{
	return button_position_;
}

void Button::SetPosition(gef::Vector4 n_position)
{
	button_position_ = n_position;
}

void Button::SetPosition(float x, float y, float z)
{
	button_position_.set_x(x);
	button_position_.set_y(y);
	button_position_.set_z(z);
}

gef::Sprite * Button::GetSprite()
{
	return &button_sprite_;
}
