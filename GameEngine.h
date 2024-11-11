#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <random>
#include <set>

enum class DiskType {
    Empty,
    Horseshoe,
    Robber,
    Topaz,
    Emerald,
    Ruby,
    StarOfAfrica
};

struct Disk {
    DiskType type;
    int value;  // Jalokivien arvo (topaasi 300, smaragdi 600, rubiini 1000), muille 0
};

struct City {
    std::string name;
    int nodeId;
    Disk* disk = nullptr;  // Osoitin kiekkoon; nullptr tarkoittaa, ettei kaupungissa ole kiekkoa
};


struct Node {
    int id;
    std::vector<int> neighbors;
};

struct Player {
    int currentNodeId;
    int money = 300;
    bool hasAfricanStar = false;  // Indicates if the player has the African Star
    bool hasHorseshoe = false;    // Indicates if the player has a horseshoe
};



class GameEngine {
public:
    GameEngine();
    int rollDice();
    void movePlayer(Player& player, int diceRoll);
    void loadMap();  // Lataa kartta
    void addWaterway(int node1, int node2);
    

    void revealDisk(const Disk& disk, Player& player);  // Julistus kiekon paljastamiseksi

    Player& getCurrentPlayer();  // Palauttaa nykyisen pelaajan
    void nextTurn();  // Vaihtaa vuoron seuraavalle pelaajalle
    int getCurrentPlayerIndex() const;  // Palauttaa nykyisen pelaajan vuoronumeron

    friend void loadGameMap(GameEngine& gameEngine);

private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<std::string, City> cities;
    std::mt19937 rng;
    std::uniform_int_distribution<int> diceDist;
    std::set<std::pair<int, int>> waterways;  // Set of waterway connections
    
    std::vector<Player> players;  // Pelaajien lista
    int currentPlayerIndex;  // Kenen vuoro on
    bool hasAfricanStar = false;  // True if any player has found the African Star

    std::vector<int> getReachableNodes(int startNode, int steps);
    bool isCityNode(int nodeId) const;
    std::string getCityName(int nodeId) const;
    void addNode(int id);
    void addCity(const std::string& name, int nodeId);
    void addConnection(int node1, int node2);
};