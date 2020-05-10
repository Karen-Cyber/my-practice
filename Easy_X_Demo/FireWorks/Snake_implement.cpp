#include "Snake_h.h"
#define WINDOW_WID 500
#define WINDOW_HEI 500
#define DISTANCE 25
#define RADIUS 5
#define SEC_NUM 5
#define SEC_SPD 250
#define FPS_SEC 16.6
// Color of fireworks
unsigned char R, G, B;

// Section methods
Section::Section(int x, int y, State state)
{
	SecPos.x = x;
	SecPos.y = y;
	SecSpd.x = 0.01f;
	SecSpd.y = 0.01f;
	mstate = EActive;
}

Game::Game()
{

}
void Game::Initialize()
{
	initgraph(WINDOW_WID, WINDOW_HEI);
	mIsRunning = true;
	mFireWorks = false;
	mTicks = 0.0f;
	for (int i = 1; i <= SEC_NUM; ++i)
	{
		Section* temp = new Section(WINDOW_WID / 2, WINDOW_HEI / 2, Section::EDead);
		msections.push_back(temp);
	}
	std::srand(std::time(0));
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
	while (!msections.empty())
		msections.pop_back();
}
void Game::ProcessInput()
{
	GetCursorPos(&mpoint);
	if (GetAsyncKeyState(VK_LBUTTON))
		mFireWorks = true;
}
void Game::UpdatingGame()
{
	Sleep(FPS_SEC);
	float deltaTime = FPS_SEC / 1000.0f;
	
	// Launch the fire and Set the explosion point
	if (mFireWorks)
	{
		for (int i = 0; i < msections.size(); ++i)
		{
			msections[i]->mstate = Section::EActive;
		}
		for (int i = 0; i < msections.size(); ++i)
		{
			msections[i]->SecPos.x = mpoint.x;
			msections[i]->SecPos.y = mpoint.y;
		}
		R = rand() % 255;
		G = rand() % 255;
		B = rand() % 255;
	}

	// Set the random speed of fireworks
	if (mFireWorks)
	{
		for (int i = 0; i < msections.size(); ++i)
		{
			msections[i]->SecSpd.x = rand() % SEC_SPD * (rand() % 2 == 0 ? 1 : -1);
			msections[i]->SecSpd.y = rand() % SEC_SPD * (rand() % 2 == 0 ? -1 : 1);
		}
		mFireWorks = false;
	}

	// Move the fireworks
	for (int i = 0; i < msections.size(); ++i)
	{
		msections[i]->SecPos.x += msections[i]->SecSpd.x * deltaTime;
		msections[i]->SecPos.y += msections[i]->SecSpd.y * deltaTime;
	}

	// Slow the speed of fireworks
	for (int i = 0; i < msections.size(); ++i)
	{
		msections[i]->SecSpd.x -= msections[i]->SecSpd.x * deltaTime;
		msections[i]->SecSpd.y -= msections[i]->SecSpd.y * deltaTime;
	}

	// Put out the fire that are too slow
	for (int i = 0; i < msections.size(); ++i)
	{
		if (pow(msections[i]->SecSpd.x, 2) + pow(msections[i]->SecSpd.y, 2) < 40)
			msections[i]->mstate = Section::EDead;
	}
	/*
	// Allow to fire again?
	for (int i = 0; i < msections.size(); ++i)
	{
		if (msections[i]->mstate == Section::EActive)
			break;
		if (i == msections.size() - 1)
			mFireWorks = true;
	}*/
}
void Game::GenerateOutput()
{
	DWORD* temp = GetImageBuffer();
	// Clear last frap
	Refresh(temp);
	// Draw Sections
	BeginBatchDraw();
	setlinecolor(RGB(R, G, B));
	setfillcolor(RGB(R, G, B));
	for (int i = 0; i < msections.size(); ++i)
	{
		if (msections[i]->mstate == Section::EActive)
		{
			circle(msections[i]->SecPos.x, msections[i]->SecPos.y, RADIUS);
			floodfill(msections[i]->SecPos.x, msections[i]->SecPos.y, RGB(R, G, B));
		}
	}
	FlushBatchDraw();
}
void Game::Refresh(DWORD* tem)
{
	unsigned char R, G, B;
	for (int i = WINDOW_WID; i < WINDOW_WID * (WINDOW_HEI - 1); i++)
	{
		R = (GetRValue(tem[i]) +
			GetRValue(tem[i - 500]) +
			GetRValue(tem[i + 500]) +
			GetRValue(tem[i + 1]) +
			GetRValue(tem[i - 1])) / 5;


		G = (GetGValue(tem[i]) +
			GetGValue(tem[i - 500]) +
			GetGValue(tem[i + 500]) +
			GetGValue(tem[i - 1]) +
			GetGValue(tem[i + 1])) / 5;


		B = (GetBValue(tem[i]) +
			GetBValue(tem[i + 500]) +
			GetBValue(tem[i - 500]) +
			GetBValue(tem[i + 1]) +
			GetBValue(tem[i - 1])) / 5;
		tem[i] = RGB(R, G, B);
	}
}