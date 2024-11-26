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
    while (true) {
        // Lock the game state
        std::lock_guard<std::mutex> lock(gameMutex);

        Player& currentPlayer = gameEngine.getCurrentPlayer();
        int currentPlayerIndex = gameEngine.getCurrentPlayerIndex();

        // Check if it's the client's turn
        if (currentPlayerIndex == 0) { // Assuming client is Player 1
            // Roll the dice and send options
            int diceRoll = gameEngine.rollDice();
            auto reachableNodes = gameEngine.getReachableNodes(currentPlayer.currentNodeId, diceRoll);

            nlohmann::json response;
            response["diceRoll"] = diceRoll;
            response["reachableNodes"] = reachableNodes;

            std::string responseStr = response.dump();
            send(clientSocket, responseStr.c_str(), responseStr.size(), 0);

            // Wait for the client's decision
            char buffer[1024];
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0) {
                std::cerr << "Client disconnected." << std::endl;
                break;
            }

            buffer[bytesReceived] = '\0';
            std::string command(buffer);

            // Parse the command
            std::istringstream iss(command);
            std::string action;
            int selectedNode;
            iss >> action >> selectedNode;

            if (action == "MOVE_PLAYER") {
                if (std::find(reachableNodes.begin(), reachableNodes.end(), selectedNode) != reachableNodes.end()) {
                    gameEngine.movePlayer(currentPlayer, selectedNode);
                    std::cout << "Client moved to node " << selectedNode << std::endl;
                } else {
                    std::cerr << "Invalid move from client." << std::endl;
                }
            }
        } else {
            // If it's not the client's turn, continue the game loop normally
            int diceRoll = gameEngine.rollDice();
            auto reachableNodes = gameEngine.getReachableNodes(currentPlayer.currentNodeId, diceRoll);

            // Simulate player action for Player 2
            if (!reachableNodes.empty()) {
                gameEngine.movePlayer(currentPlayer, reachableNodes[0]);
                std::cout << "Player 2 moved to node " << reachableNodes[0] << std::endl;
            }
        }

        // End turn and switch to the next player
        gameEngine.nextTurn();
    }
}

void Server::cleanup() {
    #ifdef _WIN32
    closesocket(serverSocket);
    WSACleanup();
    #else
    close(serverSocket);
    #endif
}
