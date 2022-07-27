#pragma once

#include <vector>
//#include <conio.h>
#include "Screen.h"

using namespace std;

class Snake {
public:
	Snake();
	~Snake();
	void Reset(COORD start_pos);                   // "�����" ����
	void Draw(Screen& scr);                        // ��������� ��������� ���� �� ������
	void Move(const COORD& delta, Screen& scr);   // ������������ ���� �� ���������� ���������
	void Grow(const COORD& pos, int growbits);     // ���������� ����� ����
	bool IsInSnakeBody(const COORD& pos);                   // �������� ��������� ���������� � ���� ����
	COORD GetSnakeHeadCoordinate();                                  // ����� ���������� ���������� ������ ����
	int GetSnakeSize();                                     // ����� ���������� ����� ����

private:
	vector<COORD> m_worm;       // ������ ��������� ��������� ���� ����
	string m_head_mark;         // ������, ������� �������������� ������ ����
	unsigned int m_drawn;     // ����� ������������� ���� ����
};