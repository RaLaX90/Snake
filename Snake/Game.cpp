#include "Game.h"

string BORDER = "#";    // ������ ��� ��������� ����� �������� ����

string ver_number = "v 1.0";
//const string copyright = "(c) Cranium, 2014.";

string food_symbol = "$";      // ������ ��� ������ ���

// �����������
// _scr     - ������, �������������� ����� �� �������
// _width   - ������ �������� ���� (x)
// _height  - ������ �������� ���� (y)
// _latency - �������� ����� ���������� ������� � �������������
Game::Game(Screen& _scr, int _width, int _height, int _latency) :
	screen(_scr), latency(_latency) {

	if (_width == 0 && _height == 0) {
		CONSOLE_SCREEN_BUFFER_INFO pcsbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &pcsbi);

		width = pcsbi.dwMaximumWindowSize.X;
		height = pcsbi.dwMaximumWindowSize.Y / 2 - 2;
	}
	else {
		width = _width;
		height = _height;
	}

	m_generator = mt19937(m_rd());
	m_distribution_width = uniform_int_distribution<int>(1, width - 2);
	m_distribution_height = uniform_int_distribution<int>(1, height - 2);

	duration_game = 0;

	// ������������� ������� ������ ���������� �����
	cmd_table[0] = pair<int, Command>(27, CMD_EXIT);   // escape key
	cmd_table[1] = pair<int, Command>('K', CMD_LEFT);  // ������� �����
	cmd_table[2] = pair<int, Command>('M', CMD_RIGHT); // ������� ������
	cmd_table[3] = pair<int, Command>('H', CMD_UP);    // ������� �����
	cmd_table[4] = pair<int, Command>('P', CMD_DOWN);  // ������� ����
}

Game::Command Game::getCommand() {
	int ch;

	ch = _getch();
	if (ch == 0 || ch == 0xe0) {
		ch = _getch();
	}

	for (int i = 0; i < 5; i++) {
		if (cmd_table[i].first == ch) {
			return cmd_table[i].second;
		}
	}

	return CMD_NOCOMMAND;
}

void Game::clearkeys()
{
	while (_kbhit()) {
		_getch();
	}
}

// ���������� ��� ����������� ��������� �������.
// �����������: ���������� �� ������ �������� � ���� ����.
COORD Game::createFood() {
	COORD food{};

	do {
		food.X = m_distribution_width(m_generator);
		food.Y = m_distribution_height(m_generator);
	} while (snake.IsInSnakeBody(food));

	return food;
}

void Game::drawGameField() {

	screen.ClearScreen();

	for (int i = 0; i < height; i++) {
		if (i == 0 || i == height - 1) {
			for (int j = 0; j < width; j++) {
				screen.PrintString(j, i, BORDER);
			}
		}
		else {
			screen.PrintString(0, i, BORDER);
			screen.PrintString(width - 1, i, BORDER);
		}
	}
}

void Game::printStatistics() {
	screen.PrintString(0, height, "Length: " + to_string(snake.GetSnakeSize()));
	screen.PrintString(12, height, "Game duration: " + to_string(duration_game));
}

void Game::WaitForClick(int position_x, int position_y) {
	screen.PrintString(width / 2, height / 2, "Press any key for continue...");
	_getch();
	clearkeys();
}

bool Game::IsOnceMore() {
	screen.PrintString(width / 2 - 8, 13, "O n c e    m o r e ?");

	int ch = _getch();
	clearkeys();

	if (ch == 'N' || ch == 'n' || ch == 27) {
		return false;
	}

	return true;
}

void Game::PrintLogo() {
	screen.PrintString(width / 2 + 7, height / 2 - 2, "S  N  A  K  E");
	screen.PrintString(width / 2 + 11, height / 2 + 10, ver_number);
	//screen.PrintString(width / 2 - 9, height, copyright);
}

