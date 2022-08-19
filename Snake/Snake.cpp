#include "Snake.h"

string SNAKE_TAIL = "@";        // symbol for drawing segments of the snake's body, except for the head

Snake::Snake() {
	m_head_mark = '<';
}

Snake::~Snake()
{
}

void Snake::Reset(short _start_position_X, short _start_position_Y) {
	m_worm.clear();
	m_worm.reserve(1000);												// reserve memory
	m_worm.push_back(COORD{ _start_position_X, _start_position_Y });	// add head coordinates
	m_worm.push_back(COORD{ _start_position_X, _start_position_Y });	// add tail coordinates
	m_worm[0].X++;														// x coordinate of the tail - 1 to the right
}

void Snake::Draw(Screen& scr) {
	uint16_t wsize = m_worm.size() - 1;

	for (uint8_t i = 0; i < wsize; i++) {
		scr.PrintString(m_worm[i].X, m_worm[i].Y, SNAKE_TAIL);
	}

	scr.PrintString(m_worm[wsize].X, m_worm[wsize].Y, m_head_mark);
	m_drawn = m_worm.size();
}

void Snake::Move(const COORD& delta, Screen& scr, uint8_t mode_number) {
	// When moving the snake, only the position of the head (and the first segment) is redrawn
	// and tail position. The remaining segments of the snake are not redrawn.

	// Redraw the tail.
	// The length of the snake increases as the snake grows (grow() method),
	// but the snake on the screen does not change. Therefore, if the rendered length of the snake
	// coincides with the real length, then the last segment of the snake (tail) is overwritten on the screen.
	// Otherwise, the tail remains in place, the head moves by one,
	// and the rendered length is increased.
	if (m_drawn == m_worm.size()) {
		scr.PrintString(m_worm[0].X, m_worm[0].Y, " ");
	}
	else {
		m_drawn++;
	}

	// shift coordinates in the vector without drawing
	for (unsigned int i = 1; i < m_worm.size(); i++) {
		m_worm[i - 1] = m_worm[i];
	}

	m_worm[m_worm.size() - 1].X += delta.X;       // head coordinate is incremented
	m_worm[m_worm.size() - 1].Y += delta.Y;       // head coordinate is incremented

	// select a symbol for drawing the head depending on the direction of movement
	if (delta.X < 0) {
		m_head_mark = '<';
	}
	else if (delta.X > 0) {
		m_head_mark = '>';
	}
	else if (delta.Y < 0) {
		m_head_mark = 'A';
	}
	else /*(delta.Y > 0)*/ {
		m_head_mark = 'V';
	}

	// if you chose simple mode, then allow the snake to pass through the borders of the map
	if (mode_number == 49) {
		for (unsigned int i = 0; i < m_worm.size(); i++) {
			if (m_worm[i].X == 0) {
				m_worm[i].X = scr.GetWidth() - 2;
			}
			else if (m_worm[i].X == scr.GetWidth() - 1) {
				m_worm[i].X = 1;
			}
			else if (m_worm[i].Y == 0) {
				m_worm[i].Y = scr.GetHeight() - 2;
			}
			else if (m_worm[i].Y == scr.GetHeight() - 1) {
				m_worm[i].Y = 1;
			}
		}
	}
}

void Snake::Grow(const COORD& pos, uint8_t growbits) {
	for (uint8_t i = 0; i < growbits; ++i) {
		m_worm.insert(m_worm.begin(), pos);
	}
}

bool Snake::IsInSnakeBody(const COORD& pos) {
	for (uint8_t i = 0; i < m_worm.size() - 1; i++) {
		if (m_worm[i].X == pos.X && m_worm[i].Y == pos.Y) {
			return true;
		}
	}

	return false;
}

COORD Snake::GetSnakeHeadCoordinate() {
	return m_worm[m_worm.size() - 1];
}

uint16_t Snake::GetSnakeSize() {
	return m_worm.size();
}