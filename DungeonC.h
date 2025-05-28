#ifndef DUNGEONC_H
#define DUNGEONC_H
// Header file for DungeonC
// Funtion Declarations
void initializeMap();
void printScreen();
void playerTurn();
void spawnEnemies();
void enemyTurn();
void titleScreen();
void endScreen();
void updateMap();

int takeInput();

// Player Data Structure
typedef struct {
	int x;
	int y;
	int health;
} Player;

typedef struct {
	int x;
	int y;
	int health;
	bool alive;
	bool stunned;
} Enemy;

#endif