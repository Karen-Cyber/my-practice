#ifndef _GAME_H_
#define _GAME_H_
#include <SDL.h>

struct vector2
{
	float x;
	float y;
};

class GAME
{
private:
	void ProcessInput();
	void UpdateGame();
	void Generateoutput();
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;

	static const int mWidth = 800;
	static const int mHeight = 600;
	static const int mthickness = 15;

	vector2 mPaddlePos;
	vector2 mBallPos;
	vector2 mBallVel;

	Uint32 mTicksCount;

	int mPaddleDir;
	int paddleH;

public:
	GAME();
	bool Initialize();
	void RunLoop();
	void ShutDown();
};
#endif