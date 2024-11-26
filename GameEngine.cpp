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
#include "SpecialEvents.h"
#include "Server.h" 

GameEngine::GameEngine() : rng(std::random_device{}()), diceDist(1, 6), currentPlayerIndex(0), hasAfricanStar(false) {
    loadMap();  // Load the game map before assigning disks

    // Initialize the disks vector
    disks = {
        {DiskType::StarOfAfrica, 0},
        {DiskType::Ruby, 1000}, {DiskType::Ruby, 1000},
        {DiskType::Emerald, 600}, {DiskType::Emerald, 600}, {DiskType::Emerald, 600},
        {DiskType::Topaz, 300}, {DiskType::Topaz, 300}, {DiskType::Topaz, 300}, {DiskType::Topaz, 300},
        {DiskType::Robber, 0}, {DiskType::Robber, 0}, {DiskType::Robber, 0},
        {DiskType::Horseshoe, 0}, {DiskType::Horseshoe, 0}, {DiskType::Horseshoe, 0},
        {DiskType::Horseshoe, 0}, {DiskType::Horseshoe, 0},
    };

    // Add 12 Empty disks to make up a total of 30
    for (int i = 0; i < 12; ++i) {
        disks.push_back({DiskType::Empty, 0});
    }

    // Shuffle the disks
    std::shuffle(disks.begin(), disks.end(), rng);

    // Assign disks to cities
    size_t diskIndex = 0;
    for (auto& [name, city] : cities) {
        if (name != "Tangier" && name != "Cairo" && diskIndex < disks.size()) {
            city.disk = &disks[diskIndex];
            std::cout << "Assigning to city: " << name
                      << ", Disk Type: " << static_cast<int>(disks[diskIndex].type)
                      << ", Memory Address: " << city.disk << std::endl;
            diskIndex++;
        } else {
            city.disk = nullptr;  // Ensure no disk in Tangier and Cairo
            std::cout << "Assigning to city: " << name << ", No Disk Assigned." << std::endl;
        }
    }


    // Initialize players
    players.push_back(Player{300}); // Start in Tangier
    players.push_back(Player{301}); // Start in Cairo

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

void GameEngine::interactWithDisk(Player& player, City& city) {
    if (city.disk == nullptr) {
        std::cout << "No disk to open here.\n";
        player.pendingDisk = false; // Reset the pendingDisk flag
        return;
    }

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
            diskOpened = true;
        } else if (option == 2) {
            int attemptRoll = rollDice();
            std::cout << "You rolled a " << attemptRoll << "." << std::endl;
            if (attemptRoll >= 4) {
                revealDisk(*city.disk, player);
                diskOpened = true;
            } else {
                std::cout << "The roll was not enough to open the disk. Try again on your next turn.\n";
                player.pendingDisk = true;  // Set pending flag for next turn
                return;  // End the turn
            }
        } else {
            std::cout << "Invalid option or insufficient funds. Try again.\n";
        }
    }

    // Reset the pendingDisk flag after successful disk interaction
    if (diskOpened) {
        applyDiskInteractionEvent(*this, player, city.name, city.disk);
        city.disk = nullptr;  // Remove the disk from the city after interaction
        player.pendingDisk = false;  // Reset the pending flag
    }
}



// Helper function to check the winning condition
void GameEngine::checkWinningCondition(const Player& player, const City& city) const {
    if (player.hasAfricanStar && (city.name == "Tangier" || city.name == "Cairo")) {
        std::cout << "Player with the African Star reached " << city.name << " and wins the game!" << std::endl;
        exit(0);
    } else if (player.hasHorseshoe && hasAfricanStar && (city.name == "Tangier" || city.name == "Cairo")) {
        std::cout << "Player with the horseshoe reached " << city.name << " and wins the game!" << std::endl;
        exit(0);
    }
}

int GameEngine::handleWaterwayCost(Player& player, const std::vector<int>& path) {
    bool usedWaterRoute = false;
    for (size_t i = 1; i < path.size(); ++i) {
        if (waterways.count({path[i - 1], path[i]}) > 0) {
            usedWaterRoute = true;
            break;
        }
    }

    if (usedWaterRoute) {
        if (player.money >= 100) {
            player.money -= 100;
            std::cout << "Paid £100 to use the water route. Remaining balance: £" << player.money << "\n";
            player.isOnWater = true;
            return path.size();  // Full path
        } else {
            int roll = rollDice();
            std::cout << "You don't have enough money. Rolled a " << roll << "." << std::endl;
            if (roll >= 2) {
                std::cout << "You can travel 2 nodes.\n";
                player.isOnWater = true;
                return 2;  // Limit to 2 nodes
            } else {
                std::cout << "You rolled a 1 and cannot move this turn.\n";
                return 0;  // No movement
            }
        }
    }
    return path.size();  // Full path if no waterway or sufficient funds
}

