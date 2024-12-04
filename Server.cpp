#include "Server.h"
#include "GameEngine.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <nlohmann/json.hpp>

// Mutex to synchronize access to shared game state
std::mutex gameMutex;

Server::Server(GameEngine& engine) : gameEngine(engine) {}

void Server::start(int port) {
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return;
    }
    #endif

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
        cleanup();
        return;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        cleanup();
        return;
    }

    std::cout << "Server is running on port " << port << ". Waiting for clients..." << std::endl;

    sockaddr_in clientAddr{};
    socklen_t clientSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed." << std::endl;
        cleanup();
        return;
    }

    std::cout << "Client connected." << std::endl;

    // Run the game loop
    try {
        runGameLoop(clientSocket);
    } catch (const std::exception& ex) {
        std::cerr << "Error during game loop: " << ex.what() << std::endl;
    }

    #ifdef _WIN32
    closesocket(clientSocket);
    #else
    close(clientSocket);
    #endif

    cleanup();
}

void Server::runGameLoop(SOCKET clientSocket) {

    // Send game state to the client before first turn
    sendClientData(clientSocket);

    while (true) {
        {
            std::lock_guard<std::mutex> lock(gameMutex);

            // get current player
            Player& currentPlayer = gameEngine.getCurrentPlayer();
            int currentPlayerIndex = gameEngine.getCurrentPlayerIndex();

            // Announce the player's turn
            std::cout << "\nPlayer " << (currentPlayerIndex + 1) << "'s turn.\n";

            // Roll the dice
            int diceRoll = gameEngine.rollDice();
            currentPlayer.diceRoll = diceRoll;
            std::cout << "Rolled a " << diceRoll << ".\n";

            // Use movePlayer to handle all gameplay logic
            //    -Need to add some sort of message to show on GUI
            // 
            //      -Check if player has a pending disk interaction
            //      -Check if the player is stuck on a special event node
            //      -Check for the "wait until roll" event
            //      -Collect reachable flight paths, filtering by player's available money
            //      -Display reachable nodes
            //          -add sendClientData to movePlayer() to show available moves on GUI 
            //      -Ask the player for their choice
            //          -add some form of asking from GUI
            //      -Move the player to the selected destination
            //      -Special Events
            gameEngine.movePlayer(currentPlayer, diceRoll);

            // Send game state to the client
            sendClientData(clientSocket);

            // Switch to the next player's turn
            gameEngine.nextTurn();
        }
    }
}

void Server::sendClientData(SOCKET clientSocket) {
    nlohmann::json gameState;

    // Add player data
    for (size_t i = 0; i < gameEngine.players.size(); ++i) {
        const Player& player = gameEngine.players[i];
        nlohmann::json playerData = {
            {"location", player.currentNodeId},
            {"money", player.money},
            {"diceroll", player.diceRoll},
            {"possible moves", (i == gameEngine.getCurrentPlayerIndex()) ?
                nlohmann::json(gameEngine.getReachableNodes(player.currentNodeId, player.diceRoll)) :
                nlohmann::json::array()}
        };

        // Add information about opened disk if applicable
        if (player.discStatus.first) {
            playerData["opened disk type"] = static_cast<int>(player.discStatus.second);
        } else {
            playerData["opened disk type"] = nullptr;
        }

        gameState["players"]["player" + std::to_string(i + 1)] = playerData;
    }

    // Add current player index
    gameState["current player"] = gameEngine.getCurrentPlayerIndex();

    // Add city discs information
    /*for (const auto& [cityName, city] : gameEngine.getCities()) {
        gameState["city discs"][cityName] = (city.disk != nullptr);
    }*/

    // Changed JSON to include nodeIds of cities
    for (const auto& [cityName, city] : gameEngine.getCities()) {
        gameState["city discs"][cityName] = { (city.disk != nullptr), city.nodeId };
    }

    // Add remaining discs information
    std::unordered_map<DiskType, int> remainingDiscs;
    for (const auto& disk : gameEngine.getDisks()) {
        if (disk.type != DiskType::Empty) {
            remainingDiscs[disk.type]++;
        }
    }

    for (const auto& [diskType, count] : remainingDiscs) {
        gameState["remaining discs"].push_back({
            {"type", static_cast<int>(diskType)},
            {"count", count}
        });
    }

    // Send JSON data to the client
    std::string gameStateStr = gameState.dump();
    send(clientSocket, gameStateStr.c_str(), gameStateStr.size(), 0);
}




void Server::cleanup() {
    #ifdef _WIN32
    closesocket(serverSocket);
    WSACleanup();
    #else
    close(serverSocket);
    #endif
}
