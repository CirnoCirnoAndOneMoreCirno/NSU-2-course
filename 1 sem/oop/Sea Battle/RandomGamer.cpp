#include "Gamer.h"

RandomGamer::RandomGamer(string n)
{
	name = n;
	logs.open((string)name + ".txt");
	for (int i = 0; i < 100; i++)
		attackCords.push_back(i);
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

void RandomGamer::prepare()
{
	int shipSize = 4;
	for (int i = 0; i < 10; i++)
	{
		if (i == 1)
			shipSize = 3;
		else if (i == 3)
			shipSize = 2;
		else if (i == 6)
			shipSize = 1;
		bool setRes = false;
		while(!setRes)
		{
			int cord = rand()%100;;
			int y1 = cord / 10, x1 = cord % 10;
			int x2, y2;
			int rot = rand() % 2; //0 - вертикально
								  // 1 - горизотнально
			if (rot)
			{
				x2 = x1;
				y2 = y1 - shipSize + 1;
			}
			else
			{
				y2 = y1;
				x2 = x1 - shipSize + 1;
			}
			if ((x2 >= 0) && (y2 >= 0) && (x2 < 10) && (y2 < 10))
				setRes = setShip(x1, y1, x2, y2);
		}

	}
	logs << "Поле игрока " << name << endl;
	logs << "  abcdefghij" << endl;
	for (int i = 0; i < 10; i++)
	{
		logs << i << ':';
		for (int j = 0; j < 10; j++)
		{
			logs << selfField[i][j];
		}
		logs << endl;
	}
	logs << endl;
}

void RandomGamer::attack()
{
	int index = rand() % possibleAttackCords;
	int cords = attackCords[index];
	attackCords.erase(attackCords.begin() + index);
	possibleAttackCords--;
	int y = cords / 10;
	int x = cords % 10;
	int res = enemy->attackResult(x, y);
	logs << "Атака игрока " << name << endl;
	logs <<(char) ('a' + x) << y << endl;
	if (res == hit)
	{
		enemyField[y][x] = '@';
		attack();
	}
	else if (res == kill)
	{
		kills++;
		paintKilledShip(x, y);
		if (kills < 10)
			attack();
	}
	else
	{
		enemyField[y][x] = 'X';
	}
}

RandomGamer::RandomGamer()
{
	logs.open((string)name + ".txt");
	for (int i = 0; i < 100; i++)
		attackCords.push_back(i);
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


void RandomGamer::paintKilledShip(int x, int y)
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

int RandomGamer::attackResult(int x, int y)
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

			if ((y > 0) && (selfField[y - 1][x] == '@'))
				hitRes = checkNeighbors(x, y-1, selfField) || hitRes;
			if ((y < 9) && (selfField[y + 1][x] == '@'))
				hitRes = checkNeighbors(x, y + 1, selfField) || hitRes;
			if ((x > 0) && (selfField[y][x-1] == '@'))
				hitRes = checkNeighbors(x-1, y, selfField) || hitRes;
			if ((x < 9) && (selfField[y][x+1] == '@'))
				hitRes = checkNeighbors(x+1, y, selfField) || hitRes;
			if (hitRes)
				return hit;
			return kill;
		}
	}
	return noRes;
}

void RandomGamer::setEnemy(IGamer* gamer)
{
	enemy = gamer;
}

void RandomGamer::setView(IGameView* v)
{
	view = v;
}

int RandomGamer::getKillsStats()
{
	return kills;
}


bool RandomGamer::setShip(int x1, int y1, int x2, int y2)
{
	if (x1 > x2)
		swap(x1, x2);
	if (y1 > y2)
		swap(y1, y2);
	if (x1 != x2)
	{
		for (int i = x1; i <= x2; i++)
			if ((selfField[y1][i] == '#') || checkNeighbors(i, y1, selfField))
				return false;
		for (int i = x1; i <= x2; i++)
			selfField[y1][i] = '#';
	}
	else
	{
		for (int i = y1; i <= y2; i++)
			if ((selfField[i][x1] == '#') || checkNeighbors(x1, i, selfField))
				return false;
		for (int i = y1; i <= y2; i++)
			selfField[i][x1] = '#';
	}
	return true;
}

void RandomGamer::resetGamer(int round)
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
	logs.close();
	logs.open((string)(name + ' ' + "round no " + to_string(round) + ".txt"));
	kills = 0;
	attackCords.clear();
	for (int i = 0; i < 100; i++)
		attackCords.push_back(i);
	possibleAttackCords = 100;
}

