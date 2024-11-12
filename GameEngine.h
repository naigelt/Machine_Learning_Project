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
    bool hasAfricanStar = false;
    bool hasHorseshoe = false;
    bool isOnWater = false;  // Tracks if the player is currently on a waterway
    bool pendingDisk = false;  // Tracks if player is waiting to try opening a disk again
    
};





class GameEngine {
public:
    GameEngine();
    int rollDice();
    void movePlayer(Player& player, int diceRoll);
    void loadMap();  // Load the map
    void addWaterway(int node1, int node2);
    std::vector<int> getPathToNode(int startNode, int endNode, int steps);
    void revealDisk(const Disk& disk, Player& player);  // Reveal the disk content
    Player& getCurrentPlayer();  // Returns the current player
    void nextTurn();  // Switches to the next player's turn
    int getCurrentPlayerIndex() const;  // Returns the index of the current player

    friend void loadGameMap(GameEngine& gameEngine);

private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<std::string, City> cities;
    std::mt19937 rng;
    std::uniform_int_distribution<int> diceDist;
    std::set<std::pair<int, int>> waterways;  // Set of waterway connections
    
    std::vector<Player> players;  // List of players
    int currentPlayerIndex;  // Indicates whose turn it is
    bool hasAfricanStar = false;  // True if any player has found the African Star

    std::vector<int> getReachableNodes(int startNode, int steps);
    bool isCityNode(int nodeId) const;
    std::string getCityName(int nodeId) const;
    void addNode(int id);
    void addCity(const std::string& name, int nodeId);
    void addConnection(int node1, int node2);

    // New helper functions
    void checkWinningCondition(const Player& player, const City& city) const;
    void interactWithDisk(Player& player, City& city);
    int handleWaterwayCost(Player& player, const std::vector<int>& path);
    void displayReachableNodes(int diceRoll, const std::vector<int>& reachableNodes) const;
};
