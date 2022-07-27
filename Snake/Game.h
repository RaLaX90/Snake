#pragma once

#include <conio.h>
#include "Snake.h"
#include <random>

using namespace std;

class Game {
public:
	Game(Screen& _scr, int _width = 0, int _height = 0, int _latency = 100);
	void StartGameLoop();           // основной цикл игры
	bool IsOnceMore();           // вывод запроса и приём ответа от игрока
	void WaitForClick(int position_x,int position_y);  // "Press any key for continue..."
	void PrintLogo();                // вывод заставки игры
	void EndGame();             // вывод копирайта по окончании игры

private:
	enum Command { CMD_NOCOMMAND = 0, CMD_EXIT, CMD_LEFT, CMD_RIGHT, CMD_UP, CMD_DOWN };
	enum State { STATE_OK = 0, STATE_EXIT, STATE_DIED };

	pair<int, Command> cmd_table[5];   // таблица команд управления игрой

	int width, height;      // ширина и высота игрового поля
	int latency;            // задержка между изменением позиции в миллисекундах
	Screen screen;            // подсистема визуализации
	Snake snake;           // змейка
	double duration_game;   // длительность игры

	random_device m_rd;
	mt19937 m_generator;
	uniform_int_distribution<int> m_distribution_width;
	uniform_int_distribution<int> m_distribution_height;

	void drawGameField();      // прорисовка игрового поля
	COORD createFood();     // вычисление позиции для еды
	void printStatistics();      // вывод текущей статистики ниже игрового поля
	Command getCommand();  // приём команды с клавиатуры
	void clearkeys();       // очистка буфера клавиатуры
};