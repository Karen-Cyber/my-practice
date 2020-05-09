#include "SDL2_EasyX_header.h"

//Private methods:
void GAME::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) // While there are still events in the queue
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
		mIsRunning = false;

	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])    // Use sum and sub instead of assignment
		mPaddleDir -= 1;          // This guarantee that Dir is 0 if w and s are pressed at the same time
	if (state[SDL_SCANCODE_S])
		mPaddleDir +=1;

	mPaddleDir2 = 0;
	if (state[SDL_SCANCODE_W])    // Use sum and sub instead of assignment
		mPaddleDir2 -= 1;          // This guarantee that Dir is 0 if w and s are pressed at the same time
	if (state[SDL_SCANCODE_S])
		mPaddleDir2 += 1;
}
//这段代码可以意为：event对象包含了事件的类型，比如SDL_QUIT指的是右上角的X关闭键
//Uint8 是虚拟键盘类型, SDL_SCANCODE_ESCAPE指的是按下ESC键

void GAME::UpdateGame()
{
	// Delta time is the difference in ticks from last frame
	// (Converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
		deltaTime = 0.05f;
	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks(); // This function return ms unit
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
	// This function check if the formmer ticks is bigger than the latter one

	//Updating paddle and ball
	if (mPaddleDir)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime; // Move at the speed of 300.0 pixels per second
		// Make sure Paddle doesn't move off screen!
		if (mPaddlePos.y < paddleH / 2.0f + mthickness)
			mPaddlePos.y = paddleH / 2.0f + mthickness;
		else if (mPaddlePos.y > 600.0f - paddleH / 2.0f - mthickness)
			mPaddlePos.y = 600.0f - paddleH / 2.0f - mthickness;
	}

	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;
	if (mBallPos.y <= mthickness && mBallVel.y < 0.0f)
		mBallVel.y *= -1;
	if (mBallPos.y >= mHeight - mthickness && mBallVel.y > 0.0f)
		mBallVel.y *= -1;
	if (mBallPos.x >= mWidth - mthickness && mBallVel.x > 0.0f)
		mBallVel.x *= -1;
	if (mBallPos.x <= mPaddlePos.x && mBallVel.x < 0.0f)
	{
		int diff = mBallPos.y - mPaddlePos.y;
		if (diff < 0) diff *= -1;
		if (diff <= paddleH / 2.0f)
			mBallVel.x *= -1;
	}
	if (mBallPos.x <= 0.0f)
		mIsRunning = false;

}

void GAME::Generateoutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 150, 150, 255); //渲染器, Red, Green, Blue, Alpha(透明度, 255即不透明)
	SDL_RenderClear(mRenderer);                          //将后台缓冲区设置为当前屏幕读取的缓冲区，输出颜色(即将整个屏幕输出为这个颜色)
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_Rect wall{0, 0, mWidth, mthickness};
	SDL_RenderFillRect(mRenderer, &wall);
	wall.y = mHeight - mthickness;
	SDL_RenderFillRect(mRenderer, &wall);
	wall.x = mWidth - mthickness; wall.y = 0; wall.w = mthickness; wall.h = mHeight;
	SDL_RenderFillRect(mRenderer, &wall);

	SDL_Rect Paddle
	{
		static_cast<int>(mPaddlePos.x - mthickness),
		static_cast<int>(mPaddlePos.y - 3 * mthickness),
		mthickness,
		paddleH
	};
	SDL_RenderFillRect(mRenderer, &Paddle);
	SDL_Rect Ball
	{
		mBallPos.x - mthickness,
		mBallPos.y - mthickness,
		mthickness,
		mthickness
	};
	SDL_RenderFillRect(mRenderer, &Ball);

	SDL_RenderPresent(mRenderer); //在缓冲区后台绘制颜色，显示器会马上交换用于输出的缓冲区(实际上显示器和显卡一直在交换缓冲区)
}

//Public methods:
GAME::GAME()
{
	mWindow = SDL_CreateWindow("Game Programming in C++", 100, 100, 1024, 768, 0);
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	mIsRunning = true;
	mTicksCount = 0;
}

bool GAME::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: $", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game Programming in C++", 100, 100, 800, 600, 0); // 100, 100是新建窗口时窗口左上角的屏幕坐标
	if (!mWindow)
	{
		SDL_Log("Fail to create window; %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//-1代表在唯一的窗口中创建属于这个窗口的渲染器，使用加速渲染器(accelerated | presentvsync)与垂直同步

	mPaddlePos.x = mthickness;
	mPaddlePos.y = static_cast<int>(mHeight / 2);
	mPaddlePos2.x = mWidth - mthickness;
	mPaddlePos2.y = static_cast<int>(mHeight / 2);
	mBallPos.x = static_cast<int>(mWidth / 2);
	mBallPos.y = static_cast<int>(mHeight / 2);
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	mPaddleDir = 0;
	paddleH = mthickness * 6;
	return true;
}

void GAME::ShutDown()
{
	SDL_DestroyWindow(mWindow); //关闭窗口
	SDL_DestroyRenderer(mRenderer); //关闭渲染器
	SDL_Quit();
}

void GAME::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		Generateoutput();
	}
}