#include <graphics.h>
#include <random>
#include <cmath>
#include <ctime>
#define WINDOW_WID 1024
#define WINDOW_HEI 576
#define PARTIC_NUM 300
#define DELAY_TIME 15
#define FRICTION 0.95

struct Particle
{
	float posx;
	float posy;
	float spdx;
	float spdy;
	float radius;
	COLORREF color;
};

// 定义全局共享变量
Particle particles[PARTIC_NUM]; // 粒子总数
MOUSEMSG m;                     // 用于获取鼠标信息
DWORD* pBuffer;                 // 获取当前窗口显存指针
DWORD Ticks;                    // 每帧的刷新时间
bool trigger;                   // 冲击触发

int mouse_posx, mouse_posy; // 鼠标当前坐标
int mouse_prex, mouse_prey; // 鼠标上帧坐标
int mouse_spdx, mouse_spdy; // 鼠标当前速度

void Initialize();
void ProcessInput();
void Updating();
void GenerateOutput();
void DarkenHalf();
void DelayTime(DWORD ms);
void ShutDown();


int main()
{
	Initialize();
	while (true)
	{
		ProcessInput();
		Updating();
		GenerateOutput();
	}
	ShutDown();
	return 0;
}

void Initialize()
{
	srand(unsigned int(time(NULL)));
	initgraph(WINDOW_WID, WINDOW_HEI);

	for (int i = 0; i < PARTIC_NUM; ++i)
	{
		particles[i].posx = WINDOW_WID / 2.0f;
		particles[i].posy = WINDOW_HEI / 2.0f;
		particles[i].spdx = float(cos(float(i))) * (rand() % 40);
		particles[i].spdy = float(sin(float(i))) * (rand() % 40);
		particles[i].radius = 3.5f;
		particles[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}

	mouse_posx = mouse_prex = WINDOW_WID / 2.0f;
	mouse_posy = mouse_prey = WINDOW_HEI / 2.0f;
	mouse_spdx = mouse_spdy = 0.0f;

	trigger = false;
	
	pBuffer = GetImageBuffer(NULL);
	BeginBatchDraw(); // 启用双缓存显示
}

void ProcessInput()
{
	while (MouseHit())
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_MOUSEMOVE:
			mouse_posx = m.x;
			mouse_posy = m.y;
			break;

		case WM_LBUTTONDOWN:
			trigger = true;
			break;

		case WM_LBUTTONUP:
			trigger = false;
			break;
		}
	}
}

void Updating()
{
	DelayTime(DELAY_TIME);

	float BlowRange = WINDOW_WID * 0.25f;   // 冲击范围
	float DrainRange = WINDOW_WID * 0.125f;  // 牵引范围
	float PullRange = WINDOW_WID * 0.25;    // 吸引范围

	mouse_spdx = mouse_posx - mouse_prex; // 计算鼠标x速度
	mouse_spdy = mouse_posy - mouse_prey; // 计算鼠标y速度
	mouse_prex = mouse_posx; // 更新鼠标旧坐标
	mouse_prey = mouse_posy; // 更新鼠标旧坐标

	for (int i = 0; i < PARTIC_NUM; ++i)
	{
		float distance_x = particles[i].posx - mouse_posx;                             // 水平距离差
		float distance_y = particles[i].posy - mouse_posy;                             // 垂直距离差
		float distance_s = float(sqrt(distance_x*distance_x + distance_y*distance_y)); // 距离勾股和

		// 计算三角函数，便于冲击力度附加在粒子速度的计算
		distance_x = distance_s ? (distance_x / distance_s) : 0.0f; // cos
		distance_y = distance_s ? (distance_y / distance_s) : 0.0f; // sin

		if (trigger && distance_s < BlowRange) // 冲击触发且该粒子在冲击范围内
		{
			float intensity = (1 - (distance_s / BlowRange)) * 14; // 冲击强度
			particles[i].spdx += distance_x * intensity + 0.5f - float(rand()) / RAND_MAX;
			particles[i].spdy += distance_y * intensity + 0.5f - float(rand()) / RAND_MAX;
		}

		if (distance_s < PullRange) // 该粒子在吸引范围内
		{
			float intensity = (1 - (distance_s / PullRange)) * 1.5f; // 吸引强度
			particles[i].spdx -= distance_x * intensity;
			particles[i].spdy -= distance_y * intensity;
		}

		if (distance_s < DrainRange) // 该粒子在牵引范围内
		{
			float intensity = (1 - (distance_s / DrainRange)) * 0.5f; // 牵引强度
			particles[i].spdx += mouse_spdx * intensity;
			particles[i].spdy += mouse_spdy * intensity;
		}

		// 合并阻力影响
		particles[i].spdx *= FRICTION;
		particles[i].spdy *= FRICTION;

		// 计算该速度下粒子半径
		particles[i].radius = (fabs(particles[i].spdx) + fabs(particles[i].spdy)) * 0.50f;
		particles[i].radius = max(min(particles[i].radius, 3.5), 1.0f); // 粒子半径不小于1像素，不大于3.5像素

		// 更新粒子坐标
		particles[i].posx += particles[i].spdx;
		particles[i].posy += particles[i].spdy;

		// 粒子不允许超出边界
		if (particles[i].posx >= WINDOW_WID)
		{
			particles[i].posx = WINDOW_WID;
			particles[i].spdx *= -1;
		}
		else if (particles[i].posx <=0)
		{
			particles[i].posx = 0;
			particles[i].spdx *= -1;
		}
		if (particles[i].posy >= WINDOW_HEI)
		{
			particles[i].posy = WINDOW_HEI;
			particles[i].spdy *= -1;
		}
		else if (particles[i].posy <= 0)
		{
			particles[i].posy = 0;
			particles[i].spdy *= -1;
		}
	}
}

void GenerateOutput()
{
	DarkenHalf();

	for (int i = 0; i < PARTIC_NUM; ++i)
	{
		setcolor(particles[i].color);
		setfillstyle(particles[i].color);
		fillcircle(int(particles[i].posx + 0.5), int(particles[i].posy + 0.5), int(particles[i].radius + 0.5));
	}
	FlushBatchDraw();
}

void DarkenHalf()
{
	for (int i = 0; i < WINDOW_WID * WINDOW_HEI; ++i)
	{
		if (pBuffer[i] != 0)
			pBuffer[i] = RGB(GetRValue(pBuffer[i]) >> 1, GetGValue(pBuffer[i]) >> 1, GetBValue(pBuffer[i]) >> 1);
	}
}

void DelayTime(DWORD ms)
{
	Ticks = GetTickCount();

	while (GetTickCount() - Ticks < ms)
		Sleep(1);

	Ticks = GetTickCount();
}

void ShutDown()
{
	EndBatchDraw();
	closegraph();
}
