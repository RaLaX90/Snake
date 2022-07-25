#include "Snake.h"
#include <conio.h>

const char SNAKE_TAIL = '@';        // ������ ��� ��������� ��������� ���� ����, ����� ������

Snake::Snake() {
	head_mark = '<';
}

void Snake::reset(COORD start_pos) {
	worm.clear();
	worm.reserve(1000);         // �������������� ������
	worm.push_back(start_pos);  // �������� ���������� ������
	worm.push_back(start_pos);  // �������� ���������� ������
	worm[0].X++;                // ���������� x ������ - �� 1 ������
}

void Snake::draw(Screen& scr) {
	unsigned int wsize = worm.size() - 1;

	for (unsigned int i = 0; i < wsize; i++) {
		scr.print_console_symbol(worm[i].X, worm[i].Y, SNAKE_TAIL);
	}

	scr.print_console_symbol(worm[wsize].X, worm[wsize].Y, head_mark);
	drawn = worm.size();
}

void Snake::move(const COORD& delta, Screen& scr) {
	// ��� ����������� ���� ���������������� ������ ��������� ������ (� ������� ��������)
	// � ��������� ������. ��������� �������� ���� �� ����������������.

	// ����������� ������.
	// ����� ���� �������������, ����� ���� ����� (����� grow()),
	// �� ���� �� ������ �� ����������. �������, ���� ������������ ����� ����
	// ��������� � �������� ������, �� �� ������ ���������� ��������� ������� ���� (�����).
	// � ��������� ������, ����� ������� �� �����, ������ ���������� �� �������,
	// � ������������ ����� �������������.
	if (drawn == worm.size()) {
		scr.print_console_symbol(worm[0].X, worm[0].Y, ' ');
	}
	else {
		drawn++;
	}

	// ����� ��������� � ������� ��� ���������
	for (unsigned int i = 1; i < worm.size(); i++) {
		worm[i - 1] = worm[i];
	}

	//worm[worm.size() - 1] += delta;       // ���������� ������ ���������� �� ����������
	worm.push_back(delta);

	// ����� ������� ��� ��������� ������ � ����������� �� ����������� ��������
	if (delta.X < 0) {
		head_mark = '<';
	}
	else if (delta.X > 0) {
		head_mark = '>';
	}
	else if (delta.Y < 0) {
		head_mark = 'A';
	}
	else /*(delta.Y > 0)*/{
		head_mark = 'V';
	}

	// ����������� ������ � ������� �������� ����.
	scr.print_console_symbol(worm[worm.size() - 1].X, worm[worm.size() - 1].Y, head_mark);
	scr.print_console_symbol(worm[worm.size() - 2].X, worm[worm.size() - 2].Y, SNAKE_TAIL);
}

void Snake::grow(const COORD& pos, int growbits) {
	for (int i = 0; i < growbits; ++i) {
		worm.insert(worm.begin(), pos);
	}
}

bool Snake::into(const COORD& pos) {
	for (unsigned int i = 0; i < worm.size(); i++) {
		if (worm[i].X == pos.X && worm[i].Y == pos.Y) {
			return true;
		}
	}

	return false;
}

COORD Snake::head() {
	return worm[worm.size() - 1];
}

int Snake::size() {
	return static_cast<int>(worm.size());
}