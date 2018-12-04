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

	void Init(gef::Platform& platform, gef::Vector4 pos, std::string text);
	void CleanUp();

	gef::Vector4 GetPosition() { return button_position_; };
	void SetPosition(gef::Vector4 n_position) { button_position_ = n_position; };
	void SetPosition(float x, float y, float z);

	gef::Sprite* GetSprite() { return &button_sprite_; };

private:
	gef::Vector4 button_position_;
	gef::Texture* button_texture_;
	gef::Sprite button_sprite_;

	float button_width_ = 256;	// Replace with #defines?
	float button_height_ = 128;

	std::string button_text_;
	
};

