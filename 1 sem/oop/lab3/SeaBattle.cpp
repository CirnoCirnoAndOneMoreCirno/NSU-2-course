#include <iostream>
#include <ctime>
#include "Gamer.h"
#include "Server.h"
#include "SFML/Graphics.hpp"

using namespace sf;

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");
    int roundCount = 1;
    IGamer* playerA = nullptr;
    IGamer* playerB = nullptr;
    gamers g1 = random, g2 = random;
    for (int i = 1; i < argc; i++)
    {
        if ((string)argv[i] == "-h")
        {
            cout << "-h - печать сообщения со списком параметров программы и завершение работы" << endl;
            cout << "-c количество раундов в серии" << endl;
            cout << "-f тип первого игрока (rand, opt, cons)" << endl;
            cout << "-s тип второго игрока  (rand, opt, cons)" << endl;
        }
        else if ((string)argv[i] == "-c")
        {
            roundCount = atoi(argv[i + 1]);
        }
        else if ((string)argv[i] == "-f")
        {
            if ((string)argv[i + 1] == "opt")
            {
                g1 = opt;
            }
            else if ((string)argv[i + 1] == "cons")
            {
                g1 = cons;
            }
        }
        else if ((string)argv[i] == "-s")
        {
            if ((string)argv[i + 1] == "opt")
            {
                g2 = opt;
            }
            else if ((string)argv[i + 1] == "cons")
            {
                g2 = cons;
            }
        }
    }
    srand(time(0));

    switch (g1)
    {
        case opt:
        {
            playerA = new OptimalGamer("Player A smart");
            break;
        }
        case random:
        {
            playerA = new RandomGamer("Player A");
            break;
        }
        case cons:
        {
            playerA = new ConsoleGamer();
            break;
        }
    }
    switch (g2)
    {
        case opt:
        {
            playerB = new OptimalGamer("Player B smart");
            break;
        }
        case random:
        {
            playerB = new RandomGamer("Player B");
            break;
        }
        case cons:
        {
            playerB = new ConsoleGamer();
            break;
        }
    }
    //IGamer* playerA = new ConsoleGamer();
    //IGamer* playerB = new OptimalGamer("Player B smart");
    
    IGameView* v = new ConsoleView();

    playerA->setView(v);
    playerB->setView(v);
    playerA->setEnemy(playerB);
    playerB->setEnemy(playerA);
    for (int i = 0; i < roundCount; i++) 
    {
        Server s(playerA, playerB, g1, g2);
        int winner = s.Game();
        if (winner == 1)
            v->printMessage(const_cast<char*>("Победил Игрок А!"));
        else
            v->printMessage(const_cast<char*>("Победил Игрок Б!"));
        playerA->resetGamer(i);
        playerB->resetGamer(i);

    }
    return 0;
}
