#pragma once
#include <windows.h>

class Screen {
public:
    Screen();
    ~Screen();
    void cursor_show(bool visible);                     // ��������/������ ������
    void text_attr(WORD attr);                          // ���������� ���� ������/����
    void set_cursor(int position_x, int position_y);
    void print_console(int x, int y, char ch);       // ���������������� ������� �
                                                        // ����� �������, ���� ch != 0
    //void write_string(int x, int y, const char* str);   // ���������������� �������
                                                        // � ����� ������
    void cls();                                         // ������� ������

private:
    HANDLE hConsoleOutput;
    CONSOLE_CURSOR_INFO oldCursorInfo, curCursorInfo;
    WORD oldTextAttr;
};