// GameMap.cpp
#include "GameMap.h"
#include "GameEngine.h"


void loadGameMap(GameEngine& gameEngine) {
    gameEngine.addCity("Start1", 100);
    gameEngine.addCity("Start2", 101);
    gameEngine.addCity("City1", 102);
    gameEngine.addCity("City2", 103);
    gameEngine.addCity("City3", 104);
    gameEngine.addCity("City4", 105);
    gameEngine.addCity("City5", 106);
    gameEngine.addCity("City6", 107);
    gameEngine.addCity("City7", 108);
    gameEngine.addCity("City8", 109);

    gameEngine.addNode(1);
    gameEngine.addNode(2);
    gameEngine.addNode(3);
    gameEngine.addNode(4);
    gameEngine.addNode(5);
    gameEngine.addNode(6);
    gameEngine.addNode(7);
    gameEngine.addNode(8);
    gameEngine.addNode(9);
    gameEngine.addNode(10);
    gameEngine.addNode(11);
    gameEngine.addNode(12);
    gameEngine.addNode(13);
    gameEngine.addNode(14);
    gameEngine.addNode(15);
    gameEngine.addNode(16);
    gameEngine.addNode(17);
    gameEngine.addNode(18);
    gameEngine.addNode(19);
    gameEngine.addNode(20);
    gameEngine.addNode(21);
    gameEngine.addNode(22);

    // Reittien lisääminen
    gameEngine.addConnection(100, 1);
    gameEngine.addConnection(1, 2);
    gameEngine.addConnection(2, 3);
    gameEngine.addConnection(3, 4);
    gameEngine.addConnection(4, 5);
    gameEngine.addConnection(4, 6);
    gameEngine.addConnection(5, 101);
    gameEngine.addConnection(3, 103);
    gameEngine.addConnection(6, 102);
    gameEngine.addConnection(102, 9);
    gameEngine.addConnection(9, 8);
    gameEngine.addConnection(8, 7);
    gameEngine.addConnection(103, 7);
    gameEngine.addConnection(103, 10);
    gameEngine.addConnection(4, 6);
    gameEngine.addConnection(10, 11);
    gameEngine.addConnection(11, 12);
    gameEngine.addConnection(12, 109);
    gameEngine.addConnection(12, 104);
    gameEngine.addConnection(104, 13);
    gameEngine.addConnection(13, 14);
    gameEngine.addConnection(14, 109);
    gameEngine.addConnection(14, 15);
    gameEngine.addConnection(15, 105);
    gameEngine.addConnection(105, 7);
    gameEngine.addConnection(105, 16);
    gameEngine.addConnection(16, 17);
    gameEngine.addConnection(17, 18);
    gameEngine.addConnection(18, 19);
    gameEngine.addConnection(19, 106);
    gameEngine.addConnection(19, 20);
    gameEngine.addConnection(20, 21);
    gameEngine.addConnection(21, 107);
    gameEngine.addConnection(21, 105);
    gameEngine.addConnection(21, 22);
    gameEngine.addConnection(22, 108);
}
