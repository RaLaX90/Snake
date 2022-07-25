#pragma once
#pragma once

#include <time.h>
#include <fstream>
#include <utility>

#include "Screen.h"
#include "Snake.h"

using namespace std;

class Game {
public:
    Game(Screen& _scr, int _width = 0, int _height = 0, int _latency = 100);
    void game_loop();           // �������� ���� ����
    bool once_more();           // ����� ������� � ���� ������ �� ������
    void wait_to_click(int position_y);  // "Press any key for continue..."
    void logo();                // ����� �������� ����
    void goodbye();             // ����� ��������� �� ��������� ����

private:
    enum Command { CMD_NOCOMMAND = 0, CMD_EXIT, CMD_LEFT, CMD_RIGHT, CMD_UP, CMD_DOWN };
    enum State { STATE_OK, STATE_EXIT, STATE_DIED };

    typedef pair<int, Command> CmdPair;

    int width, height;      // ������ � ������ �������� ����
    int latency;            // �������� ����� ���������� ������� � �������������
    Screen screen;            // ���������� ������������
    Snake snake;           // ������
    double duration_game;   // ������������ ����
    //double rating, rating_i; // ������� �������� � ���������

    //Record ttop10[10];     // ������� 10 ������ �����������

    CmdPair cmd_table[5];   // ������� ������ ���������� �����

    void draw_field();      // ���������� �������� ����
    COORD make_food();     // ���������� ������� ��� ���
    void print_stat();      // ����� ������� ���������� ���� �������� ����
    Command get_command();  // ���� ������� � ����������
};