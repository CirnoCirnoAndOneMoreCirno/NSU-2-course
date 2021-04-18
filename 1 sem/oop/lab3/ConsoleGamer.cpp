#include "Gamer.h"

bool checkNeighbors(int x, int y, char** field)
{
	if (((y > 0) && (field[y - 1][x] == '#'))
		|| ((y < 9) && (field[y + 1][x] == '#'))
		|| ((x > 0) && (field[y][x - 1] == '#'))
		|| ((x < 9) && (field[y][x + 1] == '#')))
		return true;
	return false;
}

ConsoleGamer::ConsoleGamer(string n)
{
	name = n;
	selfField = new char* [10];
	enemyField = new char* [10];

	for (int i = 0; i < 10; i++)
	{
		selfField[i] = new char[10];
		for (int j = 0; j < 10; j++)
			selfField[i][j] = 'O';
	}
	for (int i = 0; i < 10; i++)
	{
		enemyField[i] = new char[10];
		for (int j = 0; j < 10; j++)
			enemyField[i][j] = 'O';
	}

}

void ConsoleGamer::paintKilledShip(int x, int y)
{
	enemyField[y][x] = '*';
	for (int i = x - 1; ((i >= 0) && (enemyField[y][i] == '@')); i--)
	{
		enemyField[y][i] = '*';
	}
	for (int i = x + 1; ((i < 10) && (enemyField[y][i] == '@')); i++)
	{
		enemyField[y][i] = '*';
	}
	for (int i = y - 1; ((i >= 0) && (enemyField[i][x] == '@')); i--)
	{
		enemyField[i][x] = '*';
	}
	for (int i = y + 1; ((i < 10) && (enemyField[i][x] == '@')); i++)
	{
		enemyField[i][x] = '*';
	}
}

void swap(int &a, int& b)
{
	int c = a;
	a = b;
	b = c;
}

bool ConsoleGamer::setShip(int x1, int y1, int x2, int y2, char** field)
{
	if (x1 > x2)
		swap(x1, x2);
	if (y1 > y2)
		swap(y1, y2);
	if (x1 != x2)
	{
		for (int i = x1; i <= x2; i++)
			if ((field[y1][i] == '#') || checkNeighbors(i, y1, field))
				return false;
		for (int i = x1; i <= x2; i++)
			field[y1][i] = '#';
	}
	else 
	{
		for (int i = y1; i <= y2; i++)
			if ((field[i][x1] == '#') || checkNeighbors(x1, i, field))
				return false;
		for (int i = y1; i <= y2; i++)
			field[i][x1] = '#';
	}
	return true;
}

int ConsoleGamer::attackResult(int x, int y)
{
	if (selfField[y][x] == 'O')
	{
		selfField[y][x] = 'X';
		return miss;
	}
	if (selfField[y][x] == '#')
	{
		selfField[y][x] = '@';
		if (checkNeighbors(x, y, selfField))
			return hit;
		else
		{
			bool hitRes = false;
			/*
			if (((y > 0) && (field[y - 1][x] == '#'))
		|| ((y < 9) && (field[y + 1][x] == '#'))
		|| ((x > 0) && (field[y][x - 1] == '#'))
		|| ((x < 9) && (field[y][x + 1] == '#')))
			*/
			if ((y > 0) && (selfField[y - 1][x] == '@'))
				hitRes = checkNeighbors(x, y - 1, selfField) || hitRes;
			if ((y < 9) && (selfField[y + 1][x] == '@'))
				hitRes = checkNeighbors(x, y + 1, selfField) || hitRes;
			if ((x > 0) && (selfField[y][x - 1] == '@'))
				hitRes = checkNeighbors(x - 1, y, selfField) || hitRes;
			if ((x < 9) && (selfField[y][x + 1] == '@'))
				hitRes = checkNeighbors(x + 1, y, selfField) || hitRes;
			if (hitRes)
				return hit;
			return kill;
		}
	}
	return noRes;
}

int getCord(char cord)
{
	if ((cord >= 'a') && (cord <= 'j'))
	{
		return cord - 'a';
	}
	else if ((cord >= '0') && (cord <= '9'))
	{
		return cord - '0';
	}
	else
		return -1;
}

