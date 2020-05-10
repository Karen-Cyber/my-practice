#include "Snake_h.h"

int main()
{
	Game game;
	game.Initialize();
	game.RunLoop();
	game.Shutdown();
	return 0;
}