#include <iostream>
#include <conio.h>
//#include <windows.h>

using namespace std;
bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
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
				cout << " ";
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

	if (x > width || x < 0 || y > height || y < 0) {
		gameOver = true;
	}

	if (x == fruitX && y == fruitY) {
		score++;
		fruitX = rand() % width;
		fruitY = rand() % height;
	}
}

int main()
{
	Setup();
	while (!gameOver) {
		Draw();
		Input();
		Logic();
		//Sleep(200);
	}

	return 0;
}