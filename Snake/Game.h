#pragma once

#include <conio.h>
#include "Snake.h"
#include <random>

using namespace std;

class Game {
public:
	Game(Screen& _scr, int _width = 0, int _height = 0, int _latency = 100);
	void StartGameLoop();           // �������� ���� ����
	bool IsOnceMore();           // ����� ������� � ���� ������ �� ������
	void WaitForClick(int position_x,int position_y);  // "Press any key for continue..."
	void PrintLogo();                // ����� �������� ����
	void EndGame();             // ����� ��������� �� ��������� ����

private:
	enum Command { CMD_NOCOMMAND = 0, CMD_EXIT, CMD_LEFT, CMD_RIGHT, CMD_UP, CMD_DOWN };
	enum State { STATE_OK = 0, STATE_EXIT, STATE_DIED };

	pair<int, Command> cmd_table[5];   // ������� ������ ���������� �����

	int width, height;      // ������ � ������ �������� ����
	int latency;            // �������� ����� ���������� ������� � �������������
	Screen screen;            // ���������� ������������
	Snake snake;           // ������
	double duration_game;   // ������������ ����

	random_device m_rd;
	mt19937 m_generator;
	uniform_int_distribution<int> m_distribution_width;
	uniform_int_distribution<int> m_distribution_height;

	void drawGameField();      // ���������� �������� ����
	COORD createFood();     // ���������� ������� ��� ���
	void printStatistics();      // ����� ������� ���������� ���� �������� ����
	Command getCommand();  // ���� ������� � ����������
	void clearkeys();       // ������� ������ ����������
};