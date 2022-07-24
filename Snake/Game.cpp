#include "Game.h"

#include <iostream>
#include <cstring>
#include <conio.h>

// ��������� ������ ��� �������������� ���������� ����
//const char* recordFormatStr = "%-15s  %9.4f  %4u  %7.2f  %s";
//
//Record::Record() {
//    name[0] = '\0';
//    rating = 0.0;
//    length = 0;
//    game_time = 0;
//    date = static_cast<time_t>(0);
//}
//
//void Record::as_string(char* buffer) {
//    sprintf(buffer, recordFormatStr, name, rating, length, game_time, ctime(&date));
//}
//
//ostream& operator << (ostream& os, const Record& rec) {
//    os
//        << rec.rating << ' '
//        << rec.length << ' '
//        << rec.game_time << ' '
//        << rec.date << ' '
//        << rec.name << endl;
//    return os;
//}
//
//istream& operator >> (istream& is, Record& rec) {
//    is
//        >> rec.rating
//        >> rec.length
//        >> rec.game_time
//        >> rec.date;
//    is.ignore(1);
//    is.getline(&rec.name[0], 16);
//    return is;
//}
//
//// ������� ��������� ����������� �� ��������.
//// ���������� ��� ������ qsort() ��� ���������� �� ��������.
//int rec_compare(const void* _op1, const void* _op2) {
//    const Record* op1 = reinterpret_cast<const Record*>(_op1);
//    const Record* op2 = reinterpret_cast<const Record*>(_op2);
//    return static_cast<int>(op2->rating - op1->rating);
//}

/*----- end of struct SRecord -------------------------------------*/


// ������� ������ ����������
void clearkeys() {
	while (_kbhit())
		_getch();
}

// �����������
// _scr     - ������, �������������� ����� �� �������
// _width   - ������ �������� ���� (x)
// _height  - ������ �������� ���� (y)
// _latency - �������� ����� ���������� ������� � �������������

Game::Game(Screen& _scr, int _width = 0, int _height = 0, int _latency) :
	scr(_scr), latency(_latency) {

	if (_width == 0 && _height == 0) {
		PCONSOLE_SCREEN_BUFFER_INFO pcsbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), pcsbi);

		width = pcsbi->dwSize.X;
		height = pcsbi->dwSize.Y;
	}

	srand(static_cast<unsigned int>(time(NULL)));

	duration_game = 0;
	rating = 0.0;

	// ������������� ������� ������ ���������� �����
	cmd_table[0] = CmdPair(27, CMD_EXIT);   // escape key
	cmd_table[1] = CmdPair('K', CMD_LEFT);  // ������� �����
	cmd_table[2] = CmdPair('M', CMD_RIGHT); // ������� ������
	cmd_table[3] = CmdPair('H', CMD_UP);    // ������� �����
	cmd_table[4] = CmdPair('P', CMD_DOWN);  // ������� ����
}

Game::Command Game::get_command() {
	int ch;

	ch = _getch();
	if (ch == 0 || ch == 0xe0) {
		ch = _getch();
	}

	for (int i = 0; i < 5; i++) {
		if (cmd_table[i].first == ch) {
			return cmd_table[i].second;
		}
	}
	return CMD_NOCOMMAND;
}

// ���������� ��� ����������� ��������� �������.
// �����������: ���������� �� ������ �������� � ���� ����.
Coord Game::make_food() {
	Coord food;
	do {
		food.x = rand() % (width - 2) + 1;
		food.y = rand() % (height - 2) + 1;
	} while (snake.into(food));

	return food;
}


const char BORDER = '#';    // ������ ��� ��������� ����� �������� ����


void Game::draw_field() {

	scr.cls();

	for (int i = 0; i < height; i++) {
		if (i == 0 || i == height - 1) {
			for (int j = 0; j < width; j++)
				scr.print_console(j, i, BORDER);
		}
		else {
			scr.print_console(0, i, BORDER);
			scr.print_console(width - 1, i, BORDER);
		}
	}
	scr.print_console(0, height);
	_cprintf("Length: ****  Rating: ****.**** (****.****)  Time: ****.**");
}


void Game::print_stat() {
	scr.print_console(8, height);
	_cprintf("%04u", snake.size());
	scr.print_console(22, height);
	_cprintf("%09.4f", rating);
	scr.print_console(33, height);
	_cprintf("%09.4f", rating_i);
	scr.print_console(51, height);
	_cprintf("%07.2f", duration_game);
}

//void Game::top10_table() {
//	scr.cls();
//	char buf[80];
//
//	scr.pos_str(width / 2 - 12, 2, "***** T O P    1 0 *****");
//	scr.pos_str(5, 4, "Name              Rating    Length  Time   Date");
//
//
//	for (int i = 0; i < 10; i++) {
//		ttop10[i].as_string(buf);
//		scr.pos_str(5, 5 + i, buf);
//	}
//}

