#include "Screen.h"

// _width - playing field m_width (x)
// _height - m_height of the playing field (y)
Screen::Screen(short _width, short _height) :
	m_width(_width), m_height(_height) {

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

void Screen::SetCursorShow(bool _visible) {
	m_current_cursor_info.bVisible = _visible;
	if (!SetConsoleCursorInfo(m_console_handle, &m_current_cursor_info)) {
		throw "Failed SetConsoleCursorInfo()";
	}
}

void Screen::SetTextAttribute(WORD _attr) {
	SetConsoleTextAttribute(m_console_handle, _attr);
}

void Screen::SetCursorPosition(short _position_X, short _position_Y)
{
	SetConsoleCursorPosition(m_console_handle, COORD{ _position_X ,_position_Y });
}

void Screen::PrintString(short _position_X, short _position_Y, string _text)
{
	SetCursorPosition(_position_X, _position_Y);
	cout << _text;
}

void Screen::ClearScreen() {
	system("cls");
}

short Screen::GetWidth()
{
	return m_width;
}

short Screen::GetHeight()
{
	return m_height;
}
