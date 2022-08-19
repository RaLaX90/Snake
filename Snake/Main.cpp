#include <iostream>
#include "Screen.h"
#include "Game.h"

using namespace std;

int main()
{
	try
	{
		Screen screen(80, 24);
		screen.SetCursorShow(false);
		screen.SetTextAttribute((WORD)0x0a);
		screen.ClearScreen();
		Game game(screen, 120);

		game.PrintLogo();

		game.WaitForClick();

		// display a suggestion for choosing a game mode, if the user has not chosen what we want - repeat the output
		int ch = 0;
		bool rightChoose = false;
		do
		{
			screen.ClearScreen();
			screen.PrintString(0, 0, "Choose game mode and press enter (other answers are not accepted):");
			screen.PrintString(0, 1, "1) Easy (the snake can pass through the borders of the map)");
			screen.PrintString(0, 2, "2) Middle (the snake will die when crossing the border of the map)");
			screen.PrintString(0, 3, "");
			ch = cin.get();
			if (ch == 49 || ch == 50) {
				rightChoose = true;
			}
		} while (!rightChoose);

		do {
			game.StartGameLoop(ch);

		} while (game.IsOnceMore());

		game.EndGame();
	}
	catch (const std::exception& Ex)
	{
		cerr << "*** " << Ex.what() << endl;
	}

	return 0;
}