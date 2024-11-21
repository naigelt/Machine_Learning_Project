#include "GameEngine.h"
#include "Server.h"
#include <iostream>

int main() {
    GameEngine gameEngine;
    Server server(gameEngine);

    std::cout << "Enter 'socket' to accept commands from a client or 'manual' for manual input: ";
    std::string mode;
    std::cin >> mode;

    if (mode == "socket") {
        std::cout << "Server is now running in socket mode." << std::endl;
        server.start(12345);
    } else if (mode == "manual") {
        std::cout << "Server is running in manual mode." << std::endl;
        while (true) {
            Player& currentPlayer = gameEngine.getCurrentPlayer();

            std::cout << "\nPlayer " << (gameEngine.getCurrentPlayerIndex() + 1) << "'s turn." << std::endl;

            int diceRoll = gameEngine.rollDice();
            std::cout << "Rolled a " << diceRoll << "." << std::endl;

            gameEngine.movePlayer(currentPlayer, diceRoll);

            gameEngine.nextTurn();
        }
    }

    return 0;
}
