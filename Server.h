#pragma once

#include "GameEngine.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif
#include <string>

class Server {
public:
    explicit Server(GameEngine& engine);
    void start(int port);
private:
    SOCKET serverSocket;
    GameEngine& gameEngine;

    void handleClient(SOCKET clientSocket);
    std::string handleCommand(const std::string& command);
    void cleanup();
};
