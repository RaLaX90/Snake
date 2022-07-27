#pragma once

#include <windows.h>
#include <string>
//#include <conio.h>
#include <iostream>

using namespace std;

class Screen {
public:
	Screen();
	~Screen();
	void SetCursorShow(bool visible);                               // показать/скрыть курсор
	void SetTextAttribute(WORD attr);                               // установить цвет текста/фона
	void SetCursorPosition(int position_x, int position_y);         // позиционирование курсора
	void PrintString(int position_x, int position_y, string text);  // вывод символа
	void ClearScreen();                                             // очистка экрана

private:
	HANDLE m_console_handle;
	CONSOLE_CURSOR_INFO m_old_cursor_info, m_current_cursor_info;
	WORD m_old_text_attribute;
};