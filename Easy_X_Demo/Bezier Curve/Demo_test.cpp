#include <graphics.h>
#include <list>
#include <cmath>
using namespace std;

const int WINDOW_WID = 640;     // 窗口宽度
const int WINDOW_HEI = 480;     // 窗口高度
const int TIMEPERFRP = 14;      // 每帧时间
const int BASELINE_B = 5;       // 基线粗细
const int BASECIRC_R = 10;      // 节点半径
double ACCURACY_A = 0.1;        // 曲线精度(越小越平滑)
double ACCURACY_B = -0.01;      // 曲线精度改变量
bool mIsRunning;

int Mousex;
int Mousey;
bool MouseLeft;
bool MouseRight;
bool MouseMid;
bool MouseWheel;

class Point
{
public:
	int mPosx;
	int mPosy;
	int mRadius;

public:
	Point();
	Point(const Point& p);
	Point(int x, int y);
	Point(int x, int y, int r);
	~Point();
};                                                 // 自定义的横坐标类
Point::Point()
{
	mPosx = 0;
	mPosy = 0;
	mRadius = BASECIRC_R;
}
Point::Point(const Point& p)
{
	mPosx = p.mPosx;
	mPosy = p.mPosy;
	mRadius = p.mRadius;
}
Point::Point(int x, int y)
{
	mPosx = x;
	mPosy = y;
	mRadius = BASECIRC_R;
}
Point::Point(int x, int y, int r)
{
	mPosx = x;
	mPosy = y;
	mRadius = r;
}
Point::~Point()
{

}
list<Point> Bases; // 存储贝塞尔曲线基点
list<Point> Temps; // 存储贝塞尔曲线分点


void Initialize();
void ProcessInput();
void Update();
void GenerateOutput();
void RunLoop();
void ShutDown();
void Delay(DWORD ms);

Point BezierCurve(const list<Point>& given, double ration);
void DrawLine(const list<Point>& given);
void DrawPoint(const list<Point>& given);

int main()
{
	Initialize();
	RunLoop();
	ShutDown();

	return 0;
}

void Initialize()
{
	initgraph(WINDOW_WID, WINDOW_HEI, SHOWCONSOLE);
	BeginBatchDraw();
	mIsRunning = true;

	Mousex = 0.0;
	Mousey = 0.0;
	MouseLeft = false;
	MouseRight = false;
	MouseMid = false;
	MouseWheel = false;

	Bases.push_back(Point(50, 50));
	Bases.push_back(Point(WINDOW_WID - 50, WINDOW_HEI - 50));
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
			Mousex = m.x; 
			Mousey = m.y; 
			break;

		case WM_LBUTTONDOWN: 
			MouseLeft = !MouseLeft;    
			break;

		case WM_RBUTTONDOWN:  
			MouseRight = true;   
			break;

		case WM_MBUTTONDOWN:
			MouseMid = true;
			break;

		case WM_MOUSEWHEEL:
			MouseWheel = true;
			break;

		default:
			break;
		}
	}
}

void Update()
{
	if (MouseLeft)
	{
		for (auto iter = Bases.begin(); iter != Bases.end(); ++iter)
		{
			int Distance = sqrt(pow(Mousex - iter->mPosx, 2) + pow(Mousey - iter->mPosy, 2));
			if (int(Distance) <= BASECIRC_R + 5)
			{
				iter->mPosx = Mousex;
				iter->mPosy = Mousey;
				break;
			}
		}
	}
	if (MouseRight)
	{
		Bases.push_back(Point(Mousex, Mousey));
		MouseRight = false;
	}
	if (MouseMid)
	{
		if (Bases.size() > 2) // 如果只剩一个点的话，就无法形成曲线了，不允许删除最后两个点
		{
			for (auto iter = Bases.begin(); iter != Bases.end(); ++iter)
			{
				int Distance = sqrt(pow(Mousex - iter->mPosx, 2) + pow(Mousey - iter->mPosy, 2));
				if (int(Distance) <= BASECIRC_R + 5)
				{
					Bases.erase(iter);
					break;
				}
			}
		}
		// 改变精度调整方向
		ACCURACY_B *= -1.0;
		MouseMid = false;
	}
	if (MouseWheel)
	{
		if (ACCURACY_A > 0.05 && ACCURACY_A < 1.0)
		{
			ACCURACY_A += ACCURACY_B;
		}
		else
			ACCURACY_A = 0.1;
		MouseWheel = false;
	}

	// Calculate Bezier Curve point
	double ratio = 0.0;
	Temps.clear(); // 清空上一次的节点
	while (ratio < 1.0)
	{
		Temps.push_back(BezierCurve(Bases, ratio));
		ratio += ACCURACY_A;
	}
}

void GenerateOutput()
{
	cleardevice();
	// Draw Basic Points
	setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 3);
	setlinecolor(BLUE);
	setfillcolor(GREEN);
	DrawPoint(Bases);

	// Draw Basic lines
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, BASELINE_B);
	DrawLine(Bases);
	
	// Draw Bezier Curve Point
	setlinestyle(PS_SOLID, 2);
	setlinecolor(YELLOW);
	setfillcolor(WHITE);
	DrawPoint(Temps);
	// Draw Bezier Curve Line
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, BASELINE_B / 2);
	DrawLine(Temps);

	FlushBatchDraw();
}

void RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		Update();
		GenerateOutput();
		Delay(TIMEPERFRP);
	}
}

void Delay(DWORD ms)
{
	static DWORD oldtime = GetTickCount();

	while (GetTickCount() - oldtime < ms)
		Sleep(1);

	oldtime = GetTickCount();
}

void ShutDown()
{
	Bases.clear();
	Temps.clear();
	EndBatchDraw();
	closegraph();
	closegraph();
}

Point BezierCurve(const list<Point>& given, double ration)
{
	list<Point> copy = given;
	list<Point> store;
	while (copy.size() > 1)
	{
		for (auto iter = copy.begin(); iter != copy.end(); ++iter)
		{
			int x1 = iter->mPosx;
			int y1 = iter->mPosy;
			iter++;
			if (iter != copy.end())
			{
				int x2 = iter->mPosx;
				int y2 = iter->mPosy;
				// Create next point
				int tempx = x1 + int((x2 - x1) * ration);
				int tempy = y1 + int((y2 - y1) * ration);
				store.push_back(Point(tempx, tempy, BASECIRC_R / 2));
			}
			iter--;
		}
		copy.clear();
		copy = store;
		store.clear();
	}

	return *copy.begin();
}

void DrawLine(const list<Point>& given)
{
	for (auto iter = given.begin(); iter != given.end(); ++iter)
	{
		int tempx1 = iter->mPosx;
		int tempy1 = iter->mPosy;
		iter++;
		if (iter != given.end())
		{
			int tempx2 = iter->mPosx;
			int tempy2 = iter->mPosy;
			line(tempx1, tempy1, tempx2, tempy2);
		}
		iter--;
	}
}

void DrawPoint(const list<Point>& given)
{
	for (auto iter = given.begin(); iter != given.end(); ++iter)
	{
		fillcircle(iter->mPosx, iter->mPosy, iter->mRadius);
	}
}