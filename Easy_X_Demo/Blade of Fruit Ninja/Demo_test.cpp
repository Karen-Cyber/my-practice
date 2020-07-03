#include <graphics.h>
#include <cmath>
#include <list>
using namespace std;

const int WINDOW_WID = 640;                        // 窗口宽度
const int WINDOW_HEI = 480;                        // 窗口高度
const int CURSOR_LEN = 10;                         // 鼠标准星粗细
const double PAI = 3.1415926;                      // 自己定义的pai
const double TimeofCreateCD = 0.4;                 // 创建线段的冷却时间
const double LineExist = 2.0;                      // 创建的这段线段的留存时间
const double LineDown = 0.05;                      // 每帧线段的留存时间减少量
bool mIsRunning;                                   // 示例是否在运行的标志

double Mousex;                                     // 鼠标的横坐标
double Mousey;                                     // 鼠标的纵坐标
double MouseDegree;                                // 鼠标准星的旋转角度(默认设置为不旋转，把相应注释取消即可)
double MouseAnguSpeed;                             // 鼠标准星每帧增加的旋转角度
bool MouseClick;                                   // 鼠标左键是否按下

double CreateCD;                                   // 用于记录当前“创建线段”的CD是否冷却完毕

class Point
{
public:
	int mPosx;
	int mPosy;
	double mExist;

public:
	Point();
	Point(int x, int y, double e);
	~Point();

	void CountDown(double t);
};                                                 // 自定义的横坐标类
Point::Point()
{
	mPosx = 0;
	mPosy = 0;
	mExist = 2.0;
}
Point::Point(int x, int y, double e)
{
	mPosx = x;
	mPosy = y;
	mExist = e;
}
Point::~Point()
{

}
void Point::CountDown(double t)
{
	mExist -= t;
}
list<Point> points;                                // 利用list弹出元素自动调用析构函数的特性

void Initialize();                                 // 初始化函数(用于新建窗口还有一些其他变量)
void Update();                                     // 更新要显示的对象各种数据(比如线段粗细)
void ProcessInput();                               // 处理鼠标输入(Easyx自带的输入检测是阻塞式的，需要转换)
void GenerateOutput();                             // 打印到屏幕
void RunLoop();                                    // 循环运行
void ShutDown();                                   // 结束程序
void Delay(DWORD ms);                              // 每帧停留时间

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
	mIsRunning = true;
	Mousex = 0.0;
	Mousey = 0.0;
	MouseClick = false;
	MouseDegree = 0.0;
	MouseAnguSpeed = 0.0;

	CreateCD = 1.0;
}

void Update()
{
	if (MouseClick)                                // 取消注释以启用鼠标准星旋转
	{
		//MouseAnguSpeed += 0.05;
	}
	else
	{
		if (MouseAnguSpeed > 0.0)
		{
			MouseAnguSpeed -= 0.01;
		}
		if (MouseAnguSpeed < 0.0)
		{
			MouseAnguSpeed = 0.0;
		}
	}
	MouseDegree += MouseAnguSpeed;

	// Update tail line effect
	CreateCD -= 0.1;                                // 最好不要连续创建线段，否则看起来像曲线
	if (MouseClick && CreateCD < 0.0)
	{
		points.push_back(Point(Mousex, Mousey, LineExist));
		CreateCD = TimeofCreateCD;                  // 更新创建线段的冷却时间
	}
	if (points.size())                              // 存在点列才检测是否消减线段
	{
		for (auto iter = points.begin(); iter != points.end(); ++iter)
		{
			iter->CountDown(LineDown);
		}
		if (points.begin()->mExist < 0.0)
		{
			points.pop_front();                     // 该线段已经很细，可以消除
		}
	}
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
			MouseClick = true;
			break;

		case WM_LBUTTONUP:
			MouseClick = false;
			break;

		case WM_RBUTTONDOWN:
			mIsRunning = false;
			break;
		}
	}
}

void GenerateOutput()
{
	cleardevice();                                  // 刷新屏幕
	// Draw Cursor
	for (int i = 0; i < 4; ++i)
	{
		int tempx1 = Mousex + CURSOR_LEN * cos(MouseDegree + double(i) * 2.0 * PAI / 4.0);
		int tempy1 = Mousey + CURSOR_LEN * sin(MouseDegree + double(i) * 2.0 * PAI / 4.0);
		int tempx2 = Mousex + 2.0 * CURSOR_LEN * cos(MouseDegree + double(i) * 2.0 * PAI / 4.0);
		int tempy2 = Mousey + 2.0 * CURSOR_LEN * sin(MouseDegree + double(i) * 2.0 * PAI / 4.0);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
		line(tempx1, tempy1, tempx2, tempy2);       // 画出准星线段
	}

	// Draw tail
	if (points.size() >= 2)
	{
		for (auto iter = points.begin(); iter != points.end(); ++iter)
		{
			int tempx1 = iter->mPosx;
			int tempy1 = iter->mPosy;
			iter++;
			if (iter != points.end())
			{
				int tempx2 = iter->mPosx;
				int tempy2 = iter->mPosy;
				setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, int(10.0 * (iter->mExist / LineExist)));
				line(tempx1, tempy1, tempx2, tempy2);
			}
			iter--;
		}
	}
	FlushBatchDraw();                               // 批量绘图(或者双缓冲？)
}

void RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();                             // 处理输入
		Update();                                   // 更新对象
		GenerateOutput();                           // 显示
		Delay(10);                                  // 显示一段时间
	}
}

void ShutDown()
{
	EndBatchDraw();
	closegraph();
}

void Delay(DWORD ms)
{
	static DWORD oldtime = GetTickCount();

	while (GetTickCount() - oldtime < ms)
		Sleep(1);

	oldtime = GetTickCount();
}