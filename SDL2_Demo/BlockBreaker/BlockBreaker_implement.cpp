#include "BlockBreaker_h.h"
#define WINDOW_WID 800
#define WINDOW_HEI 600
#define HALF_BLOCK_WID 20
#define HALF_BLOCK_HEI 10
#define BLOCK_NUM 10
#define BLOCK_LINE 2
#define HALF_BALL_WID 5
#define BALL_NUM 3

//Blocks methods
Blocks::Blocks(int x, int y)
{
	mstate = EActive;
	mBlockPos.x = x;
	mBlockPos.y = y;
}
Blocks::~Blocks()
{

}

//Balls methods
Balls::Balls()
{
	mstate = EActive;
	mBallPos.x = static_cast<int>(WINDOW_WID / 2.0f);
	mBallPos.y = static_cast<int>(WINDOW_HEI / 2.0f);
	mBallSpeed.x = 50 + rand() % 50;
	mBallSpeed.y = 50 + rand() % 50;
}
Balls::~Balls()
{

}

//Game methods
Game::Game()
{
	mGoals = 0;
	mTicks = 0;
	mIsRunning = true;
	mBlocksNum = BLOCK_NUM;
	mBallsNum = BALL_NUM;
	mPaddlePos.x = static_cast<int>(WINDOW_WID / 2.0f);
	mPaddlePos.y = WINDOW_HEI - HALF_BLOCK_HEI * 2;
	mPaddleDir = 0;
}
void Game::Initialize()
{
	initgraph(WINDOW_WID, WINDOW_HEI);

	int widperblock = WINDOW_WID / BLOCK_NUM;
	for (int i = 1; i <= BLOCK_LINE; ++i)
	{
		for (int j = 1; j < BLOCK_NUM; ++j)
		{
			Blocks* temp = new Blocks(j * widperblock, 3 * i * HALF_BLOCK_HEI);
			mBlocks.push_back(temp);
		}
	}
	for (int i = 0; i < BALL_NUM; ++i)
	{
		Balls* temp = new Balls;
		mBalls.push_back(temp);
	}
	srand(time(0));
}
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdatingGame();
		GenerateOutput();
	}
}
void Game::Shutdown()
{
	closegraph();
	while (!mBlocks.empty())
		mBlocks.pop_back();
	while (!mBalls.empty())
		mBalls.pop_back();
}
void Game::ProcessInput()
{
	if (GetAsyncKeyState(VK_ESCAPE))
		mIsRunning = false;
	if (GetAsyncKeyState(VK_LEFT) && mPaddlePos.x - 4 * HALF_BLOCK_WID > 0)
		mPaddleDir = -1;
	if (GetAsyncKeyState(VK_RIGHT) && mPaddlePos.x + 4 * HALF_BLOCK_WID < WINDOW_WID)
		mPaddleDir = 1;
}
void Game::UpdatingGame()
{
	float deltaTime = (clock() - mTicks) / CLOCKS_PER_SEC;
	//while (clock() - mTicks < 16.6); // Lock at 60 fps
	Sleep(16);
	mTicks = clock();

	if (deltaTime < 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Delete dead actors
	for (int i = 0; i < mBlocks.size(); ++i)
	{
		if (mBlocks[i]->mstate == Blocks::EDead)
		{
			mBlocks.erase(mBlocks.begin() + i);
			--i;
		}
	}
	for (int i = 0; i < mBalls.size(); ++i)
	{
		if (mBalls[i]->mstate == Balls::EDead)
		{
			mBalls.erase(mBalls.begin() + i);
			--i;
		}
	}

	// Moving paddle
	mPaddlePos.x += 200 * deltaTime * mPaddleDir;
	mPaddleDir = 0; // Don't move until next time press
	// Moving Balls
	for (int i = 0; i < mBalls.size(); ++i)
	{
		mBalls[i]->mBallPos.x += mBalls[i]->mBallSpeed.x * deltaTime;
		mBalls[i]->mBallPos.y += mBalls[i]->mBallSpeed.y * deltaTime;
	}

	// Detecting Block-collision
	for (int i = 0; i < mBalls.size(); ++i)
	{ // 遍历检测每一个球的撞击情况
		for (int j = 0; j < mBlocks.size(); ++j)
		{
			int temp_x = mBalls[i]->mBallPos.x - mBlocks[j]->mBlockPos.x;
			int temp_y = mBalls[i]->mBallPos.y - mBlocks[j]->mBlockPos.y;
			// Start changing direction
			if (temp_x >= HALF_BLOCK_WID && temp_x <= HALF_BLOCK_WID + 2)
			{
				if (temp_y < HALF_BLOCK_HEI && temp_y > -HALF_BLOCK_HEI)
				{
					if (mBalls[i]->mBallSpeed.x < 0)
					{
						mBalls[i]->mBallSpeed.x *= -1;
						mBlocks[j]->mstate = Blocks::EDead;
						mGoals += 10;
						break;
					}
				}
			}
			if (temp_x <= -HALF_BLOCK_WID && temp_x >= -HALF_BLOCK_WID - 2)
			{
				if (temp_y < HALF_BLOCK_HEI && temp_y > -HALF_BLOCK_HEI)
				{
					if (mBalls[i]->mBallSpeed.x > 0)
					{
						mBalls[i]->mBallSpeed.x *= -1;
						mBlocks[j]->mstate = Blocks::EDead;
						mGoals += 10;
						break;
					}
				}
			}
			if (temp_y >= HALF_BLOCK_HEI && temp_y <= HALF_BLOCK_HEI + 2)
			{
				if (temp_x < HALF_BLOCK_WID && temp_x > -HALF_BLOCK_WID)
				{
					if (mBalls[i]->mBallSpeed.y < 0) // 正在向上移动
					{
						mBalls[i]->mBallSpeed.y *= -1;
						mBlocks[j]->mstate = Blocks::EDead;
						mGoals += 10;
						break;
					}
				}
			}
			if (temp_y <= -HALF_BLOCK_HEI && temp_y >= -HALF_BLOCK_HEI - 2)
			{
				if (temp_x < HALF_BLOCK_WID && temp_x > -HALF_BLOCK_WID)
				{
					if (mBalls[i]->mBallSpeed.y > 0) // 正在向下移动
					{
						mBalls[i]->mBallSpeed.y *= -1;
						mBlocks[j]->mstate = Blocks::EDead;
						mGoals += 10;
						break;
					}
				}
			}
		}
	}
	// Detecting Paddle-collision
	for (int i = 0; i < mBalls.size(); ++i)
	{ // 检测有无撞击到挡板
		int temp_x = mBalls[i]->mBallPos.x - mPaddlePos.x;
		int temp_y = mBalls[i]->mBallPos.y - mPaddlePos.y;
		if (temp_x < HALF_BLOCK_WID * 4 && temp_x > -HALF_BLOCK_WID * 4)
		{
			if (temp_y > -HALF_BLOCK_HEI - 2 && mBalls[i]->mBallSpeed.y > 0)
				mBalls[i]->mBallSpeed.y *= -1;
			//else if (temp_y < HALF_BLOCK_HEI + 2 && mBalls[i]->mBallSpeed.y < 0)
				//mBalls[i]->mBallSpeed.y *= -1;
		}
	}
	// Detecting Wall-collision
	for (int i = 0; i < mBalls.size(); ++i)
	{
		if (mBalls[i]->mBallPos.x < 0 && mBalls[i]->mBallSpeed.x < 0)
			mBalls[i]->mBallSpeed.x *= -1;
		else if (mBalls[i]->mBallPos.x > WINDOW_WID && mBalls[i]->mBallSpeed.x > 0)
			mBalls[i]->mBallSpeed.x *= -1;
		if (mBalls[i]->mBallPos.y < 0 && mBalls[i]->mBallSpeed.y < 0)
			mBalls[i]->mBallSpeed.y *= -1;
		else if (mBalls[i]->mBallPos.y > WINDOW_HEI && mBalls[i]->mBallSpeed.y > 0)
			mBalls[i]->mstate = Balls::EDead;
	}
}
void Game::GenerateOutput()
{
	BeginBatchDraw();
	// Clear last frame
	cleardevice();
	// Draw Blocks
	setlinecolor(YELLOW);
	setfillcolor(GREEN);
	for (int i = 0; i < mBlocks.size(); ++i)
	{
		rectangle(mBlocks[i]->mBlockPos.x - HALF_BLOCK_WID,mBlocks[i]->mBlockPos.y - HALF_BLOCK_HEI,mBlocks[i]->mBlockPos.x + HALF_BLOCK_WID,mBlocks[i]->mBlockPos.y + HALF_BLOCK_HEI);
		floodfill(mBlocks[i]->mBlockPos.x, mBlocks[i]->mBlockPos.y, YELLOW);
	}
	// Draw Balls
	setlinecolor(GREEN);
	setfillcolor(YELLOW);
	for (int i = 0; i < mBalls.size(); ++i)
	{
		rectangle(mBalls[i]->mBallPos.x - HALF_BALL_WID,mBalls[i]->mBallPos.y - HALF_BALL_WID,mBalls[i]->mBallPos.x + HALF_BALL_WID,mBalls[i]->mBallPos.y + HALF_BALL_WID);
		floodfill(mBalls[i]->mBallPos.x, mBalls[i]->mBallPos.y, GREEN);
	}
	// Draw Paddle
	setlinecolor(RED);
	setfillcolor(WHITE);
	{
		rectangle(mPaddlePos.x - 4 * HALF_BLOCK_WID, mPaddlePos.y - HALF_BLOCK_HEI, mPaddlePos.x + 4 * HALF_BLOCK_WID, mPaddlePos.y + HALF_BLOCK_HEI);
		floodfill(mPaddlePos.x, mPaddlePos.y, RED);
	}
	// Print the scores
	const char s[] = "Your scores: ";
	settextstyle(16, 0, _T("The song"));
	outtextxy(WINDOW_WID / 2 - 50, WINDOW_HEI / 2, _T("BLOCKBREAKER!"));
	FlushBatchDraw();
}