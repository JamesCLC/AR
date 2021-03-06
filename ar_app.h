#ifndef _RENDER_TARGET_APP_H
#define _RENDER_TARGET_APP_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include <platform/vita/graphics/texture_vita.h>
#include "primitive_builder.h"


// Vita AR includes
#include <camera.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>

// James' Files
#include "game_object.h"
#include "GameStates\game_state.h" 
#include "GameStates\Level.h"
#include "GameStates\main_menu.h"
#include "GameStates\game_over.h"
#include "GameStates\Victory.h"

// FRAMEWORK FORWARD DECLARATIONS

namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class RenderTarget;
	class TextureVita;
	class InputManager;
}



class ARApp : public gef::Application
{
public:
	ARApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	
	// Game States
	GameState* current_state_;
	MainMenu* main_menu_;
	Level* level_;
	GameOver* game_over_;
	Victory* victory_;
};
#endif // _RENDER_TARGET_APP_H