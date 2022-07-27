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
	void SetCursorShow(bool visible);                               // ��������/������ ������
	void SetTextAttribute(WORD attr);                               // ���������� ���� ������/����
	void SetCursorPosition(int position_x, int position_y);         // ���������������� �������
	void PrintString(int position_x, int position_y, string text);  // ����� �������
	void ClearScreen();                                             // ������� ������

private:
	HANDLE m_console_handle;
	CONSOLE_CURSOR_INFO m_old_cursor_info, m_current_cursor_info;
	WORD m_old_text_attribute;
};