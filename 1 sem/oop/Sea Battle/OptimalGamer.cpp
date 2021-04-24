#include "Gamer.h"

OptimalGamer::OptimalGamer(string n)
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

void OptimalGamer::prepare()
{
	int shipSize = 4;
	int square = rand() % 4;
	/*
	0: 0<=x<=5; 0<=y<=5 
	1: 4<=x<=9; 0<=y<=5 
	2: 4<=x<=9; 4<=y<=9 
	3: 0<=x<=5; 4<=y<=9 
	*/
	int triesCnt = 0;
	for (int i = 0; i < 6; i++)
	{
		if (i == 1)
			shipSize = 3;
		else if (i == 3)
			shipSize = 2;
		/*
		else if (i == 6)
			shipSize = 1;
			*/
		bool setRes = false;
		while ((!setRes) &&(!resField))
		{
			int x1, y1;
			/*
			int cord = rand() % 100;;
			int y1 = cord / 10, x1 = cord % 10;
			*/
			if (square == 0)
			{
				x1 = rand() % 7;
				y1 = rand() % 7;
			}
			else if (square == 1)
			{
				x1 = rand() % 7 + 3;
				y1 = rand() % 7;
			}
			else if (square == 2)
			{
				x1 = rand() % 7 + 3;
				y1 = rand() % 7 + 3;
			}
			else if (square == 3)
			{
				x1 = rand() % 7;
				y1 = rand() % 7 + 3;
			}
			int x2, y2;
			//if ()
			
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
			if (notTheSameSquare(x2, y2, square))
			{
				rot = (rot + 1) % 2;
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
			}
			if (notTheSameSquare(x2, y2, square))
			{
				x2 = -1; y2 = -1;
			}
			if ((x2 >= 0) && (y2 >= 0) && (x2 < 10) && (y2 < 10))
				setRes = setShip(x1, y1, x2, y2);
			triesCnt++;
			if (triesCnt > 1000)
				resField = true;
		}
	}
	/*
	logs << "=============" << endl;
	for (int i = 0; i < 10; i++)
	{
		logs << i << ':';
		for (int j = 0; j < 10; j++)
		{
			logs << selfField[i][j];
		}
		logs << endl;
	}
	logs << "=============" << endl;
	*/
	if (!resField)
	{
		for (int i = 6; i < 10; i++)
		{
			bool setRes = false;
			while (!setRes)
			{
				int x1, y1;
				x1 = rand() % 10;
				y1 = rand() % 10;
				while (!notTheSameSquare(x1, y1, square))
				{
					x1 = rand() % 10;
					y1 = rand() % 10;
				}
				setRes = setShip(x1, y1, x1, y1);
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
	else
	{
		logs << "Не получается установить корабли" << endl;
		resetField();
		prepare();
	}
}

void OptimalGamer::resetField()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			selfField[i][j] = 'O';
}

pair<int, int> OptimalGamer::getSmartCords(int tmpX, int tmpY)
{
	bool hasCords = false;
	int x, y;
	int i = tmpY, j = tmpX;
	while ((i > 0) && (enemyField[i - 1][tmpX] == '@'))
	{
		i--;
	}
	if ((i > 0) && (enemyField[i - 1][tmpX] == 'O'))
	{
		hasCords = true;
		y = i - 1;
		x = tmpX;
	}

	 i = tmpY, j = tmpX;
	while ((i < 9) && (enemyField[i + 1][tmpX] == '@')&&(!hasCords))
	{
		i++;
	}
	if ((i < 9) && (enemyField[i + 1][tmpX] == 'O') && (!hasCords))
	{
		hasCords = true;
		y = i + 1;
		x = tmpX;
	}

	 i = tmpY, j = tmpX;
	while ((j < 9) && (enemyField[tmpY][j + 1] == '@') && (!hasCords))
	{
		j++;
	}
	if ((j < 9) && (enemyField[tmpY][j + 1] == 'O') && (!hasCords))
	{
		hasCords = true;
		y = tmpY;
		x = j + 1;
	}

	 i = tmpY, j = tmpX;
	while ((j > 0) && (enemyField[tmpY][j - 1] == '@') && (!hasCords))
	{
		j--;
	}
	if ((j > 0) && (enemyField[tmpY][j - 1] == 'O') && (!hasCords))
	{
		hasCords = true;
		y = tmpY;
		x = j - 1;
	}
	if (!hasCords)
	{
		if ((tmpY > 0) && (enemyField[tmpY - 1][tmpX] == 'O'))
		{
			x = tmpX; y = tmpY - 1;
		}
		else if ((tmpY < 9) && (enemyField[tmpY + 1][tmpX] == 'O'))
		{
			x = tmpX; y = tmpY + 1;
		}
		else if ((tmpX > 0) && (enemyField[tmpY][tmpX - 1] == 'O'))
		{
			x = tmpX - 1; y = tmpY;
		}
		else if ((tmpX < 9) && (enemyField[tmpY][tmpX + 1] == 'O'))
		{
			x = tmpX + 1; y = tmpY;
		}
	}

	pair<int, int> p = make_pair(x, y);
	return p;
}

void OptimalGamer::attack()
{
	int x, y;
	int tmpX, tmpY;
	bool hitShip = false;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (enemyField[i][j] == '@')
			{
				hitShip = true;
				tmpX = j;
				tmpY = i;
				break;
			}
		}
		if (hitShip)
			break;
	}
	//Не забыть исключить координаты после "умной" атаки
	if (hitShip)
	{
		pair<int, int> p = getSmartCords(tmpX, tmpY);
		x = p.first;
		y = p.second;
		auto index = find(begin(attackCords), end(attackCords), y * 10 + x);
		if (index != end(attackCords))
		{
			attackCords.erase(index);
			possibleAttackCords--;
		}
	}
	else
	{
		int index = rand() % possibleAttackCords;
		int cords = attackCords[index];
		attackCords.erase(attackCords.begin() + index);
		possibleAttackCords--;
		y = cords / 10;
		x = cords % 10;
	}

	int res = enemy->attackResult(x, y);
	logs << "Атака игрока " << name << endl;
	logs << (char)('a' + x) << y << endl;
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

OptimalGamer::OptimalGamer()
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

void OptimalGamer::paintKilledShip(int x, int y)
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

int OptimalGamer::attackResult(int x, int y)
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

void OptimalGamer::setEnemy(IGamer* gamer)
{
	enemy = gamer;
}

void OptimalGamer::setView(IGameView* v)
{
	view = v;
}

int OptimalGamer::getKillsStats()
{
	return kills;
}


bool OptimalGamer::setShip(int x1, int y1, int x2, int y2)
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

/*bool OptimalGamer::getResField()
{
	return resField;
}*/

bool OptimalGamer::notTheSameSquare(int x, int y, int square)
{
	if (square == 0)
	{
		return (x > 6) || (y > 6);
	}
	if (square == 1)
	{
		return (x < 3) || (y > 6);
	}
	if (square == 2)
	{
		return (x < 3) || (y < 3);
	}
	if (square == 3)
	{
		return (x > 6) || (y < 3);
	}
}

void OptimalGamer::resetGamer(int round)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			selfField[i][j] = 'O';
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			enemyField[i][j] = 'O';
	}
	logs.close();
	logs.open((string)(name + ' ' + "round no " + to_string(round) + ".txt"));
	kills = 0;
	hitFlag = false;
	horHit = false;
	verHit = false;
	resField = false;
	attackCords.clear();
	for (int i = 0; i < 100; i++)
		attackCords.push_back(i);
	possibleAttackCords = 100;
}
