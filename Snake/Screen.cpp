#include "Screen.h"

// _width - playing field width (x)
// _height - height of the playing field (y)
Screen::Screen(int _width, int _height) {

	if (_width == 0 && _height == 0) {
		CONSOLE_SCREEN_BUFFER_INFO pcsbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &pcsbi);

		width = pcsbi.dwMaximumWindowSize.X;
		height = pcsbi.dwMaximumWindowSize.Y / 2 - 2;
	}
	else {
		width = _width;
		height = _height;
	}

	m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (m_console_handle == INVALID_HANDLE_VALUE) {
		throw "Failed GetStdHandle(): INVALID_HANDLE_VALUE";
	}

	if (!GetConsoleCursorInfo(m_console_handle, &m_old_cursor_info)) {
		throw "Failed GetConsoleCursorInfo()";
	}

	m_current_cursor_info.dwSize = m_old_cursor_info.dwSize;
	m_current_cursor_info.bVisible = m_old_cursor_info.bVisible;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(m_console_handle, &csbi);
	m_old_text_attribute = csbi.wAttributes;
}

Screen::~Screen() {
	SetConsoleCursorInfo(m_console_handle, &m_old_cursor_info);
	SetConsoleTextAttribute(m_console_handle, m_old_text_attribute);
}

void Screen::SetCursorShow(bool visible) {
	m_current_cursor_info.bVisible = visible;
	if (!SetConsoleCursorInfo(m_console_handle, &m_current_cursor_info)) {
		throw "Failed SetConsoleCursorInfo()";
	}
}

void Screen::SetTextAttribute(WORD attr) {
	SetConsoleTextAttribute(m_console_handle, attr);
}

void Screen::SetCursorPosition(int position_x, int position_y)
{
	COORD pos = { position_x, position_y };
	SetConsoleCursorPosition(m_console_handle, pos);
}

void Screen::PrintString(int position_x, int position_y, string text)
{
	SetCursorPosition(position_x, position_y);
	cout << text;
}

void Screen::ClearScreen() {
	system("cls");
}

int Screen::getWidth()
{
	return width;
}

int Screen::getHeight()
{
	return height;
}
