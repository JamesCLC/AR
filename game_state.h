#pragma once

#include <platform/vita/system/platform_vita.h>
//class gef::Platform;

class GameState
{
public:
	GameState(gef::Platform& platform_) /*: platform(platform_) {}*/;
	virtual ~GameState();

	virtual void Init() = 0;
	virtual bool Update(float frame_time) = 0;
	virtual void Render() = 0;
	virtual void CleanUp() = 0;

protected:
	// Variables shared across game states?
	class gef::Platform& platform;
};

