#include "Game.h"

#include <iostream>
#include <cstring>
#include <conio.h>

// форматная строка для форматирования результата игры
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
//// Функция сравнения результатов по рейтингу.
//// Необходима для работы qsort() для сортировки по убыванию.
//int rec_compare(const void* _op1, const void* _op2) {
//    const Record* op1 = reinterpret_cast<const Record*>(_op1);
//    const Record* op2 = reinterpret_cast<const Record*>(_op2);
//    return static_cast<int>(op2->rating - op1->rating);
//}

/*----- end of struct SRecord -------------------------------------*/


// очистка буфера клавиатуры
void clearkeys() {
	while (_kbhit()) {
		_getch();
	}
}

// Конструктор
// _scr     - объект, осуществляющий вывод на консоль
// _width   - ширина игрового поля (x)
// _height  - высота игрового поля (y)
// _latency - задержка между изменением позиции в миллисекундах

Game::Game(Screen& _scr, int _width, int _height, int _latency) :
	screen(_scr), latency(_latency) {

	if (_width != 0 && _height != 0) {
		CONSOLE_SCREEN_BUFFER_INFO pcsbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &pcsbi);

		width = pcsbi.dwMaximumWindowSize.X;
		height = pcsbi.dwMaximumWindowSize.Y / 2 - 2;
	}

	srand(static_cast<unsigned int>(time(NULL)));

	duration_game = 0;
	//rating = 0.0;

	// инициализация таблицы команд управления игрой
	cmd_table[0] = CmdPair(27, CMD_EXIT);   // escape key
	cmd_table[1] = CmdPair('K', CMD_LEFT);  // стрелка влево
	cmd_table[2] = CmdPair('M', CMD_RIGHT); // стрелка вправо
	cmd_table[3] = CmdPair('H', CMD_UP);    // стрелка вверх
	cmd_table[4] = CmdPair('P', CMD_DOWN);  // стрелка вниз
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

// Координата еды вычисляется случайным образом.
// Ограничение: координата не должна попадать в тело змеи.
COORD Game::make_food() {
	COORD food;
	do {
		food.X = rand() % (width - 2) + 1;
		food.Y = rand() % (height - 2) + 1;
	} while (snake.into(food));

	return food;
}


const char BORDER = '#';    // символ для рисования рамки игрового поля


void Game::draw_field() {

	screen.cls();

	for (int i = 0; i < height; i++) {
		if (i == 0 || i == height - 1) {
			for (int j = 0; j < width; j++) {
				screen.print_console_symbol(j, i, BORDER);
			}
		}
		else {
			screen.print_console_symbol(0, i, BORDER);
			screen.print_console_symbol(width - 1, i, BORDER);
		}
	}
	screen.print_console_symbol(0, height, BORDER);
	_cprintf_s("Length: ****  Rating: ****.**** (****.****)  Time: ****.**");
}


