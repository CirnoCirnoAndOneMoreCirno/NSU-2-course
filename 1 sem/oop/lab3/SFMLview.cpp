#include "GameView.h"


SFMLview::SFMLview() 
{
	window = new RenderWindow(VideoMode(800, 800), "BattleShip");
}

void SFMLview::DrawField(char** field)
{
	for (int i = 0; i < 10; i++) 
	{
		for (int j = 0; j < 10; j++)
		{
			Color c;
			if (field[i][j] == 'O')
				c = Color::Blue;
			if (field[i][j] == '#')
				c = Color::Green;
			if (field[i][j] == '@')
				c = Color::Yellow;
			if (field[i][j] == '*')
				c = Color::Red;
			sf::RectangleShape square;
			square.setSize(sf::Vector2f(20, 20));
			square.setFillColor(c);
			square.setPosition(xPos + j*20 + 10, yPos+ i*20 + 10);
			window->draw(square);
			window->display();
		}
	}
	xPos += 400;
}

void SFMLview::printMessage(const char* mes)
{

}

void SFMLview::clearScreen()
{
	yPos = 0;
	xPos = 0;
	window->clear();
}