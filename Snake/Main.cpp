#include <iostream>
#include <conio.h>
//#include <windows.h>

#include "Screen.h"
#include "Game.h"

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void Setup() {
	gameOver = false;
	dir = STOP;
	x = width / 2 - 1;
	y = height / 2 - 1;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

void Draw() {
	system("cls"); //system ("clear");
	for (int i = 0; i < width + 1; i++) {
		cout << "#";
	}
	cout << endl;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0 || j == width - 1) {
				cout << "#";
			}

			if (i == y && j == x) {
				cout << "0";
			}
			else if (i == fruitY && j == fruitX) {
				cout << "F";
			}
			else {
				bool print = false;
				for (int k = 0; k < nTail; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						print = true;
						cout << "o";
					}
				}
				if (!print) {
					cout << " ";
				}
			}
		}
		cout << endl;
	}

	for (int i = 0; i < width + 1; i++) {
		cout << "#";
	}
	cout << endl;
	cout << "Score " << score << endl;
}

void Input() {
	if (_kbhit()) {
		switch (_getch())
		{
		case 'a': {
			dir = LEFT;
			break;
		}
		case 'd': {
			dir = RIGHT;
			break;
		}
		case 'w': {
			dir = UP;
			break;
		}
		case 's': {
			dir = DOWN;
			break;
		}
		}
	}
}

void Logic() {
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++) {
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir)
	{
	case LEFT: {
		x--;
		break;
	}
	case RIGHT: {
		x++;
		break;
	}
	case UP: {
		y--;
		break;
	}
	case DOWN: {
		y++;
		break;
	}
	}

	/*if (x > width || x < 0 || y > height || y < 0) {
		gameOver = true;
	}*/

	if (x >= width - 1) {
		x = 0;
	}
	else if (x < 0) {
		x = width - 2;
	}

	if (y >= height) {
		y = 0;
	}
	else if (y < 0) {
		y = height - 1;
	}

	for (int i = 0; i < nTail; i++) {
		if (tailX[i] == x && tailY[i] == y) {
			gameOver = true;
		}
	}

	if (x == fruitX && y == fruitY) {
		score++;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}
}

void goodbye() {
	system("cls");
	_cprintf("Oldschool Snake %s\n%s\n", "1 ", "2");
}

int main()
{
	try
	{
		Screen screen;
		screen.cursor_show(false);
		screen.text_attr((WORD)0x0a);
		screen.cls();
		Game game(screen, 80, 24, 120);

		game.logo();

		game.wait_to_click(18);

		_cputs("Type 'Y' when finished typing keys: ");
		int ch = toupper(_getch());

		do {
			game.game_loop();

		} while (game.once_more());

		game.goodbye();

		//Setup();
		//while (!gameOver) {
		//	Draw();
		//	Input();
		//	Logic();
		//	//Sleep(200);
		//}
		//goodbye();
	}
	catch (const std::exception& Ex)
	{
		cerr << "*** " << Ex.what() << endl;
	}

	return 0;
}