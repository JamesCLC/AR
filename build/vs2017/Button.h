#pragma once
#include "maths\vector4.h"
#include "graphics\sprite.h"
#include "graphics\texture.h"
#include <string>
#include "m:\AppliedGamesTechnology\ar_app\load_texture.h"

class gef::Platform;

class Button
{
public:
	Button();
	~Button();

	bool Init(gef::Platform& platform, gef::Vector4 pos, std::string text);

	gef::Vector4 GetPosition();
	void SetPosition(gef::Vector4 n_position);
	void SetPosition(float x, float y, float z);

	gef::Sprite* GetSprite();

private:
	gef::Vector4 button_position_;
	gef::Texture* button_texture_;
	gef::Sprite button_sprite_;

	float button_width_;
	float button_height_;

	std::string button_text_;
	
};