void GameEngine::displayReachableNodes(int diceRoll, const std::vector<int>& reachableNodes) const {
    std::cout << "With a roll of " << diceRoll << ", you can reach the following nodes:" << std::endl;
    for (size_t i = 0; i < reachableNodes.size(); ++i) {
        int nodeId = reachableNodes[i];

        // Corrected logic for determining if the route is a waterway
        bool isWaterway = false;

        if (nodeId >= 200 && nodeId <= 276) {
            // If the node ID itself is in the range of water nodes, it's a water node
            isWaterway = true;
        } else {
            // Check if the connection between the current player node and this node is a waterway
            if (waterways.count({reachableNodes[0], nodeId}) > 0) {
                isWaterway = true;
            }
        }

        std::cout << i + 1 << ". Node " << nodeId;
        if (isCityNode(nodeId)) {
            std::cout << " (City: " << getCityName(nodeId) << ")";
        }
        std::cout << " [" << (isWaterway ? "Water" : "Land") << "]" << std::endl;
    }
}






void GameEngine::addFlightPath(const std::string& fromCity, const std::string& toCity) {
    if (cities.count(fromCity) && cities.count(toCity)) {
        int fromNode = cities[fromCity].nodeId;
        int toNode = cities[toCity].nodeId;
        flightPaths.insert({fromNode, toNode});
        flightPaths.insert({toNode, fromNode});  // Ensure bidirectional
    } else {
        std::cerr << "Error: One or both cities not found for flight path." << std::endl;
    }
}

bool GameEngine::handleFlightCost(Player& player, int destination) {
    if (flightPaths.count({player.currentNodeId, destination}) > 0) {
        if (player.money >= 300) {
            player.money -= 300;
            std::cout << "Paid £300 to take a flight. Remaining balance: £" << player.money << "\n";
            return true;  // Flight path taken successfully
        } else {
            std::cout << "Insufficient funds for flight path (requires £300). Choose another route.\n";
            return false;  // Insufficient funds for flight
        }
    }
    return true;  // Not a flight path, continue normally
}

