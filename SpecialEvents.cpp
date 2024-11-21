#include "SpecialEvents.h"
#include <iostream>

void applyWaitUntilRollEvent(GameEngine& game, Player& player, int nodeId) {
    if (nodeId == 27 || nodeId == 222 || nodeId == 270) {
        int diceRoll = game.rollDice();
        std::cout << "Special Event: You're stuck on node " << nodeId 
                  << "! You must roll a 1 or 2 to leave. You rolled a " << diceRoll << ".\n";

        if (diceRoll == 1 || diceRoll == 2) {
            std::cout << "You rolled a " << diceRoll << "! You can now move on your next turn.\n";
        } else {
            std::cout << "You didn't roll a 1 or 2. You must wait until your next turn.\n";
            player.pendingTurns = 1;  // Skip the player's movement this turn
        }
    }
}
// Flag to track if the Capetown event has been triggered
bool capetownBonusGiven = false;

void applySpecialEvent(GameEngine& game, Player& player, const std::string& cityName) {
    if (cityName == "Capetown") {
        if (!capetownBonusGiven) {
            std::cout << "Special Event: You are the first player to reach Capetown! You receive £500.\n";
            player.money += 500;
            capetownBonusGiven = true;  // Mark the bonus as given
        } else {
            std::cout << "Capetown event already triggered. No bonus awarded.\n";
        }
    }
    // Other arrival-based events can be added here...
}

void applyDiskInteractionEvent(GameEngine& game, Player& player, const std::string& cityName, Disk* disk) {
    if (cityName == "Gold Coast" && disk != nullptr) {
        // If the player finds a valuable diamond at Gold Coast, they receive double the value
        if (disk->type == DiskType::Topaz || disk->type == DiskType::Emerald || disk->type == DiskType::Ruby) {
            std::cout << "Special Event: You found a valuable diamond at Gold Coast! You receive double the money!\n";
            player.money += disk->value;  // Add an additional amount equal to the disk value
        }
    }
    else if (cityName == "Slave Coast" && disk != nullptr) {
        // If the player finds an empty disk in Slave Coast, they must stay there for 3 turns
        if (disk->type == DiskType::Empty) {
            std::cout << "Special Event: You found a blank disk in Slave Coast! You must stay here for the next 3 turns.\n";
            player.pendingTurns = 3;  // Set the player's skipped turns to 3
        }
    }
    // Other disk interaction events can be added here...
}
