#ifndef _SNK_H_
#define _SNK_H_
#include <graphics.h>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>

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

class Section
{
public:
	enum State { EActive, EDead };
	State mstate;
	Vector2 SecPos;
	Speed SecSpd;

	Section(int x, int y, State state);
};

class Game
{
private:
	void ProcessInput();
	void UpdatingGame();
	void GenerateOutput();
	void Refresh(DWORD* tem);
	bool mIsRunning;
	bool mFireWorks;
	clock_t mTicks;

	std::vector<Section*> msections;
	POINT mpoint;

public:
	Game();
	void Initialize();
	void RunLoop();
	void Shutdown();
};
#endif