void GameEngine::movePlayer(Player& player, int diceRoll) {
    // Check if player has a pending disk interaction
    if (player.pendingDisk) {
        City& city = cities[getCityName(player.currentNodeId)];
        interactWithDisk(player, city);
        if (player.pendingDisk) {
            std::cout << "Ending turn as the player still needs to open the disk.\n";
            return;
        }
    }

    // Check if the player is stuck on a special event node
    if (player.pendingTurns > 0) {
        std::cout << "You are stuck and must wait for your next turn.\n";
        player.pendingTurns--;
        return;
    }

    // Check for the "wait until roll" event
    applyWaitUntilRollEvent(*this, player, player.currentNodeId);
    if (player.pendingTurns > 0) {
        return;  // End the turn if the player is stuck
    }

    std::vector<int> reachableNodes = getReachableNodes(player.currentNodeId, diceRoll);

    // Collect reachable flight paths, filtering by player's available money
    std::vector<int> reachableFlightPaths;
    if (player.money >= 300) {
        for (const auto& [fromNode, toNode] : flightPaths) {
            if (fromNode == player.currentNodeId) {
                reachableFlightPaths.push_back(toNode);
            }
        }
    }

        // Display reachable nodes
    std::cout << "With a roll of " << diceRoll << ", you can reach the following nodes:" << std::endl;
    int optionNumber = 1;
    std::unordered_map<int, std::string> optionsMap;

    // Show land and water routes
    for (int nodeId : reachableNodes) {
        // Determine if the route to this node involves a waterway
        bool isWaterway = false;

        // Get the path to the node
        std::vector<int> path = getPathToNode(player.currentNodeId, nodeId, diceRoll);

        // Check if any part of the path involves a waterway
        for (size_t i = 1; i < path.size(); ++i) {
            if (waterways.count({path[i - 1], path[i]}) > 0) {
                isWaterway = true;
                break;
            }
        }

        std::cout << optionNumber << ". Node " << nodeId;
        if (isCityNode(nodeId)) {
            std::cout << " (City: " << getCityName(nodeId) << ")";
        }
        std::cout << " [" << (isWaterway ? "Water" : "Land") << "]\n";
        optionsMap[optionNumber++] = "route_" + std::to_string(nodeId);
    }



    // Show flight paths
    for (int nodeId : reachableFlightPaths) {
        std::cout << optionNumber << ". Node " << nodeId;
        if (isCityNode(nodeId)) {
            std::cout << " (City: " << getCityName(nodeId) << ")";
        }
        std::cout << " [Flight Path, £300]\n";
        optionsMap[optionNumber++] = "flight_" + std::to_string(nodeId);
    }

    // Ask the player for their choice
    int choice;
    std::cout << "Choose your destination (1-" << optionsMap.size() << "): ";
    std::cin >> choice;

    if (choice > 0 && choice <= static_cast<int>(optionsMap.size())) {
        std::string selectedOption = optionsMap[choice];
        int destinationNode = std::stoi(selectedOption.substr(selectedOption.find('_') + 1));

        if (selectedOption.find("flight") == 0) {
            // Handle flight path
            if (!handleFlightCost(player, destinationNode)) {
                return;  // Skip turn if player can't afford the flight
            }
        } else {
            // Handle waterway cost
            std::vector<int> path = getPathToNode(player.currentNodeId, destinationNode, diceRoll);
            if (!handleWaterwayCost(player, path)) {
                return;  // Skip turn if player can't move due to waterway rules
            }
        }

        // Move the player to the selected destination
        player.currentNodeId = destinationNode;

        if (isCityNode(player.currentNodeId)) {
            City& city = cities[getCityName(player.currentNodeId)];
            std::cout << "Player reached city: " << city.name << std::endl;

            //Special Events
            applySpecialEvent(*this, player, city.name);


            interactWithDisk(player, city);
            checkWinningCondition(player, city);
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

    queue.push({startNode, steps});
    visited[startNode] = steps;

    while (!queue.empty()) {
        int currentNode = queue.front().first;
        int stepsLeft = queue.front().second;
        queue.pop();

        // If it's a city and within the remaining steps, add it immediately
        if (isCityNode(currentNode) && stepsLeft >= 0) {
            reachableNodes.push_back(currentNode);
        }

        // If there are no steps left, add only the current node
        if (stepsLeft == 0) {
            reachableNodes.push_back(currentNode);
            continue;
        }

        // Explore land connections
        for (int neighbor : nodes[currentNode].neighbors) {
            if (visited.find(neighbor) == visited.end() || visited[neighbor] < stepsLeft - 1) {
                visited[neighbor] = stepsLeft - 1;
                queue.push({neighbor, stepsLeft - 1});
            }
        }

        // Explore waterway connections
        for (const auto& connection : waterways) {
            int waterNeighbor = -1;
            if (connection.first == currentNode) {
                waterNeighbor = connection.second;
            } else if (connection.second == currentNode) {
                waterNeighbor = connection.first;
            }

            if (waterNeighbor != -1 && (visited.find(waterNeighbor) == visited.end() || visited[waterNeighbor] < stepsLeft - 1)) {
                visited[waterNeighbor] = stepsLeft - 1;
                queue.push({waterNeighbor, stepsLeft - 1});
            }
        }
    }

    // Remove duplicate nodes and cities from the reachableNodes list
    std::sort(reachableNodes.begin(), reachableNodes.end());
    reachableNodes.erase(std::unique(reachableNodes.begin(), reachableNodes.end()), reachableNodes.end());

    return reachableNodes;
}


// Function to get the path from startNode to endNode with a limited number of steps
std::vector<int> GameEngine::getPathToNode(int startNode, int endNode, int steps) {
    std::unordered_map<int, int> parent;
    std::queue<std::pair<int, int>> queue;
    parent[startNode] = -1;
    queue.push({startNode, steps});

    // Breadth-First Search to find the path
    while (!queue.empty()) {
        int currentNode = queue.front().first;
        int stepsLeft = queue.front().second;
        queue.pop();

        // If we reach the destination node, construct the path
        if (currentNode == endNode) {
            std::vector<int> path;
            for (int node = endNode; node != -1; node = parent[node]) {
                path.push_back(node);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // If no steps are left, skip further exploration
        if (stepsLeft == 0) continue;

        // Explore neighbors (land connections)
        for (int neighbor : nodes[currentNode].neighbors) {
            if (parent.find(neighbor) == parent.end()) {  // Not visited
                parent[neighbor] = currentNode;
                queue.push({neighbor, stepsLeft - 1});
            }
        }

        // Explore waterways
        for (const auto& connection : waterways) {
            int waterNeighbor = -1;
            if (connection.first == currentNode) {
                waterNeighbor = connection.second;
            } else if (connection.second == currentNode) {
                waterNeighbor = connection.first;
            }

            if (waterNeighbor != -1 && parent.find(waterNeighbor) == parent.end()) {
                parent[waterNeighbor] = currentNode;
                queue.push({waterNeighbor, stepsLeft - 1});
            }
        }
    }

    // If no path is found, return an empty vector
    return {};
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
    std::cout << "Player's new balance: " << player.money << std::endl;
}

Player& GameEngine::getCurrentPlayer() {
    return players[currentPlayerIndex];
}

void GameEngine::nextTurn() {
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    } while (players[currentPlayerIndex].pendingTurns-- > 0);  // Skip turns if player has pending turns
}

const std::unordered_map<std::string, City>& GameEngine::getCities() const {
    return cities;
}



int GameEngine::getCurrentPlayerIndex() const {
    return currentPlayerIndex;
}

const std::vector<Disk>& GameEngine::getDisks() const {
    return disks;
}


