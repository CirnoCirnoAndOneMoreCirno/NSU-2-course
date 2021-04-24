#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace sf;


class IGameView
{
public:
	virtual void DrawField(char** field) = 0;
	virtual void printMessage(const char* mes) = 0;
	virtual void clearScreen() = 0;
};

class ConsoleView : public IGameView
{
public:
	ConsoleView();
	void DrawField(char** field) override;
	void printMessage(const char* mes) override;
	void clearScreen() override;
};

class SFMLview : public IGameView
{
private:
	int yPos = 0;
	int xPos = 0;
	RenderWindow *window;
public:
	SFMLview();
	void DrawField(char** field) override;
	void printMessage(const char* mes) override;
	void clearScreen() override;
};