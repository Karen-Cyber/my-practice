#include "SDL2_EasyX_header.h"

int main(int argv, char* argc[])
{
	GAME game;
	bool success = game.Initialize();
	if (success)
	{
		game.RunLoop();
	}
	game.ShutDown();
	return 0;
}