void Game::print_stat() {
	//scr.print_console(8, height);
	//_cprintf("%04u", snake.size());
	//scr.print_console(22, height);
	//_cprintf("%09.4f", rating);
	//scr.print_console(33, height);
	//_cprintf("%09.4f", rating_i);
	//scr.print_console(, height);
	_cprintf_s("%07.2f", 51, height, duration_game);
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
//	top10_table();      // показать таблицу 10 лучших результатов
//
//	time_t date = time(NULL);
//	if (after_game) {
//		// если игра была сыграна, то показать текущий результат
//		scr.pos(5, 16);
//		_cprintf(recordFormatStr, "Your result", rating, snake.size(), duration_game, ctime(&date));
//	}
//
//	if (rating > ttop10[9].rating) {    // если рейтинг игры больше, чем меньший из 10 лучших...
//		// запросить имя игрока
//		scr.pos_str(5, 20, "Your name: _");
//		scr.pos(16, 20);
//		cin.getline(&buf[0], NAMELENGTH);
//		clearkeys();
//		OemToCharBuff(buf, buf_encoded, static_cast<DWORD>(NAMELENGTH));
//		// заменить последнюю запись в таблице 10 лучших результатов
//		strcpy(ttop10[9].name, buf_encoded);
//		ttop10[9].date = date;
//		ttop10[9].game_time = duration_game;
//		ttop10[9].length = snake.size();
//		ttop10[9].rating = rating;
//		// отсортировать результаты по убыванию
//		qsort(ttop10, 10, sizeof(SRecord), rec_compare);
//		// обновить таблицу на экране
//		top10_table();
//
//		// обновить файл с 10 лучшими результатами
//		write_top10();
//	}
//}

void Game::wait_to_click(int position_y) {
	screen.print_console_string(width / 2 - 15, position_y, "Press any key for continue...");
	_getch();
	clearkeys();
}

bool Game::once_more() {
	screen.print_console_string(width / 2 - 12, height - 3, "O n c e    m o r e ?");

	int ch = _getch();
	clearkeys();

	if (ch == 'N' || ch == 'n' || ch == 27) {
		return false;
	}
	return true;
}

//const char* top10_filename = "snake.dat";   // имя файла для хранения 10 лучших результатов

const char* ver_number = "v 1.1";
const char* copyright = "(c) Cranium, 2014.";

void Game::logo() {
	screen.print_console_string(width / 2 - 9, 10, "O l d s c h o o l");
	screen.print_console_string(width / 2 - 7, 12, "S  N  A  K  E");
	screen.print_console_string(width / 2 - 3, 16, ver_number);
	screen.print_console_string(width / 2 - 9, height, copyright);
	//wait_to_click(22);
}

void Game::goodbye() {
	screen.cls();
	_cprintf("Oldschool Snake %s\n%s\n", ver_number, copyright);
}

const char FOOD = '$';      // символ для вывода еды

void Game::game_loop() {

	duration_game = 0;
	//rating = rating_i = 0.0;

	draw_field();           // нарисовать игровое поле

	COORD debug{ width / 2, height / 2 };
	snake.reset(debug);     // установить змею: длина 2,
													// положение - в середине игрового поля,
													// направление - влево
	Command cmd = CMD_NOCOMMAND;
	State state = STATE_OK;
	// delta  содержит приращение координат (dx, dy) для каждого перемещения змеи по полю
	COORD delta{ -1, 0 };                // начальное движение - влево
	COORD food = make_food();          // вычислить координаты еды
	screen.print_console_symbol(food.X, food.Y, FOOD);      // вывести еду на экран

	snake.draw(screen);                    // первичное рисование змеи

	print_stat();                       // вывести начальную статистику игры

	clock_t time1, time2, duration;
	time1 = clock();

	do {

		if (_kbhit()) {					// если в буфере клавиатуры есть информация,
			cmd = get_command();        // то принять команду
		}

		// обработка команд
		switch (cmd) {
		case CMD_LEFT:
			delta = COORD{ -1, 0 };
			break;
		case CMD_RIGHT:
			delta = COORD{ 1, 0 };
			break;
		case CMD_UP:
			delta = COORD{ 0, -1 };
			break;
		case CMD_DOWN:
			delta = COORD{ 0, 1 };
			break;
		case CMD_EXIT:
			state = STATE_EXIT;
		default:
			break;
		};

		COORD snake_head_coordinate = snake.head();       // координата головы змеи
		//snake_head_coordinate += delta;                    // координата головы змеи после приращения (следующая позиция)
		snake_head_coordinate.X += delta.X;
		snake_head_coordinate.Y += delta.Y;


		// если голова змеи столкнулась с границей поля или со телом змеи, то змея умирает
		if (snake_head_coordinate.X == 0 || snake_head_coordinate.X == width - 1 || snake_head_coordinate.Y == 0 || snake_head_coordinate.Y == height - 1 || snake.into(snake_head_coordinate)) {
			state = STATE_DIED;
		}

		if (state == STATE_OK) {          // если змея ещё жива, то
			snake.move(delta, screen);     // сдвинуть змею на delta

			if (snake.head().X == food.X && snake.head().Y == food.Y) { // если координата головы змеи совпадает с координатой еды, то
				snake.grow(food, 3);    // увеличить длину змеи
				food = make_food();     // вычислить координаты новой еды
				screen.print_console_symbol(food.X, food.Y, FOOD); // вывести еду на экран

				// Вычисление времени игры, частичного и общего рейтинга.
				// Частичный рейтинг вычисляется как длина змеи, делённая на время в секундах,
				// затраченное на подход к еде (время от съедения предыдущей еды до съедения следующей).
				// Таким образом, чем чаще змея ест и чем она длиннее, тем выше частичный рейтинг.
				time2 = clock();
				duration = time2 - time1;
				duration_game += static_cast<double>(duration) / CLOCKS_PER_SEC;
				//rating_i = static_cast<double>(snake.size()) / duration * CLOCKS_PER_SEC;
				//rating += rating_i;     // общий рейтинг - сумма частичных рейтингов за игру
				//time1 = time2;

				print_stat();           // вывод текущей статистики игры
			}

			Sleep(latency);             // задержка перед следующим изменением позиции
		}

	} while (state == STATE_OK);          // играем, пока змея жива

	screen.print_console_string(width / 2 - 8, 10, " G a m e    o v e r ");
	clearkeys();
	_getch();
	clearkeys();
}