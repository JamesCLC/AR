#pragma once
#include "maths\vector4.h"
#include "graphics\sprite.h"
#include "graphics\texture.h"
#include <string>
#include "m:\AppliedGamesTechnology\ar_app\load_texture.h"

class gef::Platform;
//class GameState;

struct Bounds
{
	// Structure that represents a rectagonal "collision box"
	// Used for detecting presses.
	// Rectangle is described by defining opposite corners.
	gef::Vector2 upper_left;
	gef::Vector2 lower_right;
};

class Button
{
public:
	Button();
	~Button();

	void Init(gef::Platform& platform, gef::Vector4 pos, std::string text);
	void CleanUp();

	gef::Vector4 GetPosition() { return button_position; };
	void SetPosition(gef::Vector4 n_position) { button_position = n_position; };
	void SetPosition(float x, float y, float z);
	bool IsPressed(gef::Vector2);		// Change to return game state pointer.

	gef::Sprite* GetSprite() { return &button_sprite; };

	std::string GetText() { return button_text; };

private:
	void CreateBounds();

	gef::Vector4 button_position;
	gef::Texture* button_texture;
	gef::Sprite button_sprite;
	std::string button_text;
	Bounds bounds;

	const float button_width = 256;
	const float button_height = 128;

	// Tells the appliation which state to transition to.
	//GameState* transition_state;
};