void Game::EndGame() {
	screen.ClearScreen();
	screen.PrintString(width / 2 - 9, height, "Snake " + ver_number);
	//_cprintf_s("Oldschool Snake %s\n%s\n", ver_number, copyright);
}

void Game::StartGameLoop() {

	duration_game = 0;

	drawGameField();           // ���������� ������� ����

	COORD debug{ width / 2, height / 2 };
	snake.Reset(debug);     // ���������� ����: ����� 2,
													// ��������� - � �������� �������� ����,
													// ����������� - �����
	Command cmd = CMD_NOCOMMAND;
	State state = STATE_OK;
	// delta  �������� ���������� ��������� (dx, dy) ��� ������� ����������� ���� �� ����
	COORD delta{ -1, 0 };                // ��������� �������� - �����
	COORD food = createFood();          // ��������� ���������� ���
	screen.PrintString(food.X, food.Y, food_symbol);      // ������� ��� �� �����

	snake.Draw(screen);                    // ��������� ��������� ����

	printStatistics();                       // ������� ��������� ���������� ����

	clock_t time1, time2, duration;
	time1 = clock();

	do {

		if (_kbhit()) {					// ���� � ������ ���������� ���� ����������,
			cmd = getCommand();        // �� ������� �������
		}

		// ��������� ������
		switch (cmd) {
		case CMD_LEFT: {
			delta = COORD{ -1, 0 };
			break;
		}
		case CMD_RIGHT: {
			delta = COORD{ 1, 0 };
			break;
		}
		case CMD_UP: {
			delta = COORD{ 0, -1 };
			break;
		}
		case CMD_DOWN: {
			delta = COORD{ 0, 1 };
			break;
		}
		case CMD_EXIT: {
			state = STATE_EXIT;
			break;
		}
		default: {
			break;
		}
		};

		COORD snake_head_coordinate = snake.GetSnakeHeadCoordinate();   // ���������� ������ ����
		snake_head_coordinate.X += delta.X;								// ���������� ������ ���� ����� ���������� (��������� �������)
		snake_head_coordinate.Y += delta.Y;								// ���������� ������ ���� ����� ���������� (��������� �������)

		// ���� ������ ���� ����������� � �������� ���� ��� �� ����� ����, �� ���� �������
		if (snake_head_coordinate.X == 0 || snake_head_coordinate.X == width - 1 || snake_head_coordinate.Y == 0 || snake_head_coordinate.Y == height - 1 || snake.IsInSnakeBody(snake_head_coordinate)) {
			state = STATE_DIED;
		}

		if (state == STATE_OK) {          // ���� ���� ��� ����, ��
			snake.Move(delta, screen);     // �������� ���� �� delta

			if (snake.GetSnakeHeadCoordinate().X == food.X && snake.GetSnakeHeadCoordinate().Y == food.Y) { // ���� ���������� ������ ���� ��������� � ����������� ���, ��
				snake.Grow(food, 3);    // ��������� ����� ����
				food = createFood();     // ��������� ���������� ����� ���
				screen.PrintString(food.X, food.Y, food_symbol); // ������� ��� �� �����

				// ���������� ������� ����, ���������� � ������ ��������.
				// ��������� ������� ����������� ��� ����� ����, ������� �� ����� � ��������,
				// ����������� �� ������ � ��� (����� �� �������� ���������� ��� �� �������� ���������).
				// ����� �������, ��� ���� ���� ��� � ��� ��� �������, ��� ���� ��������� �������.
				time2 = clock();
				duration = time2 - time1;
				duration_game += static_cast<double>(duration) / CLOCKS_PER_SEC;

				printStatistics();           // ����� ������� ���������� ����
			}

			Sleep(latency);             // �������� ����� ��������� ���������� �������
		}

	} while (state == STATE_OK);          // ������, ���� ���� ����

	screen.PrintString(width / 2 - 8, 10, " G a m e    o v e r ");
	clearkeys();
	_getch();
	clearkeys();
}