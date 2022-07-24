#pragma once
#include <windows.h>

class Screen {
public:
    Screen();
    ~Screen();
    void cursor_show(bool visible);                     // показать/скрыть курсор
    void text_attr(WORD attr);                          // установить цвет текста/фона
    void set_cursor(int position_x, int position_y);
    void print_console(int x, int y, char ch);       // позиционирование курсора и
                                                        // вывод символа, если ch != 0
    //void write_string(int x, int y, const char* str);   // позиционирование курсора
                                                        // и вывод строки
    void cls();                                         // очистка экрана

private:
    HANDLE hConsoleOutput;
    CONSOLE_CURSOR_INFO oldCursorInfo, curCursorInfo;
    WORD oldTextAttr;
};