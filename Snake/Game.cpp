#include "Game.h"

string ver_number = "v 1.0";

string food_symbol = "$";     // symbol for displaying food

// Constructor
// _scr - an object that outputs to the console
// _latency - delay between position changes in milliseconds
Game::Game(const Screen& _scr, uint8_t _latency) :
	screen(_scr), latency(_latency) {

	m_generator = mt19937(m_rd());
	m_distribution_width = uniform_int_distribution<short>(1, screen.GetWidth() - 2);
	m_distribution_height = uniform_int_distribution<short>(1, screen.GetHeight() - 2);

	duration_game = 0;

	// initialization of the table of game control commands
	cmd_table[0] = pair<int, Command>(27, CMD_EXIT);   // escape key
	cmd_table[1] = pair<int, Command>('K', CMD_LEFT);  // left arrow
	cmd_table[2] = pair<int, Command>('M', CMD_RIGHT); // right arrow
	cmd_table[3] = pair<int, Command>('H', CMD_UP);    // up arrow
	cmd_table[4] = pair<int, Command>('P', CMD_DOWN);  // arrow to down
}

Game::Command Game::getCommand() {
	int ch = _getch();
	if (ch == 0 || ch == 0xe0) {
		ch = _getch();
	}

	for (uint8_t i = 0; i < 5; i++) {
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

// The food coordinate is calculated randomly.
// Restriction: the coordinate must not fall into the body of the snake.
COORD Game::createFood() {
	COORD food{};

	do {
		food.X = m_distribution_width(m_generator);
		food.Y = m_distribution_height(m_generator);
	} while (snake.IsInSnakeBody(food));

	return food;
}

void Game::drawGameField(string symbol) {

	screen.ClearScreen();

	for (uint16_t i = 0; i < screen.GetHeight(); i++) {
		if (i == 0 || i == screen.GetHeight() - 1) {
			for (uint16_t j = 0; j < screen.GetWidth(); j++) {
				screen.PrintString(j, i, symbol);
			}
		}
		else {
			screen.PrintString(0, i, symbol);
			screen.PrintString(screen.GetWidth() - 1, i, symbol);
		}
	}
}

void Game::printStatistics() {
	screen.PrintString(0, screen.GetHeight(), "Length: " + to_string(snake.GetSnakeSize()));
	screen.PrintString(12, screen.GetHeight(), "Game duration: " + to_string(duration_game));
}

void Game::WaitForClick() {
	screen.PrintString(screen.GetWidth() / 2, screen.GetHeight() / 2, "Press any key for continue...");
	_getch();
	clearkeys();
}

bool Game::IsOnceMore() {
	screen.PrintString(screen.GetWidth() / 2 - 8, 13, "O n c e    m o r e ?");

	int ch = _getch();
	clearkeys();

	if (ch == 'N' || ch == 'n' || ch == 27) {
		return false;
	}

	return true;
}

void Game::PrintLogo() {
	screen.PrintString(screen.GetWidth() / 2 + 7, screen.GetHeight() / 2 - 2, "S  N  A  K  E");
	screen.PrintString(screen.GetWidth() / 2 + 11, screen.GetHeight() / 2 + 10, ver_number);
}

void Game::EndGame() {
	screen.ClearScreen();
	screen.PrintString(0, 0, "Snake " + ver_number);
}

void Game::StartGameLoop(uint8_t mode_number) {

	thread th([&]() { drawGameField("#"); });						// draw the playing field parallel
	
	duration_game = 0;

	snake.Reset(screen.GetWidth() / 2, screen.GetHeight() / 2);		// set snake: length 2,
																	// position - in the middle of the playing field,
																	// direction - left
	snake.Draw(screen);												// initial snake drawing

	Command cmd = CMD_NOCOMMAND;
	State state = STATE_OK;
	// delta contains the coordinate increment (dx, dy) for each snake move across the field
	COORD delta{ -1, 0 };                // initial movement - to the left
	COORD food = createFood();          // calculate food coordinates

	th.join();												// wait until the playing field is completed before drawing food 
	screen.PrintString(food.X, food.Y, food_symbol);      // display food on the screen

	printStatistics();                       // display the initial statistics of the game

	clock_t time1, time2, duration;
	time1 = clock();

	do {

		if (_kbhit()) {					// if there is information in the keyboard buffer,
			cmd = getCommand();        // then accept the command
		}

		// обработка команд
		switch (cmd) {
		case CMD_LEFT: {
			if (delta.X != 1 && delta.Y != 0) {
				delta = COORD{ -1, 0 };
			}
			break;
		}
		case CMD_RIGHT: {
			if (delta.X != -1 && delta.Y != 0) {
				delta = COORD{ 1, 0 };
			}
			break;
		}
		case CMD_UP: {
			if (delta.X != 0 && delta.Y != -1) {
				delta = COORD{ 0, -1 };
			}
			break;
		}
		case CMD_DOWN: {
			if (delta.X != 0 && delta.Y != 1) {
				delta = COORD{ 0, 1 };
			}
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

		snake.Move(delta, screen, mode_number);     // move the snake to delta

		COORD snake_head_coordinate = snake.GetSnakeHeadCoordinate();   // snake head coordinate

		// if the snake's head collides with the field boundary or with the snake's body, then the snake dies
		if (snake_head_coordinate.X == 0 || snake_head_coordinate.X == screen.GetWidth() - 1 || snake_head_coordinate.Y == 0 || snake_head_coordinate.Y == screen.GetHeight() - 1 || snake.IsInSnakeBody(snake_head_coordinate)) {
			state = STATE_DIED;
		}

		if (state == STATE_OK) {          // if the snake is still alive, then
			if (snake.GetSnakeHeadCoordinate().X == food.X && snake.GetSnakeHeadCoordinate().Y == food.Y) { // if the coordinate of the snake's head is the same as the coordinate of the food, then
				snake.Grow(food, 3);    // increase the length of the snake

				food = createFood();     // calculate the coordinates of the new food
				screen.PrintString(food.X, food.Y, food_symbol); // bring food to the screen

				time2 = clock();
				duration = time2 - time1;
				duration_game += static_cast<double>(duration) / CLOCKS_PER_SEC;

				printStatistics();           // output of current game statistics
			}

			snake.Draw(screen);			// drawing a snake
			Sleep(latency);             // delay before next position change
		}

	} while (state == STATE_OK);          // play while the snake is alive

	screen.PrintString(screen.GetWidth() / 2 - 8, 10, " G a m e    o v e r ");
	clearkeys();
	_getch();
	clearkeys();
}