#pragma once

#include <vector>
#include "Screen.h"

using namespace std;

class Snake {
public:
	Snake();
	~Snake();
	void Reset(COORD start_pos);									// "reset" snake
	void Draw(Screen& scr);											// initial drawing of a snake on the screen
	void Move(const COORD& delta, Screen& scr, int mode_number);	// snake movement by increment of coordinates
	void Grow(const COORD& pos, int growbits);						// increasing the length of the snake
	bool IsInSnakeBody(const COORD& pos);							// checking if the coordinates hit the body of the snake
	COORD GetSnakeHeadCoordinate();                                 // get snake head coordinates
	int GetSnakeSize();												// get length of the snake

private:
	vector<COORD> m_worm;											// coordinate vector of snake body segments
	string m_head_mark;												// symbol for the head of a snake
	unsigned int m_drawn;											// length of the rendered body of the snake
};