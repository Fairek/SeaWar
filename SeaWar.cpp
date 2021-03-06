#include "pch.h"
#include <iostream>
#include <ctime>

// Сокращение написания самого маленького по памяти типа
#define byte unsigned short int
//#define DEBUG

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
	SHOT_DESTROY,
	SHOT_ERR
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
	~Ship() {
		x = 0;
		y = 0;
	}
};

// Прототипы функций
byte nextTurn(Gamer);
void initField();
void initShips(Board * brd);
bool isPosible(Ship *, Board *, byte);
void drawBoards();
Shot shoot(byte, byte, Gamer);
byte charToState(char);
void getXY(byte*, byte*);

// Две "доски"
Board * player;
Board * AI;

int main()
{
	srand(time(0)); // Инициализация рандомизатора
	setlocale(0, ""); // Установка русской локали

	byte lpsc = 20, lpscAI = 20, lpscPlayer = 20; // Last player ships count
	byte step = 0;

	initField();
	initShips(player);
	initShips(AI);

	do {
		step++;
#ifdef DEBUG
		system("clear");
#else
		system("cls");
#endif // DEBUG
		cout << "Ход №" << step << endl;
		drawBoards();
		lpscAI += nextTurn(GAMER_PLAYER);
		lpscPlayer += nextTurn(GAMER_AI);
		lpsc = (lpscPlayer < lpscAI) ? lpscPlayer : lpscAI;
	} while (lpsc > 0);

	cout << "GAME OVER! " << ((lpscPlayer < lpscAI) ? "AI" : "You") << " win!" << endl;
	system("pause");
	system("pause");
}

// Отработка следующего хода
byte nextTurn(Gamer gmr) {
	Shot result;
	switch (gmr) {
	case GAMER_PLAYER: {
		do {
			// Получаем координаты
			byte *x = new byte(-1), *y = new byte(-1);
			getXY(x, y);
			
			result = shoot(*x, *y, GAMER_PLAYER);
			switch (result)
			{
			case SHOT_MISS:
				return 0;
			case SHOT_DESTROY:
				return -1;
			case SHOT_ERR:
				cout << "\nВ эту клетку стрелять нельзя! Попробуйте ещё раз.\n";
				break;
			}
		} while (result == SHOT_ERR);

		break; 
	}
	case GAMER_AI:
		// Выстрел
		do {
			result = shoot(rand() % 9, rand() % 9, GAMER_AI);
			switch (result)
			{
			case SHOT_MISS:
				return 0;
			case SHOT_DESTROY:
				return -1;
			case SHOT_ERR:
				break;
			}
		} while (result == SHOT_ERR);
	}
	return 0;
}

void getXY(byte * x, byte * y) {
	char shotX, shotY;
	// Защита от дурака
	do {
		cout << "\nПожалуйста, введите адрес ячейки, в которую хотите выстрелить: ";
		cin >> shotY >> shotX;
		
		// Преобразование символов в числа
		*x = charToState(shotX);
		*y = charToState(shotY);

		// Очистка потока ввода
		cin.clear();
		while (cin.get() != '\n');
		
		// Проверка введенного адреса
		if (*x < 0 || *y < 0 || *x > 9 || *y > 9)
			cout << "Вы ввели не верный адрес ячейки! Попробуйте еще раз.\n";
		else {
#ifdef DEBUG
			cout << "Y = " << y << " X = " << x << endl;
#endif // DEBUG
			break;
		}
	} while (true);
}

// Отработка одного выстрела
Shot shoot(byte y, byte x, Gamer gmr)
{
	Board * target = new Board;
	target = (gmr == GAMER_PLAYER) ? AI : player;

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
		return SHOT_ERR;
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
	cout << "\n        player                     AI          ";
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
		cout << AI->y[y] << " ";
	}
	cout << endl;
	// Вывод досок
	for (byte x = 0; x < 10; x++) {
		// Доска пользователя
		cout << player->x[x] << " ";
		for (byte y = 0; y < 10; y++) {
			cout << (char)player->field[x][y] << " ";
		}
		cout << "--- " << AI->x[x] << " ";
		// Доска ИИ
		for (byte y = 0; y < 10; y++) {
#ifdef DEBUG
			cout << (char)AI->field[x][y] << " ";
#else
			char out = (char)AI->field[x][y] == STATE_SHIP ? STATE_EMPTY : (char)AI->field[x][y];
			cout << out << " ";
#endif // DEBUG
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
	byte countOfDeck = 4, numOfShip = 10;
	Ship * templ = new Ship [numOfShip];

	for (numOfShip; numOfShip >= 1; numOfShip--) {
		if (numOfShip == 9 || numOfShip == 7 || numOfShip == 3) countOfDeck--;
		do {
						
			// Шаблон корабля
			templ = new Ship[countOfDeck];
			
			// Рандомные координаты
			templ[0].x = 0 + rand() % 9;
			templ[0].y = 0 + rand() % 9;
			if (countOfDeck > 1)
				if (rand() % 2)
					for (int i = 1; i < countOfDeck; i++) { // Вниз
						templ[i].x = templ[i - 1].x;
						templ[i].y = templ[i - 1].y + 1;
					}
				else
					for (int i = 1; i < countOfDeck; i++) { // Вправо
						templ[i].x = templ[i - 1].x + 1;
						templ[i].y = templ[i - 1].y;
					}

			// Проверка на возможность установки
			if (isPosible(templ, brd, countOfDeck)) {
				for (int i = 0; i < countOfDeck; i++) // Нанесение шаблона на доску
					brd->field[templ[i].x][templ[i].y] = STATE_SHIP;
				break;
			}
		    delete[] templ; // Высвобождение памяти

		} while (true);
	}
}

// Проверка на возможность установки корабля в клетку
bool isPosible(Ship * templ, Board * brd, byte type) {
	// Проверка на выход за края доски 
	for (int i = 0; i < type; i++) {
		if (!(templ[i].x >= 0 && templ[i].x <= 9 && templ[i].y >= 0 && templ[i].y <= 9)) return false;
	}
	// Проверка на наезд на другой корабль
	for (int i = 0; i < type; i++) {
		if (brd->field[templ[i].x][templ[i].y] == STATE_SHIP) return false;
	}
	// Проверка на соприкосновение с другим кораблем
	for (int i = 0; i < type; i++) {
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