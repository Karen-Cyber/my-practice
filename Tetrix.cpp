//This is a brief test with Easy_x
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <random>
#include <ctime>
#define Width 20
#define Height 25

struct Movin_Unit
{
	int x;
	int y;
	int kind;
	int rotate;
} Current, Next;

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
COORD coord;

int map[Height][Width];

//只有三种方块，每个方块两种方向，包含四个偏移坐标
int Block[3][2][4][2] =
{
	{
		{{0,0},{0,-1},{0,1},{0,2}},
		{{0,0},{1,0},{2,0},{-1,0}}
	},

	{
		{{0,0},{0,-1},{1,0},{1,1}},
		{{0,0},{-1,0},{0,-1},{1,-1}}
	},

	{
		{{0,0},{0,1},{0,2},{1,0}},
		{{0,0},{-1,0},{0,-1},{0,-2}}
	}
};

enum KEY {TURN, DOWN, RIGHT, LEFT, ESC, NONE};
KEY k;
bool WOL = true;
int scores;

void GotoXY(const int x, const int y)
{
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(handle, coord);
}

void DrawBlock() //记录砖块当前的位置
{
	for (int i = 0; i < 4; ++i)
		map[Current.y + Block[Current.kind][Current.rotate][i][1]][Current.x + Block[Current.kind][Current.rotate][i][0]] = -1;
}

void EraseBlock() //消除砖块上次的痕迹
{
	for (int i = 0; i < 4; ++i)
		map[Current.y + Block[Current.kind][Current.rotate][i][1]][Current.x + Block[Current.kind][Current.rotate][i][0]] = 0;
}

void EraseLine() //是否有行可以消除？
{
	for (int i = 0; i < Height - 1; ++i)
	{
		for (int j = 0; j < Width; ++j)
		{
			if (!map[i][j])
				break;
			if (j == Width - 1)
			{
				for (int k = i; k > 0; --k)
				{
					for (int l = 0; l < Width; ++l)
						if (map[k - 1][l] == 1)
							map[k][l] = map[k - 1][l];
				}
				scores++;
			}
		}
	}
}

bool CheckMove(KEY vk) //是否能移动？
{
	if (vk == TURN)
	{
		int temp_rotate = (Current.rotate + 1) % 2;
		for (int i = 0; i < 4; ++i)
			if (map[Current.y + Block[Current.kind][temp_rotate][i][1]][Current.x + Block[Current.kind][temp_rotate][i][0]] == 1)
				return false;
		return true; //通过检测
	}
	else if (vk == LEFT)
	{
		for (int i = 0; i < 4; ++i)
			if (map[Current.y + Block[Current.kind][Current.rotate][i][1]][Current.x + 1 + Block[Current.kind][Current.rotate][i][0]] == 1)
				return false;
		return true; //通过检测
	}
	else if (vk == RIGHT)
	{
		for (int i = 0; i < 4; ++i)
			if (map[Current.y + Block[Current.kind][Current.rotate][i][1]][Current.x - 1 + Block[Current.kind][Current.rotate][i][0]] == 1)
				return false;
		return true; ////通过检测
	}
	else if (vk == DOWN)
	{
		for (int i = 0; i < 4; ++i)
			if (map[Current.y + 1 + Block[Current.kind][Current.rotate][i][1]][Current.x + Block[Current.kind][Current.rotate][i][0]] == 1)
				return false;
		return true;
	}
	else if (vk == ESC)
	{
		WOL = false;
		return true;
	}
}

void TurnWall() //该砖块转化成固定类型
{
	for (int i = 0; i < 4; ++i)
		map[Current.y + Block[Current.kind][Current.rotate][i][1]][Current.x + Block[Current.kind][Current.rotate][i][0]] = 1;
}

void CreateNew()
{
	Current.x = 6;
	Current.y = 3;
	Current.kind = rand() % 3;
	Current.rotate = rand() % 2;

}

void ProcessInput()
{
	char temp;
	if (_kbhit())
	{
		temp = _getch();
		if (temp == 'w')
			k = TURN;
		if (temp == 'a')
			k = RIGHT;
		if (temp == 'd')
			k = LEFT;
		if (temp == 's')
			k = DOWN;
		if (temp == 'q')
			k = ESC;
	}
}

void UpdateGaming()
{
	//消除上次砖块
	EraseBlock();
	//处理输入事件
	switch (k)
	{
	case TURN:
		if (CheckMove(k))
		{
			Current.rotate = (Current.rotate + 1) % 2;
		}
		break;
	case RIGHT:
		if (CheckMove(k))
		{
			Current.x--;
		}
		break;
	case LEFT:
		if (CheckMove(k))
		{
			Current.x++;
		}
		break;
	case DOWN:
		if (CheckMove(k))
		{
			Current.y+=2;
		}
		break;
	case ESC:
		WOL = false;
		break;
	case NONE:
		Current.y++;
		break;
	}
	DrawBlock();
	if (!CheckMove(DOWN)) //如果下一次已经无法向下，则转变为固定类型
	{
		TurnWall();
		CreateNew();
	}
	EraseLine();
	k = NONE;
}

void GenerateOutput()
{
	for (int i = 0; i < Height; ++i)
	{
		for (int j = 0; j < Width; ++j)
		{
			if (map[i][j] == 1 || map[i][j] == -1)
				std::cout << "[]";
			else
				std::cout << "**";
		}
		std::cout << std::endl;
	}

	GotoXY(0, 0);
}

void Initialize()
{
	system("title = Tetrix");
	//system("mode con: cols=35 lines=30");
	Current.x = 8;
	Current.y = 4;
	Current.kind = rand() % 6;
	Current.rotate = rand() % 2;
	//初始化边界
	for (int i = 0; i < Height; ++i)
		map[i][0] = map[i][Width - 1] = 1;
	for (int i = 0; i < Width; ++i)
		map[Height - 1][i] = 1;
}

int main()
{
	srand(time(0));
	Initialize();
	while (WOL)
	{
		ProcessInput();
		UpdateGaming();
		GenerateOutput();
		Sleep(150);
	}
	std::cout << "Scores: " << scores << std::endl;
	return 0;
}