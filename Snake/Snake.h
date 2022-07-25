#pragma once

#include <vector>

//#include "Coord.h"
#include "Screen.h"

using namespace std;

//typedef vector<COORD> CoordVector;

class Snake {
public:
    Snake();
    void reset(COORD start_pos);                   // "�����" ����
    void draw(Screen& scr);                        // ��������� ��������� ���� �� ������
    void move(const COORD& delta, Screen& scr);   // ������������ ���� �� ���������� ���������
    void grow(const COORD& pos, int growbits);     // ���������� ����� ����
    bool into(const COORD& pos);                   // �������� ��������� ���������� � ���� ����
    COORD head();                                  // ����� ���������� ���������� ������ ����
    int size();                                     // ����� ���������� ����� ����

private:
    vector<COORD> worm;       // ������ ��������� ��������� ���� ����
    char head_mark;         // ������, ������� �������������� ������ ����
    unsigned int drawn;     // ����� ������������� ���� ����
};