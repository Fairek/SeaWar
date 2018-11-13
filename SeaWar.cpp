#include "pch.h"
#include <iostream>
#include <ctime>

// Сокращение написания самого маленького по памяти типа
#define byte unsigned short int

using namespace std;

// Перечисление состояний для клеток поля
enum State : char {
	STATE_EMPTY   = '0', // Пусто
	STATE_SHIP    = 'v', // Кусок корабля
	STATE_DESTROY = 'x', // Убитый кусок корабля
	STATE_BUSY    = '*'  // Промах
};

// Перечисление игроков
enum Gamer : byte {
	GAMER_PLAYER,
	GAMER_AI
};

// Перечисление результатов выстрела
enum Shot {
	SHOT_MISS,
	SHOT_DESTROY
};

// Структура "доски"
struct Board {
	char x[10] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
	char y[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	State field[10][10];
};

// Структура шаблона корабля
struct Ship {
	byte x;
	byte y;
};

// Прототипы функций
byte nextTurn(Gamer);
void initField();
void initShips(Board * brd);
bool isPosible(Ship *, Board *, byte);
void drawBoards();
Shot shoot(char, char, Gamer);
byte charToState(char);

// Две "доски"
Board * player;
Board * AI;

int main()
{
	setlocale(0, "");

	byte lpsc = 10; // Last player ships count

	initField();
	initShips(player);
	initShips(AI);

	do {
		byte lpscAI = nextTurn(GAMER_PLAYER);
		byte lpscPlayer = nextTurn(GAMER_AI);
		lpsc = (lpscPlayer < lpscAI) ? lpscPlayer : lpscAI;
	} while (lpsc > 0);
}

// Отработка следующего хода
byte nextTurn(Gamer gmr) {
	switch (gmr) {
	case GAMER_PLAYER:
		drawBoards();
		cout << "\nПожалуйста, введите адрес ячейки, в которую хотите выстрелить\nв формате"
			<< "\"%номер строки% %пробел% %номер столбца%\":";
		char shotX, shotY;
		cin >> shotY >> shotX;

		cout << "Y = " << shotY << " X = " << shotX << endl;

		//shoot(shotX, shotY, GAMER_PLAYER);

		system("pause");
		break;
	case GAMER_AI:

		break;
	}

	return 0;
}

// Отработка одного выстрела
Shot shoot(char chX, char chY, Gamer gmr)
{
	Board * target = new Board;
	byte x, y;
	switch (gmr)
	{
	case GAMER_PLAYER:
		target = AI;
		if (x = charToState(chX) == -1) {
			
		};
		if (y = charToState(chY) == -1) {

		};
		break;
	case GAMER_AI:
		target = player;
		break;
	}

	switch (target->field[x][y])
	{
	case STATE_SHIP: // корабль
		target->field[x][y] = STATE_DESTROY;
		return SHOT_DESTROY;
	case STATE_EMPTY: // пусто
		target->field[x][y] = STATE_BUSY;
		return SHOT_MISS;
	default: // либо промах, либо ошметок
		// Возврат пользователю сообщения о том, что он - лох и в эту клетку стрелять нельзя
		return SHOT_MISS;
	}
}

byte charToState(char ch)
{
	switch (ch)
	{
	case '0': case 'A': case 'a':
		return 0;
	case '1': case 'B': case 'b':
		return 1;
	case '2': case 'C': case 'c':
		return 2;
	case '3': case 'D': case 'd':
		return 3;
	case '4': case 'E': case 'e':
		return 4;
	case '5': case 'F': case 'f':
		return 5;
	case '6': case 'G': case 'g':
		return 6;
	case '7': case 'H': case 'h':
		return 7;
	case '8': case 'I': case 'i':
		return 8;
	case '9': case 'J': case 'j':
		return 9;
	default:
		return -1;
	}
}

// Отрисовка досок для человека
void drawBoards() {
	cout << "\n-----------------------------------------------\n";

	cout << "  ";
	// Вывод имен столбцов
	for (byte y = 0; y < 10; y++)
	{
		cout << player->y[y] << " ";
	}
	cout << "---   ";
	// Вывод имен столбцов
	for (byte y = 0; y < 10; y++)
	{
		cout << player->y[y] << " ";
	}
	cout << endl;
	for (byte x = 0; x < 10; x++) {
		cout << player->x[x] << " ";
		for (byte y = 0; y < 10; y++) {
			cout << (char)player->field[x][y] << " ";
		}
		cout << "--- " << AI->x[x] << " ";
		for (byte y = 0; y < 10; y++) {
			cout << (char)AI->field[x][y] << " ";
		}
		cout << endl;
	}

	cout << "-----------------------------------------------\n";
}

// Создание досок и заполненике их нулями
void initField() {
	player = new Board;
	AI = new Board;

	for (byte x = 0; x < 10; x++)
		for (byte y = 0; y < 10; y++) {
			player->field[x][y] = STATE_EMPTY;
			AI->field[x][y] = STATE_EMPTY;
		}
}

// Установка кораблей
void initShips(Board * brd) {
	srand(time(0)); // Инициализация рандомизатора
	byte x, y;

	// 4 палубы
	for (int s = 1; s <= 1; s++) {
		Ship * templ;
		do {
			// Шаблон корабля
			templ = new Ship[4];
			// Рандомные координаты
			templ[0].x = 0 + rand() % 9;
			templ[0].y = 0 + rand() % 9;
			if (0 + rand() % 1) {
				// Вниз
				
				templ[1].x = templ[0].x;
				templ[2].x = templ[0].x;
				templ[3].x = templ[0].x;

				templ[1].y = templ[0].y + 1;
				templ[2].y = templ[0].y + 2;
				templ[3].y = templ[0].y + 3;
			} else {
				// Вправо
				templ[1].y = templ[0].y;
				templ[2].y = templ[0].y;
				templ[3].y = templ[0].y;

				templ[1].x = templ[0].x + 1;
				templ[2].x = templ[0].x + 2;
				templ[3].x = templ[0].x + 3;
			}
			// Проверка на возможность установки
		} while (!isPosible(templ, brd, 4));
		for (int i = 0; i < 4; i++)
			brd->field[templ[i].x][templ[i].y] = STATE_SHIP;
	}
	drawBoards();
	system("pause");
	// 3 палубы
	for (int i = 1; i <= 2; i++) {
		//
		for (int p = 1; p <= 2; p++) {

		}
	}
	// 2 палубы
	for (int i = 1; i <= 3; i++) {
	
	}
	// 1 палуба
	for (int i = 1; i <= 4; i++) {
	
	}
}

// Проверка на возможность установки корабля в клетку
bool isPosible(Ship * templ, Board * brd, byte type) {
	// Проверка на выход за края доски 
	for (int i = 0; i < 4; i++) {
		if (!(templ[i].x >= 0 && templ[i].x <= 9 && templ[i].y >= 0 && templ[i].y <= 9)) return false;
	}
	// Проверка на наезд на другой корабль
	for (int i = 0; i < 4; i++) {
		if (brd->field[templ[i].x][templ[i].y] == STATE_SHIP) return false;
	}
	// Проверка на соприкосновение с другим кораблем
	for (int i = 0; i < 4; i++) {
		if (templ[i].x == 0 || templ[i].x == 9 || templ[i].y == 0 || templ[i].y == 9) {
			// У грани
			if (templ[i].x == 0) {
				// Проверка полей СПРАВА
				if (brd->field[templ[i].x + 1][templ[i].y - 1] == STATE_SHIP ||
					brd->field[templ[i].x + 1][templ[i].y] == STATE_SHIP ||
					brd->field[templ[i].x + 1][templ[i].y + 1] == STATE_SHIP
					)
					return false;
			} else if (templ[i].x == 9) {
				// Проверка полей СЛЕВА
				if (brd->field[templ[i].x - 1][templ[i].y - 1] == STATE_SHIP ||
					brd->field[templ[i].x - 1][templ[i].y] == STATE_SHIP ||
					brd->field[templ[i].x - 1][templ[i].y + 1] == STATE_SHIP
					)
					return false;
			} else {
				// Проверка СПРАВА и СЛЕВА
				if (brd->field[templ[i].x + 1][templ[i].y - 1] == STATE_SHIP ||
					brd->field[templ[i].x + 1][templ[i].y] == STATE_SHIP ||
					brd->field[templ[i].x + 1][templ[i].y + 1] == STATE_SHIP ||
					brd->field[templ[i].x - 1][templ[i].y - 1] == STATE_SHIP ||
					brd->field[templ[i].x - 1][templ[i].y] == STATE_SHIP ||
					brd->field[templ[i].x - 1][templ[i].y + 1] == STATE_SHIP
					)
					return false;
			}
			if (templ[i].y == 0) {
				// Проверка полей СНИЗУ
				if (brd->field[templ[i].x - 1][templ[i].y + 1] == STATE_SHIP ||
					brd->field[templ[i].x][templ[i].y + 1] == STATE_SHIP ||
					brd->field[templ[i].x + 1][templ[i].y + 1] == STATE_SHIP
					)
					return false;
			}
			else if (templ[i].y == 9) {
				// Проверка полей СВЕРХУ
				if (brd->field[templ[i].x - 1][templ[i].y - 1] == STATE_SHIP ||
					brd->field[templ[i].x][templ[i].y - 1] == STATE_SHIP ||
					brd->field[templ[i].x + 1][templ[i].y - 1] == STATE_SHIP
					)
					return false;
			}
			else {
				// Проверка СНИЗУ и СВЕРХУ
				if (brd->field[templ[i].x - 1][templ[i].y + 1] == STATE_SHIP ||
					brd->field[templ[i].x][templ[i].y + 1] == STATE_SHIP ||
					brd->field[templ[i].x + 1][templ[i].y + 1] == STATE_SHIP ||
					brd->field[templ[i].x - 1][templ[i].y - 1] == STATE_SHIP ||
					brd->field[templ[i].x][templ[i].y - 1] == STATE_SHIP ||
					brd->field[templ[i].x + 1][templ[i].y - 1] == STATE_SHIP
					)
					return false;
			}
		} else {
			// Проверка 8ми клеток вокруг
			if (brd->field[templ[i].x - 1][templ[i].y - 1] == STATE_SHIP ||
				brd->field[templ[i].x][templ[i].y - 1] == STATE_SHIP ||
				brd->field[templ[i].x + 1][templ[i].y - 1] == STATE_SHIP ||
				brd->field[templ[i].x + 1][templ[i].y] == STATE_SHIP ||
				brd->field[templ[i].x + 1][templ[i].y + 1] == STATE_SHIP ||
				brd->field[templ[i].x][templ[i].y + 1] == STATE_SHIP ||
				brd->field[templ[i].x - 1][templ[i].y + 1] == STATE_SHIP ||
				brd->field[templ[i].x - 1][templ[i].y] == STATE_SHIP
				)
				return false;
		}
	}
	return true;
}