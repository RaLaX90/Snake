#pragma once

#include <vector>
//#include <conio.h>
#include "Screen.h"

using namespace std;

class Snake {
public:
	Snake();
	~Snake();
	void Reset(COORD start_pos);                   // "сброс" змеи
	void Draw(Screen& scr);                        // первичная отрисовка змеи на экране
	void Move(const COORD& delta, Screen& scr);   // передвижение змеи на приращение координат
	void Grow(const COORD& pos, int growbits);     // увеличение длины змеи
	bool IsInSnakeBody(const COORD& pos);                   // проверка попадания координаты в тело змеи
	COORD GetSnakeHeadCoordinate();                                  // метод возвращает координаты головы змеи
	int GetSnakeSize();                                     // метод возвращает длину змеи

private:
	vector<COORD> m_worm;       // вектор координат сегментов тела змеи
	string m_head_mark;         // символ, которым отрисовывается голова змеи
	unsigned int m_drawn;     // длина отрисованного тела змеи
};