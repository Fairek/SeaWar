#include "pch.h"
#include <iostream>
#include <windows.h>

// Сокращение написания самого маленького по памяти типа
#define byte unsigned short int

using namespace std;

const int N = 10;
const int NUM_SHIPS = 9;
int Ships_id = 1;
int Ships[10] = { 0 };

void gotoxy(int x, int y)
{
	COORD p = { (short) x, (short) y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

void ship_show(int x, int y, int dir, int size_ship)
{
	for (int i = 0; i < size_ship; i++)
	{
		gotoxy(x + 1, y + 1);//функия, которая переставляет курсор в консоли
		cout << '#';//символ, который будет выводиться вместо палубы

		switch (dir)
		{
		case 0:
			x++;
			break;
		case 1:
			y++;
			break;
		case 2:
			x--;
			break;
		case 3:
			y--;
			break;
		}
	}
};

//проверка, находится ли корабль в пределах карты
bool ship_in_map(int x, int y, int dir, int size_ship)
{
	bool in_map = 1;
	//проверка возможности постановки корябля

	for (int i = 0; i < size_ship; i++)
	{
		if (x < 0 || y < 0 || x >= N || y >= N)
		{
			in_map = 0;
			break;
		}

		switch (dir)
		{
		case 0:
			x++;
			break;
		case 1:
			y++;
			break;
		case 2:
			x--;
			break;
		case 3:
			y--;
			break;
		}
	}
	return in_map;
}

bool set_ship(int map[N][N], int x, int y, int dir, int size_ship)
{
	int temp_x = x;
	int temp_y = y;

	bool setting_is_possible = 1;

	//проверка возможности постановки корябля
	for (int i = 0; i < size_ship; i++)
	{
		if (x < 0 || y < 0 || x >= N || y >= N)
		{
			setting_is_possible = 0;
			break;
		}

		//проверка границ
		if (map[x][y] >= 1)
		{
			setting_is_possible = 0;
			break;
		}
		if (y < N - 1)
			if (map[x][y + 1] >= 1)
			{
				setting_is_possible = 0;
				break;
			}
		if (y > 0)
			if (map[x][y - 1] >= 1)
			{
				setting_is_possible = 0;
				break;
			}
		if (x < N - 1)
			if (map[x + 1][y] >= 1)
			{
				setting_is_possible = 0;
				break;
			}
		if (x < N - 1 && y < N - 1)

			if (map[x + 1][y + 1] >= 1)
			{
				setting_is_possible = 0;
				break;
			}
		if (x < N - 1 && y > 0)
			if (map[x + 1][y - 1] >= 1)
			{
				setting_is_possible = 0;
				break;
			}
		if (x > 0)
			if (map[x - 1][y] >= 1)
			{
				setting_is_possible = 0;
				break;
			}
		if (x > 0 && y < N - 1)
			if (map[x - 1][y + 1] >= 1)
			{
				setting_is_possible = 0;
				break;
			}
		if (x > 0 && y > 0)
			if (map[x - 1][y - 1] >= 1)
			{
				setting_is_possible = 0;
				break;
			}
		switch (dir)
		{
		case 0:
			x++;
			break;
		case 1:
			y++;
			break;
		case 2:
			x--;
			break;
		case 3:
			y--;
			break;
		}
	}

	//если есть возможность поставить корабль ставим
	if (setting_is_possible == 1)
	{
		x = temp_x;
		y = temp_y;
		for (int i = 0; i < size_ship; i++)
		{
			map[x][y] = Ships_id;
			switch (dir)
			{
			case 0:
				x++;
				break;
			case 1:
				y++;
				break;
			case 2:
				x--;
				break;
			case 3:
				y--;
				break;
			}
		}

		Ships[Ships_id] = size_ship;
		Ships_id++;
	}
	return setting_is_possible;
}

void set_rand_ships(int map[N][N], int size_ship, int ship_id)
{
	int x, y; //определяют первичную координату корабля
	int dir = 0;
	int count_ship = 0;
	int count_tact = 0;//счетчик для предотвращения зацикливания

	while (count_ship < 1)
	{
		count_tact++;

		if (count_tact > 1000)//если больше 1000, то принудительно прервать цикл
		{
			break;
		}

		//первичная позиция
		x = rand() % N;
		y = rand() % N;

		int temp_x = x;//временно хранит занчение х
		int temp_y = y;//временно хранит занчение y
       
        //генерация направления
		dir = rand() % 4;
		bool setting_is_possible = 1;

		//проверка возможности постановки корябля
		for (int i = 0; i < size_ship; i++)
		{
			if (x < 0 || y < 0 || x >= N || y >= N)
			{
				setting_is_possible = 0;
				break;
			}

			//проверка граничащих клеток
			if (map[x][y] >= 1 ||
				map[x][y + 1] >= 1 ||
				map[x][y - 1] >= 1 ||
				map[x + 1][y] >= 1 ||
				map[x + 1][y + 1] >= 1 ||
				map[x + 1][y - 1] >= 1 ||
				map[x - 1][y] >= 1 ||
				map[x - 1][y + 1] >= 1 ||
				map[x - 1][y - 1] >= 1)
			{
				setting_is_possible = 0;
				break;
			}

			switch (dir)
			{
			case 0:
				x++;
				break;
			case 1:
				y++;
				break;
			case 2:
				x--;
				break;
			case 3:
				y--;
				break;
			}
		}

		//если есть возможность поставить корабль ставим
		if (setting_is_possible == 1)
		{
			x = temp_x;
			y = temp_y;

			for (int i = 0; i < size_ship; i++)
			{
				map[x][y] = ship_id;

				switch (dir)
				{
				case 0:
					x++;
					break;
				case 1:
					y++;
					break;
				case 2:
					x--;
					break;
				case 3:
					y--;
					break;
				}
			}
			count_ship++;
		}
	}
}﻿
