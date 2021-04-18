#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GameView.h"

using namespace std;

enum attackRes { noRes = 0, miss = 1, hit = 2, kill = 3 };

bool checkNeighbors(int x, int y, char** field);

//bool setShip(int x1, int y1, int x2, int y2, char** field);

class IGamer
{
public:
	virtual int attackResult(int x, int y) = 0;
	virtual void attack() = 0;
	virtual void prepare() = 0;
	virtual void setView(IGameView *v) = 0;
	virtual void setEnemy(IGamer* gamer) = 0;
	//virtual char** getSelfField() = 0;
	//virtual char** getEnemyField() = 0;
	virtual int getKillsStats() = 0;
	virtual void resetGamer(int round) = 0;
	//virtual ~IGamer();
};

class ConsoleGamer : public IGamer
{
private:
	IGamer* enemy;
	char** selfField;
	char** enemyField;
	int kills = 0;
	IGameView* view;
	string name;
	void paintKilledShip(int x, int y);
	bool setShip(int x1, int y1, int x2, int y2, char** field);
public:
	ConsoleGamer();
	ConsoleGamer(string name);
	int attackResult(int x, int y) override;
	void attack() override;
	void prepare() override;
	void setEnemy(IGamer *gamer) override;
	void setView(IGameView* v) override;
	//char** getSelfField() override;
	//char** getEnemyField() override;
	int getKillsStats();
	void resetGamer(int round) override;
	//~ConsoleGamer() override;
};

class RandomGamer : public IGamer
{
private:
	ofstream logs;
	string name = "Random_player";
	int possibleAttackCords = 100;
	vector<int> attackCords;
	IGamer* enemy;
	char** selfField;
	char** enemyField;
	int kills = 0;
	IGameView* view;
	void paintKilledShip(int x, int y);
	bool setShip(int x1, int y1, int x2, int y2);
public:
	RandomGamer();
	RandomGamer(string n);
	int attackResult(int x, int y) override;
	void attack() override;
	void prepare() override;
	void setEnemy(IGamer* gamer) override;
	void setView(IGameView* v) override;
	//char** getSelfField() override;
	//char** getEnemyField() override;
	int getKillsStats();
	void resetGamer(int round) override;
	//~RandomGamer() override;
};

class OptimalGamer : public IGamer
{
private:
	ofstream logs;
	string name = "Random_player";
	int possibleAttackCords = 100;
	vector<int> attackCords;
	IGamer* enemy;
	char** selfField;
	char** enemyField;
	bool hitFlag = false;
	bool horHit = false;
	bool verHit = false;
	bool resField = false;
	pair<int, int> getSmartCords(int tmpX, int tmpY);
	int kills = 0;
	IGameView* view;
	void paintKilledShip(int x, int y);
	bool setShip(int x1, int y1, int x2, int y2);
	bool notTheSameSquare(int x, int y, int square);
	void resetField();
public:
	OptimalGamer();
	OptimalGamer(string n);
	int attackResult(int x, int y) override;
	void attack() override;
	void prepare() override;
	void setEnemy(IGamer* gamer) override;
	void setView(IGameView* v) override;
	int getKillsStats();
	void resetGamer(int round) override;
	//~OptimalGamer() override;
	//bool getResField();
};