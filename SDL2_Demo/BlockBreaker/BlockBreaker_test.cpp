#include "BlockBreaker_h.h"
#include <iostream>

int main()
{
	Game game;
	game.Initialize();
	game.RunLoop();
	game.Shutdown();

	std::cout << "You got a total score of: " << game.GetScores() << std::endl;
	return 0;
}