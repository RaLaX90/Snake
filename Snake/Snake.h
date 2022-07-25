#pragma once

#include <vector>

//#include "Coord.h"
#include "Screen.h"

using namespace std;

//typedef vector<COORD> CoordVector;

class Snake {
public:
    Snake();
    void reset(COORD start_pos);                   // "сброс" змеи
    void draw(Screen& scr);                        // первичная отрисовка змеи на экране
    void move(const COORD& delta, Screen& scr);   // передвижение змеи на приращение координат
    void grow(const COORD& pos, int growbits);     // увеличение длины змеи
    bool into(const COORD& pos);                   // проверка попадания координаты в тело змеи
    COORD head();                                  // метод возвращает координаты головы змеи
    int size();                                     // метод возвращает длину змеи

private:
    vector<COORD> worm;       // вектор координат сегментов тела змеи
    char head_mark;         // символ, которым отрисовывается голова змеи
    unsigned int drawn;     // длина отрисованного тела змеи
};