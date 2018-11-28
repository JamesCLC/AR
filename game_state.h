#pragma once


class gef::Platform;

class GameState
{
public:
	GameState(gef::Platform& platform) : platform_(platform) {};
	virtual ~GameState();

	virtual void Init() = 0;
	virtual bool Update() = 0;
	virtual void Render() = 0;
	virtual void CleanUp() = 0;

protected:
	// Variables shared across game states?
	class gef::Platform& platform_;
};

