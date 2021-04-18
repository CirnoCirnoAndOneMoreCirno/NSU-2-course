#include "GameView.h"

using namespace std;

ConsoleView::ConsoleView(){}

void ConsoleView::DrawField(char** field)
{
	cout << "  abcdefghij" << endl;
	for (int i = 0; i < 10; ++i)
	{
		cout << i <<":";
		for (int j = 0; j < 10; ++j)
		{
			cout << field[i][j];
		}
		cout << endl;
	}
}

void ConsoleView::printMessage(const char* mes)
{
	cout << mes << endl;
}

void ConsoleView::clearScreen()
{
	system("CLS");
}