#pragma once
#include "m:\AppliedGamesTechnology\ar_app\game_state.h"

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

#include "m:\AppliedGamesTechnology\ar_app\game_object.h"

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
	bool ProcessTouchInput();

	// Move to collision detection, then replace with a single call.
	void GetRay(gef::Vector4&, gef::Vector4&, gef::Matrix44&, gef::Matrix44&);
	bool RayToSphere(GameObject&, gef::Vector4&, gef::Vector4&);
	bool PointInSphere(GameObject&, gef::Vector4&);
	void MoveGameObject(GameObject&, gef::Matrix44&, gef::Matrix44&);
	bool GameObjectFall(GameObject&, gef::Matrix44&);

private:
	// Graphics Objects
	class gef::Renderer3D* renderer_3d_;
	gef::Font* font_;
	gef::InputManager* input_manager_;
	gef::SpriteRenderer* sprite_renderer_;

	gef::Matrix44 unscaled_projection_matrix_;
	gef::Matrix44 scaled_projection_matrix_;
	gef::Matrix44 scaling_matrix_;
	gef::Matrix44 ortho_matrix_;
	gef::Matrix44 identity_;
	gef::Sprite camera_feed_sprite_;
	gef::TextureVita camera_feed_texture_;
	float scaling_factor_ = 0.0f;
	float fps_;
	int marker_id = 1;

	// Touch Input - To Be Moved
	Int32 active_touch_id;
	gef::Vector2 touch_position;

	// Raytracing - To Be Moved
	gef::Vector4 ray_start;
	gef::Vector4 ray_direction;

	GameObject* test_;
	gef::Matrix44 game_object_scale_matrix;
};

