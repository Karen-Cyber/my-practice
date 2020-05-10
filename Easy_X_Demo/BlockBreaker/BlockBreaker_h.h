#ifndef _BB_H_
#define _BB_H_
#include <graphics.h>
#include <Windows.h>
#include <vector>
#include <random>
#include <ctime>

struct Vector2
{
	float x;
	float y;
};
struct Speed
{
	float x;
	float y;
};

class Blocks
{
public:
	enum State { EActive, EDead };
	//private:
	Vector2 mBlockPos;
	State mstate;

public:
	Blocks(int x, int y);
	~Blocks();

	State GetState() { return mstate; }
};

class Balls
{
public:
	enum State { EActive, EDead };
	//private:
	Vector2 mBallPos;
	Vector2 mBallSpeed;
	State mstate;

public:
	Balls();
	~Balls();

	State GetState() { return mstate; }
};

class Game
{
private:
	void ProcessInput();
	void UpdatingGame();
	void GenerateOutput();
	bool mIsRunning;
	bool mBallMode;

	int mTicks;
	int mGoals;
	int mBlocksNum;
	int mBallsNum;

	std::vector<Blocks*> mBlocks;
	std::vector<Balls*> mBalls;

	Vector2 mPaddlePos;
	int mPaddleDir;

	IMAGE BKG;

public:
	Game();
	void Initialize();
	void RunLoop();
	void Shutdown();
	int GetScores() { return mGoals; }
};
#endif