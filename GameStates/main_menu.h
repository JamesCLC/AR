#pragma once

#include "graphics\texture.h"
#include "graphics\font.h"
#include "graphics\sprite.h"
#include "graphics\sprite_renderer.h"
#include "input\input_manager.h"
#include "input\touch_input_manager.h"

#include "game_state.h"
#include "..\load_texture.h"
#include "..\GameStates\button.h"

// Replace these includes with forward declarations?

class MainMenu : public GameState
{
public:
	MainMenu(gef::Platform& platform);
	~MainMenu();

	void Init();
	GameState* Update(float frame_time);
	void Render();
	void CleanUp();
	void SetUpGameStates(GameState* level/*, GameState* options*/);

private:
	void RenderText();
	bool ProcessTouchInput();   
	void InitButtons();

	gef::Font* font_;
	gef::InputManager* input_manager_;
	gef::SpriteRenderer* sprite_renderer_;

	gef::Texture* background_texture_;
	gef::Sprite background_sprite_;
	gef::Matrix44 ortho_matrix_;

	// pointers to the gamestates the menu can transition to.
	GameState* level_;

	std::vector<Button*> buttons;
	const int num_of_buttons = 3;

	// Touch Input
	Int32 active_touch_id;
	gef::Vector2 touch_position;

	float fps_;
};

