#include "pch.h"
#include <iostream>

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

// Прототипы функций
byte nextTurn(Gamer);
void initField();
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
		x = charToState(chX);
		y = charToState(chY);
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
	case '0': case 'A':
		return 0;
	case '1': case 'B':
		return 1;
	case '2': case 'C':
		return 2;
	case '3': case 'D':
		return 3;
	case '4': case 'E':
		return 4;
	case '5': case 'F':
		return 5;
	case '6': case 'G':
		return 6;
	case '7': case 'H':
		return 7;
	case '8': case 'I':
		return 8;
	case '9': case 'J':
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