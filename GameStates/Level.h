#pragma once

#include <platform\vita\graphics\texture_vita.h>
#include <graphics\sprite.h>
#include <graphics\sprite_renderer.h>
#include <graphics\font.h>
#include <graphics\renderer_3d.h>
#include <maths\vector2.h>
#include <input\input_manager.h>
#include <input\touch_input_manager.h>

#include <sony_sample_framework.h>
#include <sony_tracking.h>

// Vita AR includes
#include <camera.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>

#include "m:\AppliedGamesTechnology\ar_app\game_state.h"
#include "m:\AppliedGamesTechnology\ar_app\game_object.h"
#include "m:\AppliedGamesTechnology\ar_app\game_manager.h"

class Level : public GameState
{
public:
	Level(gef::Platform& platform);
	~Level();

	void Init();
	bool Update(float frame_time);
	void Render();
	void CleanUp();

private:
	void RenderOverlay();
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetUpLights();

private:
	// Graphics Objects
	class gef::Renderer3D* renderer_3d_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;

	gef::Matrix44 unscaled_projection_matrix_;
	gef::Matrix44 scaled_projection_matrix_;
	gef::Matrix44 scaling_matrix_;
	gef::Matrix44 ortho_matrix_;
	gef::Matrix44 view_matrix;
	gef::Sprite camera_feed_sprite_;
	gef::TextureVita camera_feed_texture_;
	float scaling_factor_ = 0.0f;
	float fps_;
	int marker_id = 1;

	gef::Matrix44 marker_transform_;

	GameManager* game_manager_;

	// bool is_marker_visible;
};