//void Game::top10(bool after_game) {
//
//	char buf[80];
//	char buf_encoded[NAMELENGTH];
//
//	top10_table();      // �������� ������� 10 ������ �����������
//
//	time_t date = time(NULL);
//	if (after_game) {
//		// ���� ���� ���� �������, �� �������� ������� ���������
//		scr.pos(5, 16);
//		_cprintf(recordFormatStr, "Your result", rating, snake.size(), duration_game, ctime(&date));
//	}
//
//	if (rating > ttop10[9].rating) {    // ���� ������� ���� ������, ��� ������� �� 10 ������...
//		// ��������� ��� ������
//		scr.pos_str(5, 20, "Your name: _");
//		scr.pos(16, 20);
//		cin.getline(&buf[0], NAMELENGTH);
//		clearkeys();
//		OemToCharBuff(buf, buf_encoded, static_cast<DWORD>(NAMELENGTH));
//		// �������� ��������� ������ � ������� 10 ������ �����������
//		strcpy(ttop10[9].name, buf_encoded);
//		ttop10[9].date = date;
//		ttop10[9].game_time = duration_game;
//		ttop10[9].length = snake.size();
//		ttop10[9].rating = rating;
//		// ������������� ���������� �� ��������
//		qsort(ttop10, 10, sizeof(SRecord), rec_compare);
//		// �������� ������� �� ������
//		top10_table();
//
//		// �������� ���� � 10 ������� ������������
//		write_top10();
//	}
//}

void Game::wait_to_click(int y) {
	scr.write_string(width / 2 - 15, y, "Press any key for continue...");
	_getch();
	clearkeys();
}

bool Game::once_more() {
	scr.write_string(width / 2 - 12, height - 3, "O n c e    m o r e ?");

	int ch = _getch();
	clearkeys();
	if (ch == 'N' || ch == 'n' || ch == 27)
		return false;
	return true;
}

//const char* top10_filename = "snake.dat";   // ��� ����� ��� �������� 10 ������ �����������

const char* ver_number = "v 1.1";
const char* copyright = "(c) Cranium, 2014.";

void Game::logo() {
	scr.write_string(width / 2 - 9, 10, "O l d s c h o o l");
	scr.write_string(width / 2 - 7, 12, "S  N  A  K  E");
	scr.write_string(width / 2 - 3, 16, ver_number);
	scr.write_string(width / 2 - 9, height, copyright);
	wait_to_click(22);
}

void Game::goodbye() {
	scr.cls();
	_cprintf("Oldschool Snake %s\n%s\n", ver_number, copyright);
}

const char FOOD = '$';      // ������ ��� ������ ���

void Game::game_loop() {

	duration_game = 0;
	rating = rating_i = 0.0;

	draw_field();           // ���������� ������� ����

	snake.reset(Coord(width / 2, height / 2));     // ���������� ����: ����� 2,
													// ��������� - � �������� �������� ����,
													// ����������� - �����
	Command cmd = CMD_NOCOMMAND;
	State stt = STATE_OK;
	// delta  �������� ���������� ��������� (dx, dy) ��� ������� ����������� ���� �� ����
	SCoord delta(-1, 0);                // ��������� �������� - �����
	SCoord food = make_food();          // ��������� ���������� ���
	scr.print_console(food.x, food.y, FOOD);      // ������� ��� �� �����

	snake.draw(scr);                    // ��������� ��������� ����

	print_stat();                       // ������� ��������� ���������� ����

	clock_t time1, time2, duration;
	time1 = clock();

	do {

		if (_kbhit())                   // ���� � ������ ���������� ���� ����������,
			cmd = get_command();        // �� ������� �������

		// ��������� ������
		switch (cmd) {
		case CMD_LEFT:
			delta = SCoord(-1, 0);
			break;
		case CMD_RIGHT:
			delta = SCoord(1, 0);
			break;
		case CMD_UP:
			delta = SCoord(0, -1);
			break;
		case CMD_DOWN:
			delta = SCoord(0, 1);
			break;
		case CMD_EXIT:
			stt = STATE_EXIT;
		default:
			break;
		};

		SCoord hd = snake.head();       // ���������� ������ ����
		hd += delta;                    // ���������� ������ ���� ����� ���������� (��������� �������)
		// ���� ������ ���� ����������� � �������� ���� ��� �� ����� ����, �� ���� �������
		if (hd.x == 0 || hd.x == width - 1 || hd.y == 0 || hd.y == height - 1 || snake.into(hd))
			stt = STATE_DIED;

		if (stt == STATE_OK) {          // ���� ���� ��� ����, ��
			snake.move(delta, scr);     // �������� ���� �� delta

			if (snake.head() == food) { // ���� ���������� ������ ���� ��������� � ����������� ���, ��
				snake.grow(food, 3);    // ��������� ����� ����
				food = make_food();     // ��������� ���������� ����� ���
				scr.print_console(food.x, food.y, FOOD); // ������� ��� �� �����

				// ���������� ������� ����, ���������� � ������ ��������.
				// ��������� ������� ����������� ��� ����� ����, ������� �� ����� � ��������,
				// ����������� �� ������ � ��� (����� �� �������� ���������� ��� �� �������� ���������).
				// ����� �������, ��� ���� ���� ��� � ��� ��� �������, ��� ���� ��������� �������.
				time2 = clock();
				duration = time2 - time1;
				duration_game += static_cast<double>(duration) / CLOCKS_PER_SEC;
				rating_i = static_cast<double>(snake.size()) / duration * CLOCKS_PER_SEC;
				rating += rating_i;     // ����� ������� - ����� ��������� ��������� �� ����
				time1 = time2;

				print_stat();           // ����� ������� ���������� ����
			}

			Sleep(latency);             // �������� ����� ��������� ���������� �������
		}

	} while (stt == STATE_OK);          // ������, ���� ���� ����

	scr.write_string(width / 2 - 8, 10, " G a m e    o v e r ");
	clearkeys();
	_getch();
	clearkeys();
}