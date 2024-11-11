#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <random>
#include <ctime>
#include <queue>
#include <set>
#include "GameMap.h" 
#include "GameEngine.h"

GameEngine::GameEngine() : rng(std::random_device{}()), diceDist(1, 6), currentPlayerIndex(0), hasAfricanStar(false) {
    loadMap();  // Load the game map before assigning disks

    
 
    static std::vector<Disk> disks = {
        {DiskType::StarOfAfrica, 0},
        {DiskType::Horseshoe, 0},
        {DiskType::Robber, 0},
        {DiskType::Topaz, 300},
        {DiskType::Emerald, 600},
        {DiskType::Ruby, 1000}
    };

    // Shuffle the disks
    std::shuffle(disks.begin(), disks.end(), rng);

    // Assign disks to cities randomly
    size_t diskIndex = 0;
    for (auto& [name, city] : cities) {
        if (diskIndex < disks.size()) {
            city.disk = &disks[diskIndex];
            diskIndex++;
        } else {
            city.disk = nullptr;
        }
    }

    players.push_back(Player{100}); 
    players.push_back(Player{100}); 

    // Display assigned disks for debugging
    std::cout << "Assigned disks to cities:" << std::endl;
    for (const auto& [name, city] : cities) {
        std::cout << "City: " << name << ", Disk type: ";
        if (city.disk) {
            switch (city.disk->type) {
                case DiskType::Empty: std::cout << "Empty"; break;
                case DiskType::Horseshoe: std::cout << "Horseshoe"; break;
                case DiskType::Robber: std::cout << "Robber"; break;
                case DiskType::Topaz: std::cout << "Topaz"; break;
                case DiskType::Emerald: std::cout << "Emerald"; break;
                case DiskType::Ruby: std::cout << "Ruby"; break;
                case DiskType::StarOfAfrica: std::cout << "StarOfAfrica"; break;
            }
        } else {
            std::cout << "No disk";
        }
        std::cout << std::endl;
    }
}



int GameEngine::rollDice() {
    return diceDist(rng);
}

void GameEngine::movePlayer(Player& player, int diceRoll) {
    std::vector<int> reachableNodes = getReachableNodes(player.currentNodeId, diceRoll);

    std::cout << "With a roll of " << diceRoll << ", you can reach the following nodes:" << std::endl;
    for (size_t i = 0; i < reachableNodes.size(); ++i) {
        int nodeId = reachableNodes[i];
        bool isWaterway = waterways.count({player.currentNodeId, nodeId}) > 0;  // Check if the connection is a waterway
        std::cout << i + 1 << ". Node " << nodeId;
        if (isCityNode(nodeId)) {
            std::cout << " (City: " << getCityName(nodeId) << ")";
        }
        std::cout << " [" << (isWaterway ? "Water" : "Land") << "]" << std::endl;  // Indicate if it’s a waterway
    }

    int choice;
    std::cout << "Choose your destination (1-" << reachableNodes.size() << "): ";
    std::cin >> choice;

    if (choice > 0 && choice <= reachableNodes.size()) {
        int selectedDestination = reachableNodes[choice - 1];
        bool isWaterway = waterways.count({player.currentNodeId, selectedDestination}) > 0;

        // Check if the route is a waterway and if the player can afford it
        if (isWaterway) {
            if (player.money >= 100) {
                player.money -= 100;
                std::cout << "Paid £100 to use the water route. Remaining balance: £" << player.money << "\n";
            } else {
                std::cout << "Not enough money to use the water route. Turn skipped." << std::endl;
                return;  // Exit without moving
            }
        }

        // Update player's position
        player.currentNodeId = selectedDestination;
        if (isCityNode(player.currentNodeId)) {
            City& city = cities[getCityName(player.currentNodeId)];
            std::cout << "Player reached city: " << city.name << std::endl;

            // Check if the city has a disk and let the player interact with it
            if (city.disk != nullptr) {
                bool diskOpened = false;

                while (!diskOpened) {
                    std::cout << "There is a disk here. You can either:\n";
                    if (player.money >= 100) {
                        std::cout << "1. Pay £100 to open the disk\n";
                        std::cout << "2. Wait and try to open the disk with a roll of 4, 5, or 6\n";
                        std::cout << "Choose an option (1 or 2): ";
                    } else {
                        std::cout << "You don't have enough money (£100 required) to open the disk by paying.\n";
                        std::cout << "You can only wait and try to open the disk with a roll of 4, 5, or 6.\n";
                        std::cout << "Press 2 to continue: ";
                    }

                    int option;
                    std::cin >> option;

                    if (option == 1 && player.money >= 100) {
                        player.money -= 100;
                        revealDisk(*city.disk, player);
                        city.disk = nullptr;  // Remove the disk from the city
                        diskOpened = true;
                    } else if (option == 2) {
                        int attemptRoll = rollDice();
                        std::cout << "You rolled a " << attemptRoll << "." << std::endl;
                        if (attemptRoll >= 4) {
                            revealDisk(*city.disk, player);
                            city.disk = nullptr;  // Remove the disk from the city
                            diskOpened = true;
                        } else {
                            std::cout << "The roll was not enough to open the disk. Try again on your next turn.\n";
                            break;  // Player must wait until next turn
                        }
                    } else {
                        std::cout << "Invalid option or insufficient funds. Try again.\n";
                    }
                }
            } else {
                std::cout << "No disk to open here.\n";
            }

            // Check for winning condition
            if (player.hasAfricanStar && (city.name == "Start1" || city.name == "Start2")) {
                std::cout << "Player with the African Star reached " << city.name << " and wins the game!" << std::endl;
                exit(0);
            } else if (player.hasHorseshoe && hasAfricanStar && (city.name == "Start1" || city.name == "Start2")) {
                std::cout << "Player with the horseshoe reached " << city.name << " and wins the game!" << std::endl;
                exit(0);
            }
        } else {
            std::cout << "Player moved to node " << player.currentNodeId << std::endl;
        }
    } else {
        std::cout << "Invalid choice. No movement this turn." << std::endl;
    }
}



