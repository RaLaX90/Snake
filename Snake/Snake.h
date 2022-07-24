#pragma once

#include <vector>

#include "Coord.h"
#include "Screen.h"

using namespace std;

typedef vector<Coord> CoordVector;

class Snake {
public:
    Snake();
    void reset(Coord start_pos);                   // "�����" ����
    void draw(Screen& scr);                        // ��������� ��������� ���� �� ������
    void move(const Coord& delta, Screen& scr);   // ������������ ���� �� ���������� ���������
    void grow(const Coord& pos, int growbits);     // ���������� ����� ����
    bool into(const Coord& pos);                   // �������� ��������� ���������� � ���� ����
    Coord head();                                  // ����� ���������� ���������� ������ ����
    int size();                                     // ����� ���������� ����� ����

private:
    CoordVector worm;       // ������ ��������� ��������� ���� ����
    char head_mark;         // ������, ������� �������������� ������ ����
    unsigned int drawn;     // ����� ������������� ���� ����
};