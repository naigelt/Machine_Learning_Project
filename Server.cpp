#include "Server.h"
#include "GameEngine.h"
#include <iostream>
#include <sstream>
#include <thread>

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

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed." << std::endl;
            continue;
        }

        std::cout << "Client connected." << std::endl;

        std::thread(&Server::handleClient, this, clientSocket).detach();
    }

    cleanup();
}

std::string Server::handleCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string action;
    iss >> action;

    if (action == "ROLL_DICE") {
        int diceRoll = gameEngine.rollDice();
        return "{\"diceRoll\":" + std::to_string(diceRoll) + "}";
    } else if (action == "MOVE_PLAYER") {
        int nodeId;
        iss >> nodeId;
        Player& currentPlayer = gameEngine.getCurrentPlayer();
        gameEngine.movePlayer(currentPlayer, nodeId);
        return "{\"status\":\"Player moved to node " + std::to_string(nodeId) + "\"}";
    } else {
        return "{\"error\":\"Unknown command\"}";
    }
}

void Server::handleClient(SOCKET clientSocket) {
    char buffer[1024];

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::string command(buffer);

        std::string response = handleCommand(command);

        send(clientSocket, response.c_str(), response.size(), 0);
    }

    #ifdef _WIN32
    closesocket(clientSocket);
    #else
    close(clientSocket);
    #endif
}

std::string Server::getGameState() {
    nlohmann::json state;

    state["players"] = nlohmann::json::array();
    for (const auto& player : gameEngine.players) {
        state["players"].push_back({
            {"currentNodeId", player.currentNodeId},
            {"money", player.money},
            {"hasAfricanStar", player.hasAfricanStar},
            {"pendingTurns", player.pendingTurns}
        });
    }

    state["currentPlayerIndex"] = gameEngine.getCurrentPlayerIndex();
    state["options"] = gameEngine.getReachableNodes(
        gameEngine.players[gameEngine.getCurrentPlayerIndex()].currentNodeId,
        gameEngine.rollDice()
    );

    return state.dump();
}

std::string Server::applyAction(const std::string& command) {
    std::istringstream iss(command);
    int selectedNode;
    iss >> selectedNode;

    Player& currentPlayer = gameEngine.getCurrentPlayer();
    auto reachableNodes = gameEngine.getReachableNodes(currentPlayer.currentNodeId, 6); // Max roll for simplicity
    if (std::find(reachableNodes.begin(), reachableNodes.end(), selectedNode) != reachableNodes.end()) {
        gameEngine.movePlayer(currentPlayer, selectedNode);
        // Define a reward mechanism
        int reward = gameEngine.calculateReward(currentPlayer);
        return "{\"status\":\"success\",\"reward\":" + std::to_string(reward) + "}";
    } else {
        return "{\"status\":\"failure\",\"error\":\"Invalid move\"}";
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
