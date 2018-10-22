#pragma once
#include "game_object.h"
#include "graphics\scene.h"
#include "animation\animation.h"
#include "animation\skeleton.h"
#include "motion_clip_player.h"


class AnimatedCharacter : public GameObject
{

public:
	AnimatedCharacter();
	~AnimatedCharacter();


protected:
	MotionClipPlayer motion_clip_player;

};