std::vector<int> GameEngine::getReachableNodes(int startNode, int steps) {
    std::vector<int> reachableNodes;
    std::unordered_map<int, int> visited;
    std::queue<std::pair<int, int>> queue;
    std::set<int> addedCities;

    queue.push({startNode, steps});
    visited[startNode] = steps;

    while (!queue.empty()) {
        int currentNode = queue.front().first;
        int stepsLeft = queue.front().second;
        queue.pop();

        if (stepsLeft == 0) {
            reachableNodes.push_back(currentNode);
            continue;
        }

        for (int neighbor : nodes[currentNode].neighbors) {
            if (visited.find(neighbor) == visited.end() || visited[neighbor] < stepsLeft - 1) {
                visited[neighbor] = stepsLeft - 1;
                queue.push({neighbor, stepsLeft - 1});
            }
        }
    }

    for (const auto& [name, city] : cities) {
        if (visited.find(city.nodeId) != visited.end() || city.nodeId == startNode) {
            reachableNodes.push_back(city.nodeId);
        }
    }

    return reachableNodes;
}

bool GameEngine::isCityNode(int nodeId) const {
    for (const auto& [name, city] : cities) {
        if (city.nodeId == nodeId) {
            return true;
        }
    }
    return false;
}

std::string GameEngine::getCityName(int nodeId) const {
    for (const auto& [name, city] : cities) {
        if (city.nodeId == nodeId) {
            return name;
        }
    }
    return "Unknown";
}

void GameEngine::addNode(int id) {
    nodes[id] = Node{id, {}};
}

void GameEngine::addCity(const std::string& name, int nodeId) {
    cities[name] = City{name, nodeId};
    addNode(nodeId);
}

void GameEngine::addConnection(int node1, int node2) {
    nodes[node1].neighbors.push_back(node2);
    nodes[node2].neighbors.push_back(node1);
}

void GameEngine::addWaterway(int node1, int node2) {
    waterways.insert({node1, node2});
    waterways.insert({node2, node1});  // Ensure it's bidirectional
}


void GameEngine::loadMap() {
    loadGameMap(*this);
}

void GameEngine::revealDisk(const Disk& disk, Player& player) {
    switch (disk.type) {
        case DiskType::Empty:
            std::cout << "The disk is empty. Nothing happens." << std::endl;
            break;
        case DiskType::Horseshoe:
            if (hasAfricanStar) {
                player.hasHorseshoe = true;
                std::cout << "You found a horseshoe! Now you can race to a starting city to try to win!" << std::endl;
            } else {
                std::cout << "You found a horseshoe, but the African Star hasn't been found yet. No effect." << std::endl;
            }
            break;
        case DiskType::Robber:
            std::cout << "A robber appears! You lose all your money." << std::endl;
            player.money = 0;
            break;
        case DiskType::Topaz:
            std::cout << "You found a Topaz! It is worth 300." << std::endl;
            player.money += 300;
            break;
        case DiskType::Emerald:
            std::cout << "You found an Emerald! It is worth 600." << std::endl;
            player.money += 600;
            break;
        case DiskType::Ruby:
            std::cout << "You found a Ruby! It is worth 1000." << std::endl;
            player.money += 1000;
            break;
        case DiskType::StarOfAfrica:
            std::cout << "You found the Star of Africa! Now race to either Start1 or Start2 to win!" << std::endl;
            player.hasAfricanStar = true;
            hasAfricanStar = true;
            break;
    }
    std::cout << "Player's new balance: £" << player.money << std::endl;
}



Player& GameEngine::getCurrentPlayer() {
    return players[currentPlayerIndex];
}

void GameEngine::nextTurn() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

int GameEngine::getCurrentPlayerIndex() const {
    return currentPlayerIndex;
}

int main() {
    GameEngine game;

    while (true) {
        Player& currentPlayer = game.getCurrentPlayer();

        std::cout << "\nPlayer " << (game.getCurrentPlayerIndex() + 1) << "'s turn." << std::endl;

        int diceRoll = game.rollDice();
        std::cout << "Rolled a " << diceRoll << "." << std::endl;

        game.movePlayer(currentPlayer, diceRoll);

        game.nextTurn();
    }

    return 0;
}
