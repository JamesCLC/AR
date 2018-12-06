#include "Button.h"



Button::Button() :
	button_texture(NULL)
{
}

Button::~Button()
{
}

void Button::Init(gef::Platform& platform, gef::Vector4 pos, GameState* transition_state)
{
	transition_state_ = transition_state;
	
	button_position = pos;
	button_position.set_z(-0.8f);

	button_texture = CreateTextureFromPNG("button_texture.png", platform);

	if (button_texture)
	{
		// Set up the sprite object.
		button_sprite.set_width(button_width);
		button_sprite.set_height(button_height);
		button_sprite.set_position(button_position);
		button_sprite.set_texture(button_texture);
	}

	// Set up the bounds for the button.

	CreateBounds();
}

void Button::CleanUp()
{
	if (button_texture)
	{
		delete button_texture;
		button_texture = NULL;
	}
}

void Button::SetPosition(float x, float y, float z)
{
	button_position.set_x(x);
	button_position.set_y(y);
	button_position.set_z(z);
}

GameState* Button::IsPressed(gef::Vector2 touch_position)
{
	// Create a game state pointer to be returned. When function is called,
	// it returns the appropriate state to transition to if it is pressed,
	// or a NULL pointer if it isn't.
	GameState* return_state = NULL;

	// Compare the touch position with the upper bounds.
	if ((touch_position.x >= bounds.upper_left.x) && (touch_position.y >= bounds.upper_left.y))
	{
		// Compare the touch position with the lower bounds
		if ((touch_position.x <= bounds.lower_right.x) && (touch_position.y <= bounds.lower_right.y))
		{
			// The touch is within the bounds.
			// Therefore, this button has been pressed.
			// Return the state to transition to.
			return_state = transition_state_;
		}
	}

	// The touch is not within the bounds.
	// Therefore, this button has not been pressed.
	return return_state;
}

void Button::SetText(std::string text)
{
	button_text = text;
}

void Button::CreateBounds()
{
	bounds.upper_left.x = button_position.x() - (button_width * 0.5);
	bounds.upper_left.y = button_position.y() - (button_height * 0.5);

	bounds.lower_right.x = button_position.x() + (button_width * 0.5);
	bounds.lower_right.y = button_position.y() + (button_height * 0.5);

}


