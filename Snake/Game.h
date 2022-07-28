#pragma once

#include <conio.h>
#include "Snake.h"
#include <random>

using namespace std;

class Game {
public:
	Game(Screen& _scr, int _latency = 100);
	void StartGameLoop(int mode_number);				// main game loop
	bool IsOnceMore();									// displaying a request and receiving a response from the player
	void WaitForClick(int position_x, int position_y);  // wait for user response
	void PrintLogo();									// print logo
	void EndGame();										// output text at the end of the game

private:
	enum Command { CMD_NOCOMMAND = 0, CMD_EXIT, CMD_LEFT, CMD_RIGHT, CMD_UP, CMD_DOWN };
	enum State { STATE_OK = 0, STATE_EXIT, STATE_DIED };

	pair<int, Command> cmd_table[5];					// game command table

	int latency;										// delay between position changes in milliseconds
	Screen screen;										// screen object
	Snake snake;										// snake object
	double duration_game;								// duration

	random_device m_rd;
	mt19937 m_generator;								// generator for distribution
	uniform_int_distribution<int> m_distribution_width;	// object for random distribution of width
	uniform_int_distribution<int> m_distribution_height;// object for random distribution of height

	void drawGameField(string symbol);					// drawing the playing field
	COORD createFood();									// position calculation for food
	void printStatistics();								// output of current statistics
	Command getCommand();								// receiving commands from the keyboard
	void clearkeys();									// clearing the keyboard buffer
};