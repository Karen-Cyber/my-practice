#include <graphics.h>
#include <vector>
#include <random>
#include <ctime>
using namespace std;
// Global sets
const int WINDOW_WID = 648;
const int WINDOW_HEI = 480;
const int ANIMAT_TME = 12;
const int TREMBL_FAC = 10;
const int MAXBBL_RAD = 35;
const int MAXBBL_SPD = 25;
const int RELOAD_TME = 4;
const int RELOAD_SPD = 2;

// Data
struct Bubble
{
	float radius;
	COLORREF color;
	float posx;
	float posy;
	float spdx;
	float spdy;
	bool ACTIVE;
	Bubble(float rd, float px, float py, float sx, float sy, bool A = true, COLORREF col = WHITE)
	{
		radius = rd;
		posx = px;
		posy = py;
		spdx = sx;
		spdy = sy;
		ACTIVE = A;
		color = col;
	}
};
vector<Bubble*> bubbles;
bool IsRunning;
bool trigger;
int CDtime;
float mouse_x;
float mouse_y;

// Main frame
void Initialize();
void ProcessInput();
void Updating();
void GenerateOutput();
void Delay(DWORD ms);
void RunLoop();
void ShutDown();

// Other functions:
int Abs(int n);
float Abs(float n);

int main()
{
	Initialize();
	RunLoop();
	ShutDown();
	return 0;
}

void Initialize()
{
	initgraph(WINDOW_WID, WINDOW_HEI);
	BeginBatchDraw();
	IsRunning = true;
	trigger = false;
	mouse_x = 0.0f;
	mouse_y = 0.0f;
	CDtime = RELOAD_TME;
	srand(unsigned int(time(NULL)));
}

void ProcessInput()
{
	MOUSEMSG m;
	while (MouseHit())
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_MOUSEMOVE:
			mouse_x = m.x; 
			mouse_y = m.y; 
			break;
		case WM_LBUTTONDOWN:
			trigger = true;    
			break;
		case WM_LBUTTONUP:  
			trigger = false;   
			break;
		case WM_RBUTTONDOWN:
			IsRunning = false;
		}
	}
}

void Updating()
{
	// Add new bubble
	if (CDtime <= 0)
	{
		float sx = float((rand() % MAXBBL_SPD) * (rand() % 2 ? 1 : -1));
		float sy = float((rand() % MAXBBL_SPD / 2.0f) * (rand() % 2 ? 1 : -1));
		float rd = float(rand() % MAXBBL_RAD + 10);
		COLORREF col = RGB(rand() % 256, rand() % 256, rand() % 256);
		Bubble* temp = new Bubble(rd, mouse_x, mouse_y, sx, sy, true, col);
		bubbles.push_back(temp);
		CDtime = RELOAD_TME;
	}
	if (CDtime > 0)
		CDtime -= RELOAD_SPD;

	// Change bubbles speed and radius
	int len = bubbles.size();
	for (int i = 0; i < len; ++i)
	{
		if (Abs(bubbles[i]->spdx) < float(MAXBBL_SPD))
		{
			bubbles[i]->spdx -= bubbles[i]->spdx / 4.0f;
		}
		if (bubbles[i]->spdy > (-1.0f * MAXBBL_SPD) / 2.0f)
		{
			bubbles[i]->spdy += -0.5f;
		}

		bubbles[i]->radius -= bubbles[i]->radius / 10.0f;

		bubbles[i]->posx += bubbles[i]->spdx;
		bubbles[i]->posy += bubbles[i]->spdy;
	}

	// Mark the overboundary bubbles
	for (int i = 0; i < len; ++i)
	{
		if (bubbles[i]->posx < -10.0f || bubbles[i]->posx > float(WINDOW_WID + 10))
		{
			bubbles[i]->ACTIVE = false;
		}
		else if (bubbles[i]->posy < -40.0f || bubbles[i]->posy > float(WINDOW_HEI + 40))
		{
			bubbles[i]->ACTIVE = false;
		}
		else
		{

		}
	}
	// Delete the overboundary bubbles
	for (auto iter = bubbles.begin(); iter != bubbles.end();)
	{
		if (!(*iter)->ACTIVE)
		{
			delete (*iter);
			iter = bubbles.erase(iter);
			continue;
		}
		++iter;
	}
}

void GenerateOutput()
{
	cleardevice();
	if (bubbles.size())
	{
		int len = bubbles.size();
		for (int i = 0; i < len; ++i)
		{
			setfillcolor(bubbles[i]->color);
			fillcircle(bubbles[i]->posx, bubbles[i]->posy, bubbles[i]->radius);
			setlinecolor(bubbles[i]->color);
			setlinestyle(PS_SOLID, 3);
			circle(bubbles[i]->posx, bubbles[i]->posy, bubbles[i]->radius + 4);
		}
		FlushBatchDraw();
	}
}

void Delay(DWORD ms)
{
	static DWORD oldtime = GetTickCount();

	while (GetTickCount() - oldtime < ms)
		Sleep(1);

	oldtime = GetTickCount();
}

void RunLoop()
{
	while (IsRunning)
	{
		ProcessInput();
		Updating();
		GenerateOutput();
		Delay(ANIMAT_TME);
	}
}

void ShutDown()
{
	closegraph();
	EndBatchDraw();
	bubbles.clear();
}

// Other functions:
int Abs(int n)
{
	if (n < 0)
		return -n;
	return n;
}
float Abs(float n)
{
	if (n < 0.0f)
		return -n;
	return n;
}