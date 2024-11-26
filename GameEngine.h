#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <random>
#include <set>
#include <nlohmann/json.hpp>

enum class DiskType {
    Empty = 0,
    Horseshoe = 1,
    Robber = 2,
    Topaz = 3,
    Emerald = 4,
    Ruby = 5,
    StarOfAfrica = 6
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
    bool isOnWater = false;
    bool pendingDisk = false;
    int pendingTurns = 0;  // Number of turns the player has to skip
};


class GameEngine {
public:
    nlohmann::json serializeGameState() const;  // Serialize game state to JSON
    const std::unordered_map<std::string, City>& getCities() const;
    GameEngine();
    int rollDice();
    void movePlayer(Player& player, int diceRoll);
    void loadMap();  // Load the map
    void addWaterway(int node1, int node2);
    void addFlightPath(const std::string& fromCity, const std::string& toCity);  // New function to add flight paths
    std::vector<int> getPathToNode(int startNode, int endNode, int steps);
    void revealDisk(const Disk& disk, Player& player);  // Reveal the disk content
    Player& getCurrentPlayer();  // Returns the current player
    void nextTurn();  // Switches to the next player's turn
    int getCurrentPlayerIndex() const;  // Returns the index of the current player
    std::vector<Player> players;
    std::vector<int> getReachableNodes(int startNode, int steps);

    friend void loadGameMap(GameEngine& gameEngine);

private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<std::string, City> cities;
    std::mt19937 rng;
    std::uniform_int_distribution<int> diceDist;
    std::set<std::pair<int, int>> waterways;  // Set of waterway connections
    std::set<std::pair<int, int>> flightPaths;  // Set of flight connections between cities
    std::vector<Disk> disks;  // Keep this as a member of the GameEngine


      // List of players
    int currentPlayerIndex;  // Indicates whose turn it is
    bool hasAfricanStar = false;  // True if any player has found the African Star

    
    bool isCityNode(int nodeId) const;
    std::string getCityName(int nodeId) const;
    void addNode(int id);
    void addCity(const std::string& name, int nodeId);
    void addConnection(int node1, int node2);

    // New helper functions
    void checkWinningCondition(const Player& player, const City& city) const;
    void interactWithDisk(Player& player, City& city);
    int handleWaterwayCost(Player& player, const std::vector<int>& path);
    bool handleFlightCost(Player& player, int destination);
    void displayReachableNodes(int diceRoll, const std::vector<int>& reachableNodes) const;
};
