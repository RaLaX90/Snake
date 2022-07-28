#pragma once

#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

class Screen {
public:
	Screen(int _width, int _height);
	~Screen();
	void SetCursorShow(bool visible);                               // show/hide cursor
	void SetTextAttribute(WORD attr);                               // set text/background color
	void SetCursorPosition(int position_x, int position_y);         // cursor positioning
	void PrintString(int position_x, int position_y, string text);  // character output
	void ClearScreen();												// screen cleaning

	int getWidth();													// get screenn width
	int getHeight();												// get screenn height
private:

	int width, height;												// playing field width and height
	HANDLE m_console_handle;										// console handle
	CONSOLE_CURSOR_INFO m_old_cursor_info, m_current_cursor_info;	// old and new cursor info objects
	WORD m_old_text_attribute;										// old text attribute
};