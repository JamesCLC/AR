#ifndef _RENDER_TARGET_APP_H
#define _RENDER_TARGET_APP_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include <platform/vita/graphics/texture_vita.h>
#include "primitive_builder.h"
#include "game_object.h"

// Vita AR includes
#include <camera.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>


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
	void InitFont();
	void CleanUpFont();
	void DrawHUD();

	void RenderOverlay();
	void SetupLights();

	gef::InputManager* input_manager_;

	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;

	float fps_;

	class gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;


	///	James' Stuff
	// Orthographic Projection Matrix for Rendering the viewport.
	gef::Matrix44 ortho_matrix_;

	// Scaling factor to prevent camera image distortion.
	float scaling_factor_ = 0.0f;

	gef::Sprite camera_feed_sprite_;
	gef::TextureVita camera_feed_texture_;

	// 3D Elements
	gef::Matrix44 unscaled_projection_matrix_;
	gef::Matrix44 scaling_matrix_;
	gef::Matrix44 scaled_projection_matrix_;
	gef::Matrix44 identity_;
	gef::MeshInstance box_mesh_;
	int marker_id = 1;
	gef::Matrix44 box_scale_matrix;

	
	GameObject* test_;
	///

};
#endif // _RENDER_TARGET_APP_H