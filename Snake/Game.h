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
    void game_loop();           // основной цикл игры
    bool once_more();           // вывод запроса и приём ответа от игрока
    void wait_to_click(int position_y);  // "Press any key for continue..."
    void logo();                // вывод заставки игры
    void goodbye();             // вывод копирайта по окончании игры

private:
    enum Command { CMD_NOCOMMAND = 0, CMD_EXIT, CMD_LEFT, CMD_RIGHT, CMD_UP, CMD_DOWN };
    enum State { STATE_OK, STATE_EXIT, STATE_DIED };

    typedef pair<int, Command> CmdPair;

    int width, height;      // ширина и высота игрового поля
    int latency;            // задержка между изменением позиции в миллисекундах
    Screen screen;            // подсистема визуализации
    Snake snake;           // змейка
    double duration_game;   // длительность игры
    //double rating, rating_i; // рейтинг итоговый и частичный

    //Record ttop10[10];     // таблица 10 лучших результатов

    CmdPair cmd_table[5];   // таблица команд управления игрой

    void draw_field();      // прорисовка игрового поля
    COORD make_food();     // вычисление позиции для еды
    void print_stat();      // вывод текущей статистики ниже игрового поля
    Command get_command();  // приём команды с клавиатуры
};