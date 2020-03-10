//Greedy Snake
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <random>
#include <ctime>
#define Map_Size 21                                               //Additional one_block for wall

bool Game_state = 0;                                              //Used for specify win or lose
char Direction = 'w';
int Snake_len = 5;
int Snake_steps = 1;
int Map[Map_Size][Map_Size];                                      //All have been set to zero now
int Snake_trace[Map_Size][Map_Size];                              //Tract the steps in every block
int Fruit[2];
int Snake_Head[2] = { Map_Size / 2, Map_Size / 2 };               //0 for x-axis and 1 for y-axis

inline void Init_Map()
{
    srand(time(0));
    Fruit[0] = rand() % (Map_Size - 2) + 1;
    Fruit[1] = rand() % (Map_Size - 2) + 1;
    Snake_Head[0] = Snake_Head[1] = Map_Size / 2;
    Snake_trace[Snake_Head[0]][Snake_Head[1]] = 1;

    for (int i = 0; i < Map_Size; ++i)
    {
        for (int j = 0; j < Map_Size; ++j)
        {
            if ((i == 0 || i == Map_Size - 1) || (j == 0 || j == Map_Size - 1))
                Map[i][j] = -1;//Initialized as wall
        }
    }

    Map[Snake_Head[0]][Snake_Head[1]] = 1;//1 stands for the snake
    Map[Fruit[0]][Fruit[1]] = 2;//2 stands for the fruit
}

inline void BackGround(int Forge_color = 7, int Back_color = 0)//change the next output color of console and Background
{
    HANDLE handle;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, Forge_color + Back_color * 0x10);
}

inline void Snake_move()
{
    char temp;
    if (_kbhit())
    {
        temp = _getch();
        if (temp == 'w' && Direction != 's')
        {
            Snake_Head[0]--;
            Direction = temp;
        }
        else if (temp == 'a' && Direction != 'd')
        {
            Snake_Head[1]--;
            Direction = temp;
        }
        else if (temp == 's' && Direction != 'w')
        {
            Snake_Head[0]++;
            Direction = temp;
        }
        else if (temp == 'd' && Direction != 'a')
        {
            Snake_Head[1]++;
            Direction = temp;
        }
        Snake_steps++;
        Snake_trace[Snake_Head[0]][Snake_Head[1]] = Snake_steps;

        if (Map[Snake_Head[0]][Snake_Head[1]] == 1)
        {
            Game_state = 1;
            return;
        }

        Map[Snake_Head[0]][Snake_Head[1]] = 1;
    }
    else
    {
        if (Direction == 'w')
            Snake_Head[0]--;
        else if (Direction == 'a')
            Snake_Head[1]--;
        else if (Direction == 's')
            Snake_Head[0]++;
        else if (Direction == 'd')
            Snake_Head[1]++;
        Snake_steps++;
        Snake_trace[Snake_Head[0]][Snake_Head[1]] = Snake_steps;

        if (Map[Snake_Head[0]][Snake_Head[1]] == 1)
        {
            Game_state = 1;
            return;
        }

        Map[Snake_Head[0]][Snake_Head[1]] = 1;
    }
}

inline void Fruit_move()
{
    Fruit[0] = rand() % (Map_Size - 2) + 1;
    Fruit[1] = rand() % (Map_Size - 2) + 1;
    Map[Fruit[0]][Fruit[1]] = 2;
}

inline void Erase_tail()
{
    for (int i = 1; i < Map_Size - 1; ++i)
    {
        for (int j = 1; j < Map_Size - 1; ++j)
        {
            if (Snake_trace[i][j] == Snake_steps - Snake_len)
            {
                Map[i][j] = 0;                                  //There will only be one suit the condition
                return;
            }
        }
    }
}

inline void Print_map()
{
    for (int i = 0; i < Map_Size; ++i)
    {
        for (int j = 0; j < Map_Size; ++j)
        {
            switch (Map[i][j])
            {
            case -1:
                BackGround(0, 1);                                 //Print Wall with Blue
                std::cout << "  ";
                break;
            case 0:                                               //Print Nothing with Black
                BackGround(0, 0);
                std::cout << "  ";
                break;
            case 1:                                               //Print Snake with Green
                BackGround(0, 2);
                std::cout << "  ";
                break;
            case 2:                                               //Print Fruit with Yellow
                BackGround(0, 6);
                std::cout << "  ";
                break;
            }
            //Sleep(10);
        }
        BackGround(0, 0);
        std::cout << std::endl;
    }
    HANDLE handle;
    COORD coord;
    coord.X = coord.Y = 0;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, coord);
}

inline void Game_judge()
{
    if (Snake_Head[0] == 0 || Snake_Head[0] == Map_Size - 1)       //If Snake hit itself or the wall, game over
        Game_state = 1;
    else if (Snake_Head[1] == 0 || Snake_Head[1] == Map_Size - 1)
        Game_state = 1;
    /*else if (Map[Snake_Head[0]][Snake_Head[1]])
        Game_state = 1;*/
    else if (Snake_Head[0] == Fruit[0] && Snake_Head[1] == Fruit[1])               //Snake had eatten the fruit
    {
        Map[Snake_Head[0]][Snake_Head[1]] = 1;
        Snake_len++;
        Fruit_move();
    }
}

int main()
{
    srand(time(0));
    Init_Map();

    while (true)
    {
        Snake_move();
        Erase_tail();
        Print_map();
        Game_judge();
        if (Game_state == 1)
            break;
        Sleep(100);
    }
    system("cls");
    BackGround();

    std::cout << "Game over!\n";
    return 0;
}

/*void Init_Map()
{
    srand(time(0));
    Fruit[0] = rand() % (Map_Size - 2) + 1;
    Fruit[1] = rand() % (Map_Size - 2) + 1;
    Snake_Head[0] = Snake_Head[1] = Map_Size / 2;
    Snake_trace[Snake_Head[0]][Snake_Head[1]] = 1;

    for (int i = 0; i < Map_Size; ++i)
    {
        for (int j = 0; j < Map_Size; ++j)
        {
            if ((i == 0 || i == Map_Size - 1) || (j == 0 || j == Map_Size - 1))
                Map[i][j] = -1;//Initialized as wall
        }
    }

    Map[Snake_Head[0]][Snake_Head[1]] = 1;//1 stands for the snake
    Map[Fruit[0]][Fruit[1]] = 2;//2 stands for the fruit
}*/