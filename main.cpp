//女生节祝福
//所有坐标单位均为像素单位
#include <graphics.h>
#include <random>
#include <windows.h>
#include <ctime>
#include <math.h>
#include <conio.h>
#define Rain_num 40
#define Leaf_num 6
#define Flower_size 15
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

struct Pos
{
	unsigned int Flower_Pos_x;
	int Flower_Pos_y;
}Rain[Rain_num];
const char message[18] = "Happy Girl's Day!"; int text_y = 0;

void Init_Flower_Pos()
{
	srand(time(0));
	for (int i = 0; i < Rain_num; ++i)
	{
		Rain[i].Flower_Pos_x = rand() % SCREEN_WIDTH;                  //横坐标在编译时确定
		Rain[i].Flower_Pos_y = rand() % SCREEN_HEIGHT;
	}
}

void Update_Flower_Pos_y()
{
	for (int i = 0; i < Rain_num; ++i)
	{
		Rain[i].Flower_Pos_y = (Rain[i].Flower_Pos_y + 15) % 795;  //更新一次下降20像素单位
	}
}

void Draw_Flower()
{
	double temp_x = 0.0;
	double temp_y = 0.0;
	double temp_rou = 0.0;
	int red = 0;
	int green = 0;
	int blue = 0;
	for (int i = 0; i < Rain_num; ++i)
	{
		red = rand() % 255;
		green = rand() % 255;
		blue = rand() % 255;
		for (double sita = 0.0; sita < 100.0; sita += 0.5)
		{
			temp_rou = Flower_size * sin(Leaf_num * sita);                   //15为花瓣大小，像素单位
			temp_x = temp_rou * cos(sita);
			temp_y = temp_rou * sin(sita);
			putpixel(temp_x + Rain[i].Flower_Pos_x, temp_y + Rain[i].Flower_Pos_y, RGB(red, green, blue));
		}
	}
}

void Display_text(int n)
{
	settextstyle(40, 0, _T("The song"));
	settextcolor(RGB(150 + 100 * sin(n), 150 + 100 * sin(0.9 * n), 150 + 100 * sin(0.8 * n)));
	for (int times = 0; times < 3; ++times)
	{
		for (int i = 0; i < 17; i++)
		{
			outtextxy(510 + i * 20, 420 - times * 40 + 20 * sin(0.20 * n + 0.1 * i), message[i]);
		}
	}
}

void Draw_Windmill(int n)
{
	double temp_x;
	double temp_y;
	double temp_rou;
	
	setlinecolor(RGB(150+ 100 * sin(0.1 * n), 150 + 100 *sin(0.5 * n), 150 + 100 * cos(0.7 * n)));
	for (double sita = 0.0; sita < 150.0; sita++)
	{
		temp_rou = 100 * sin(2 * sita + 0.15 * n);
		temp_x = temp_rou * cos(sita);
		temp_y = temp_rou * sin(sita);
		circle(temp_x + 200, temp_y + 380 + 250 * cos(0.05 * n), 5);
		circle(temp_x + 1150, temp_y + 380 - 250 * cos(0.05 * n), 5);
	}
}



int main()
{
	initgraph(1366, 768);
	Init_Flower_Pos();

	BeginBatchDraw();

	while (true)
	{
		Draw_Flower();
		Display_text(text_y);
		Draw_Windmill(text_y);
		FlushBatchDraw();
		Update_Flower_Pos_y();
		text_y = (text_y++) % 1080;
		Sleep(10);
		cleardevice();
	}

	return 0;
}