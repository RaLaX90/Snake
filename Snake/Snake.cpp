#include "Snake.h"

string SNAKE_TAIL = "@";        // символ для отрисовки сегментов тела змеи, кроме головы

Snake::Snake() {
	m_head_mark = '<';
}

Snake::~Snake()
{
}

void Snake::Reset(COORD start_pos) {
	m_worm.clear();
	m_worm.reserve(1000);         // зарезервировть память
	m_worm.push_back(start_pos);  // добавить координаты головы
	m_worm.push_back(start_pos);  // добавить координаты хвоста
	m_worm[0].X++;                // координата x хвоста - на 1 правее
}

void Snake::Draw(Screen& scr) {
	unsigned int wsize = m_worm.size() - 1;

	for (unsigned int i = 0; i < wsize; i++) {
		scr.PrintString(m_worm[i].X, m_worm[i].Y, SNAKE_TAIL);
	}

	scr.PrintString(m_worm[wsize].X, m_worm[wsize].Y, m_head_mark);
	m_drawn = m_worm.size();
}

void Snake::Move(const COORD& delta, Screen& scr) {
	// При перемещении змеи перерисовывается только положение головы (и первого сегмента)
	// и положение хвоста. Остальные сегменты змеи не перерисовываются.

	// Перерисовка хвоста.
	// Длина змеи увеличивается, когда змея растёт (метод grow()),
	// но змея на экране не изменяется. Поэтому, если отрисованная длина змеи
	// совпадает с реальной длиной, то на экране затирается последний сегмент змеи (хвост).
	// В противном случае, хвост остаётся на месте, голова сдвигается на единицу,
	// а отрисованная длина увеличивается.
	if (m_drawn == m_worm.size()) {
		scr.PrintString(m_worm[0].X, m_worm[0].Y, " ");
	}
	else {
		m_drawn++;
	}

	// сдвиг координат в векторе без отрисовки
	for (unsigned int i = 1; i < m_worm.size(); i++) {
		m_worm[i - 1] = m_worm[i];
	}

	m_worm[m_worm.size() - 1].X += delta.X;       // координата головы изменяется на приращение
	m_worm[m_worm.size() - 1].Y += delta.Y;       // координата головы изменяется на приращение

	// выбор символа для отрисовки головы в зависимости от направления движения
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

	// Перерисовка головы и первого сегмента змеи.
	scr.PrintString(m_worm[m_worm.size() - 1].X, m_worm[m_worm.size() - 1].Y, m_head_mark);
	scr.PrintString(m_worm[m_worm.size() - 2].X, m_worm[m_worm.size() - 2].Y, SNAKE_TAIL);
}

void Snake::Grow(const COORD& pos, int growbits) {
	for (int i = 0; i < growbits; ++i) {
		m_worm.insert(m_worm.begin(), pos);
	}
}

bool Snake::IsInSnakeBody(const COORD& pos) {
	for (unsigned int i = 0; i < m_worm.size(); i++) {
		if (m_worm[i].X == pos.X && m_worm[i].Y == pos.Y) {
			return true;
		}
	}

	return false;
}

COORD Snake::GetSnakeHeadCoordinate() {
	return m_worm[m_worm.size() - 1];
}

int Snake::GetSnakeSize() {
	return static_cast<int>(m_worm.size());
}