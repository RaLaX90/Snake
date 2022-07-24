#include "Snake.h"
#include <conio.h>

const char SNAKE_TAIL = '@';        // ������ ��� ��������� ��������� ���� ����, ����� ������

Snake::Snake() {
	head_mark = '<';
}

void Snake::reset(Coord start_pos) {
	worm.clear();
	worm.reserve(1000);         // �������������� ������
	worm.push_back(start_pos);  // �������� ���������� ������
	worm.push_back(start_pos);  // �������� ���������� ������
	worm[0].x++;                // ���������� x ������ - �� 1 ������
}

void Snake::draw(Screen& scr) {
	unsigned int wsize = worm.size() - 1;
	for (unsigned int i = 0; i < wsize; i++)
		scr.print_console(worm[i].x, worm[i].y, SNAKE_TAIL);
	scr.print_console(worm[wsize].x, worm[wsize].y, head_mark);
	drawn = worm.size();
}

void Snake::move(const Coord& delta, Screen& scr) {
	// ��� ����������� ���� ���������������� ������ ��������� ������ (� ������� ��������)
	// � ��������� ������. ��������� �������� ���� �� ����������������.

	// ����������� ������.
	// ����� ���� �������������, ����� ���� ����� (����� grow()),
	// �� ���� �� ������ �� ����������. �������, ���� ������������ ����� ����
	// ��������� � �������� ������, �� �� ������ ���������� ��������� ������� ���� (�����).
	// � ��������� ������, ����� ������� �� �����, ������ ���������� �� �������,
	// � ������������ ����� �������������.
	if (drawn == worm.size())
		scr.print_console(worm[0].x, worm[0].y, ' ');
	else
		drawn++;

	// ����� ��������� � ������� ��� ���������
	for (unsigned int i = 1; i < worm.size(); i++)
		worm[i - 1] = worm[i];

	worm[worm.size() - 1] += delta;       // ���������� ������ ���������� �� ����������

	// ����� ������� ��� ��������� ������ � ����������� �� ����������� ��������
	if (delta.x < 0)
		head_mark = '<';
	else if (delta.x > 0)
		head_mark = '>';
	else if (delta.y < 0)
		head_mark = 'A';
	else // (delta.y > 0)
		head_mark = 'V';

	// ����������� ������ � ������� �������� ����.
	scr.print_console(worm[worm.size() - 1].x, worm[worm.size() - 1].y, head_mark);
	scr.print_console(worm[worm.size() - 2].x, worm[worm.size() - 2].y, SNAKE_TAIL);
}

void Snake::grow(const Coord& pos, int growbits) {
	for (int i = 0; i < growbits; ++i)
		worm.insert(worm.begin(), pos);
}

bool Snake::into(const Coord& pos) {
	for (unsigned int i = 0; i < worm.size(); i++)
		if (worm[i].x == pos.x && worm[i].y == pos.y)
			return true;
	return false;
}

Coord Snake::head() {
	return worm[worm.size() - 1];
}

int Snake::size() {
	return static_cast<int>(worm.size());
}