#pragma once
#include <string>
#include "GameEngine.h"

void applySpecialEvent(GameEngine& game, Player& player, const std::string& cityName);
void applyDiskInteractionEvent(GameEngine& game, Player& player, const std::string& cityName, Disk* disk);
void applyWaitUntilRollEvent(GameEngine& game, Player& player, int nodeId);
