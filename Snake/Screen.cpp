#include "Screen.h"
#include <conio.h>


using namespace std;

Screen::Screen() {
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsoleOutput == INVALID_HANDLE_VALUE)
        throw "Failed GetStdHandle(): INVALID_HANDLE_VALUE";    // "INVALID_HANDLE_VALUE"

    if (!GetConsoleCursorInfo(hConsoleOutput, &oldCursorInfo))
        throw ""Failed GetConsoleCursorInfo()";
    curCursorInfo.dwSize = oldCursorInfo.dwSize;
    curCursorInfo.bVisible = oldCursorInfo.bVisible;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);
    oldTextAttr = csbi.wAttributes;
}

Screen::~Screen() {
    SetConsoleCursorInfo(hConsoleOutput, &oldCursorInfo);
    SetConsoleTextAttribute(hConsoleOutput, oldTextAttr);
}

void Screen::cursor_show(bool visible) {
    curCursorInfo.bVisible = visible;
    if (!SetConsoleCursorInfo(hConsoleOutput, &curCursorInfo))
        throw "Failed SetConsoleCursorInfo()";
}

void Screen::text_attr(WORD attr) {
    SetConsoleTextAttribute(hConsoleOutput, attr);
}

void Screen::set_cursor(int position_x, int position_y)
{
    COORD pos = { position_x, position_y };
    SetConsoleCursorPosition(hConsoleOutput, pos);
}


void Screen::print_console(int x, int y, char ch) {
    /*COORD point;
    point.X = static_cast<SHORT>(x);
    point.Y = static_cast<SHORT>(y);
    if (!SetConsoleCursorPosition(hConsoleOutput, point))
        throw "Failed SetConsoleCursorPosition()";
    if (ch > 0)
        _putch(ch);*/
    set_cursor(x, y);
    _cprintf_s("%s", ch);
}

//void Screen::write_string(int x, int y, const char* str) {
//    set_cursor(x, y);
//    //write_symbol(x, y);
//    //COORD pos = { x, y };
//    //SetConsoleCursorPosition(hConsoleOutput, pos);
//    //WriteConsole(hConsoleOutput, "Hello", 5, NULL, NULL);
//    _cprintf_s("%s", str);
//}

void Screen::cls() {
    system("cls");
}