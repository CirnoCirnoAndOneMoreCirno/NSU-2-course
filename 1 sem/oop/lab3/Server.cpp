#include "Server.h"

Server::Server(IGamer* A, IGamer* B, gamers g1, gamers g2)
{
	gamerA = g1;
	gamerB = g2;
	playerA = A;
	playerB = B;
}

int Server::Game()
{
		playerA->prepare();
		playerB->prepare();
		int killsA = 0, killsB = 0;
		while ((killsA < 10) && (killsB < 10))
		{
			playerA->attack();
			killsA = playerA->getKillsStats();
			if (killsA == 10)
				break;
			playerB->attack();
			killsB = playerB->getKillsStats();
		}
		if (killsA == 10)
			return 1;
		return 2;
}

/*

bool Server::setShip(int x1, int y1, int x2, int y2, IGamer*player )//Добавить расстояние между кораблями
{
	char** field = player->getSelfField();
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

bool Server::checkNeighbors(int x, int y, char** field)
{
	if (((y > 0) && (field[y - 1][x] == '#'))
		|| ((y < 9) && (field[y + 1][x] == '#'))
		|| ((x > 0) && (field[y][x - 1] == '#'))
		|| ((x < 9) && (field[y][x + 1] == '#')))
		return true;
	return false;
}

void Server::paintKilledShip(int x, int y, char** field)
{
	field[y][x] = '*';
	for (int i = x - 1; ((i >= 0) && (field[y][i] == '@')); i--)
		field[y][i] = '*';
	for (int i = x + 1; ((i < 10) && (field[y][i] == '@')); i++)
		field[y][i] = '*';
	for (int i = y - 1; ((i >= 0) && (field[i][x] == '@')); i--)
		field[i][x] = '*';
	for (int i = y + 1; ((i < 10) && (field[i][x] == '@')); i++)
		field[i][x] = '*';
}

int Server::attackResult(int x, int y, IGamer* attackedPlayer)
{
	char** selfField = attackedPlayer->getSelfField();
	if (selfField[y][x] == 'O')
		return miss;
	if (selfField[y][x] == '#')
	{
		if (checkNeighbors(x, y, selfField))
			return hit;
		else
			return kill;
	}
	return noRes;
}
*/