#pragma once

#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

class Screen {
public:
	Screen(short _width, short _height);
	~Screen();
	void SetCursorShow(bool _visible);										// show/hide cursor
	void SetTextAttribute(WORD _attr);										// set _text/background color
	void SetCursorPosition(short _position_X, short _position_Y);			// cursor positioning
	void PrintString(short _position_X, short _position_Y, string _text);	// character output
	void ClearScreen();														// screen cleaning

	short GetWidth();														// get screenn m_width
	short GetHeight();														// get screenn m_height
private:

	short m_width, m_height;												// playing field m_width and m_height
	HANDLE m_console_handle;												// console handle
	CONSOLE_CURSOR_INFO m_old_cursor_info, m_current_cursor_info;			// old and new cursor info objects
	WORD m_old_text_attribute;												// old _text attribute
};