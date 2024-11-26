#include "GameEngine.h"
#include "Server.h"

int main() {
    GameEngine game;      // Initialize the game engine
    Server server(game);  // Create the server with the game engine

    server.start(12345);  // Start the server on port 12345

    return 0;
}