ConsoleGamer::ConsoleGamer()
{
	selfField = new char* [10];
	enemyField = new char* [10];

	for (int i = 0; i < 10; i++)
	{
		selfField[i] = new char[10];
		for (int j = 0; j < 10; j++)
			selfField[i][j] = 'O';
	}
	for (int i = 0; i < 10; i++)
	{
		enemyField[i] = new char[10];
		for (int j = 0; j < 10; j++)
			enemyField[i][j] = 'O';
	}

}

void ConsoleGamer::setEnemy(IGamer* gamer)
{
	enemy = gamer;
}

void ConsoleGamer::setView(IGameView* v)
{
	view = v;
}

int ConsoleGamer::getKillsStats()
{
	return kills;
}

void ConsoleGamer::attack()
{
	int res = 0;
	while(res == noRes)
	{
		//system("CLS");
		view->clearScreen();
		//cout << "Ваше поле:" << endl;
		view->printMessage("Ваше поле:");
		view->DrawField(selfField);
		//cout << "Поле для атаки" << endl;
		view->printMessage("Поле для атаки:");
		view->DrawField(enemyField);
		cout << "Введите координаты атаки" << endl;
		int x, y;
		string cord;
		cin >> cord;
		x = getCord(cord[0]); y = getCord(cord[1]);
		if ((x < 0) || (y < 0) || (enemyField[y][x] != 'O'))
		{
			//cout << "Неверно заданы координаты!" << endl;
			view->printMessage("Неверно заданы координаты");
			system("pause");
		}
		else 
		{
			res = enemy->attackResult(x, y);
			if (res == kill)
			{
				kills++;
				paintKilledShip(x, y);
				if (kills < 10)
					attack();
			}
			else if (res == hit)
			{
				enemyField[y][x] = '@';
				attack();
			}
			else
			{
				enemyField[y][x] = 'X';
			}
		}
	}
}

void ConsoleGamer::prepare()
{
	int freeShipsCount = 10;
	map<int, int> availableShips;

	for (int i = 1; i <= 4; i++)
	{
		availableShips.insert(make_pair(i, 5 - i));
	}
	while (freeShipsCount != 0)
	{
		string c1, c2;
		int size;
		int x1, y1, x2, y2;
		view->DrawField(selfField);
		cout << "У вас имеется:" << endl;
		for (int i = 1; i <= 4; i++)
		{
			cout  << "Кораблей размера " << i <<" : " << availableShips[i] << endl;
		}
		cout << "Чтобы установить корабль введите размер корабля и координаты его концов в консоль" << endl;
		cout << "Например, 1 a0 a0" << endl;
		cin >> size >> c1 >> c2;
		x1 = getCord(c1[0]); y1 = getCord(c1[1]);
		x2 = getCord(c2[0]); y2 = getCord(c2[1]);
		if ((x1 < 0) || (y1 < 0) || (x2 < 0) || (y2 < 0) 
			|| ((x1 != x2) && (y1 != y2)) 
			|| (max(abs(x1-x2), abs(y1-y2)) != (size-1)))
		{
			cout << "Неверно заданы координаты! Нажмите любую кнопку, чтобы продолжить" << endl;
			system("pause");
		}
		else if (availableShips[size] == 0)
		{
			cout << "У вас закончились корабли данного размера! Нажмите любую кнопку, чтобы продолжить" << endl;
			system("pause");
		}
		else 
		{
			bool isPossible = setShip(x1, y1, x2, y2, selfField);
			if (isPossible)
			{
				freeShipsCount--;
				availableShips[size]--;
			}
			else 
			{
				cout << "Корабль пересекается с уже существующим! Нажмите любую кнопку, чтобы продолжить" << endl;
				system("pause");
			}
		}
		view->clearScreen();
	}
}

void ConsoleGamer::resetGamer(int round)
{
	for (int i = 0; i < 10; i++)
	{
		selfField[i] = new char[10];
		for (int j = 0; j < 10; j++)
			selfField[i][j] = 'O';
	}
	for (int i = 0; i < 10; i++)
	{
		enemyField[i] = new char[10];
		for (int j = 0; j < 10; j++)
			enemyField[i][j] = 'O';
	}
	//logs.close();
	kills = 0;
}


