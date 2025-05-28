// Header Files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "DungeonC.h"

// Variable Declarations
const int row = 24;
const int column = 24;
int command;
int turn = 0;
int* turnPtr = &turn;
int enemyCount = 0;
int* enemyCountPtr = &enemyCount;
Enemy enemies[128];

char input[10];

Player player;
Player* playerPtr = &player;

int main() {

	int map[row][column];

	player.x = column / 2;
	player.y = row / 2;
	player.health = 100;

	initializeMap(map, playerPtr, turnPtr);

	titleScreen();
	// Primary Game Loop
	while ( 1 == 1 ) {
		
		spawnEnemies(enemyCountPtr, turn, enemies, map, row, column);
		
		updateMap(row, column, player, map, enemies);
		
		printScreen(map, turn, player, enemyCount);
	
		command = takeInput(input);
		
		if (command == 0) { break; } // End Game
		
		playerTurn(command, map, playerPtr, enemies, turnPtr);
		
		enemyTurn(enemies, playerPtr, enemyCountPtr, map);
		// system("pause"); // DEBUG MODE
		system("cls");
		
		if (player.health <= 0) {
			endScreen(turn);
			break;
		}
	}

	return 0;
}

void initializeMap(int map[row][column], Player* playerPtr, int* turnPtr) {
	
	for(int i = 0; i < row; i++) {
		for(int j = 0; j < column; j++) {
			map[i][j] = 1;
		}
	}
	
	for(int i = row - 2; i > 0; i--) {
		for(int j = column - 2; j > 0; j--) {
			map[i][j] = 0;
		}
	}
	
	*turnPtr = 1;
}

void printScreen(int map[row][column], int turn, Player player, int enemyCount) {
	
	printf("   Turn: %i   Enemies Remaining: %i   Health: %i\n", turn, enemyCount, player.health);
	
	for(int i = 0; i < row; i++) {
		for(int j = 0; j < column; j++) {

			switch (map[i][j]) {
				case -2:
					printf(" X ");
					break;
				case -1:
					printf(" @ ");
					break;
				case 0:
					printf(" . ");
					break;
				case 1:
					printf("[#]");
					break;
			}
		}
		printf("\n");
	}
}

int takeInput(char input[10]) {

	while ( 1 == 1 ) {
		printf("Next Input...[Type 'controls' for help]\n");
		scanf("%s", input);

		if (strcmp(input, "end") == 0) { // End Program
			return 0;
		} else if (strcmp(input, "w") == 0) { // Handle Movement
			return 1;
		} else if (strcmp(input, "a") == 0) {
			return 2;
		} else if (strcmp(input, "s") == 0) {
			return 3;
		} else if (strcmp(input, "d") == 0) {
			return 4;
		} else if (strcmp(input, "W") == 0) { // Handle Attacks
			return 5;
		} else if (strcmp(input, "A") == 0) {
			return 6;
		} else if (strcmp(input, "S") == 0) {
			return 7;
		} else if (strcmp(input, "D") == 0) {
			return 8;
		} else if (strcmp(input, "h") == 0) {
			return 9;
		} else if (strcmp(input, "controls") == 0) {
			printf("	Controls(Case-Sensitive):\n");
			printf("		'w' => Walk Up         ||   'W' => Attack Up\n");
			printf("		'a' => Walk Left       ||   'A' => Attack Left\n");
			printf("		's' => Walk Down       ||   'S' => Attack Down\n");
			printf("		'd' => Walk Right      ||   'D' => Attack Right\n");
			printf("		'h' => Hold Location   ||   'controls' => See Controls\n");
			printf("		'end' => Close Game    ||\n");
		} else {
			printf("Invalid Input . . . ");
		}
	}
	
}

void playerTurn(int command, int map[row][column], Player* playerPtr, Enemy enemies[128], int* turnPtr) {
	
	switch (command) {
		case 1:
			// Move Up
			if (map[playerPtr->y - 1][playerPtr->x] == 0) {
				playerPtr->y -= 1;
			}
		break;
		case 2:
			// Move Left
			if (map[playerPtr->y][playerPtr->x - 1] == 0) {
				playerPtr->x -= 1;
			}
		break;
		case 3:
			// Move Down
			if (map[playerPtr->y + 1][playerPtr->x] == 0) {
				playerPtr->y += 1;
			}
		break;
		case 4:
			// Move Right
			if (map[playerPtr->y][playerPtr->x + 1] == 0) {
				playerPtr->x += 1;
			}
		case 5:
			// Attack Up
			for (int i = 0; i < 128; i++) {
				if ((enemies[i].y == playerPtr->y - 1) && (enemies[i].x == playerPtr->x)) {
					enemies[i].health -= 5;
					enemies[i].stunned = true;
					break;
				}
			}
		break;
		case 6:
			// Attack Left
			for (int i = 0; i < 128; i++) {
				if ((enemies[i].y == playerPtr->y) && (enemies[i].x == playerPtr->x - 1)) {
					enemies[i].health -= 5;
					enemies[i].stunned = true;
					break;
				}
			}
		break;
		case 7:
			// Attack Down
			for (int i = 0; i < 128; i++) {
				if ((enemies[i].y == playerPtr->y + 1) && (enemies[i].x == playerPtr->x)) {
					enemies[i].health -= 5;
					enemies[i].stunned = true;
					break;
				}
			}
		break;
		case 8:
			// Attack Right
			for (int i = 0; i < 128; i++) {
				if ((enemies[i].y == playerPtr->y) && (enemies[i].x == playerPtr->x + 1)) {
					enemies[i].health -= 5;
					enemies[i].stunned = true;
					break;
				}
			}
		break;
		case 9:
			// Player Hold Position
		break;
		default:
		break;
	};
	
	map[playerPtr->y][playerPtr->x] = -1;
	
	*turnPtr += 1;
}

void spawnEnemies(int* enemyCountPtr, int turn, Enemy enemies[128], int map[row][column], int row, int column) {
	int xPos;
	int yPos;
	
	if (*enemyCountPtr == 0) {
		*enemyCountPtr = (rand() % 10) + 1;
		
		for (int i = 0; i < *enemyCountPtr; i++) {
			xPos =(rand() % (column - 2)) + 1;
			yPos =(rand() % (row - 2)) + 1;
			
			enemies[i].x = xPos;
			enemies[i].y = yPos;
			enemies[i].health = 10;
			enemies[i].alive = true;
			enemies[i].stunned = false;
		}
	}
}

void enemyTurn(Enemy enemies[128], Player* playerPtr, int* enemyCountPtr, int map[row][column]) {
	int dX;
	int dY;
	bool isAttacking;
	int direction;
	int adjacents[4];
	
	for (int i = 0; i < 128; i++) {
		
		isAttacking = false;
		
		if (enemies[i].health <= 0) {
			enemies[i].alive = false;
		}
		
		if (enemies[i].alive && !enemies[i].stunned) {
	
			adjacents[0] = map[enemies[i].y - 1][enemies[i].x];
			adjacents[1] = map[enemies[i].y + 1][enemies[i].x];
			adjacents[2] = map[enemies[i].y][enemies[i].x - 1];
			adjacents[3] = map[enemies[i].y][enemies[i].x + 1];
		
			for (int i = 0; i < 4; i++) {
				if (adjacents[i] == -1) {
					isAttacking = true;
				}
			}

			if (isAttacking == false) {

				map[enemies[i].y][enemies[i].x] = 0;

				dX = playerPtr->x - enemies[i].x;
				dY = playerPtr->y - enemies[i].y;

				if (dX != 0) { 
					dX = dX / abs(dX); 
				}
			
				if (dY != 0) { 
					dY = dY / abs(dY); 
				}
				
				if (dX == 0) {
					direction = 1;
				} else if (dY == 0) {
					direction = 0;
				} else {
					direction = (rand() % 2);
				}

				if((dX == 1) && (direction == 0) && (adjacents[3] == 0)) {
					enemies[i].x += dX;
				} else if ((dX == -1) && (direction == 0) && (adjacents[2] == 0)){
					enemies[i].x += dX;
				}
				
				if((dY == 1) && (direction == 1) && (adjacents[1] == 0)) {
					enemies[i].y += dY;
				} else if ((dY == -1) && (direction == 1) && (adjacents[0] == 0)) {
					enemies[i].y += dY;
				}

				map[enemies[i].y][enemies[i].x] = -2;
				
			} else { // if (isAttacking == true)
				playerPtr->health -= 5;
			}
		} else if (enemies[i].alive && enemies[i].stunned) { 
			// Skip Turn Stunned
			enemies[i].stunned = false;
		} else if (!enemies[i].alive && enemies[i].stunned) {
			// Mark as Dead and Remove
			enemies[i].x = 0;
			enemies[i].y = 0;
			enemies[i].stunned = false;
			*enemyCountPtr -= 1;
		} // !alive and !stunned is intert state
	}
}

void updateMap(int row, int column, Player player, int map[row][column], Enemy enemies[128]) {
	
	for(int i = row - 2; i > 0; i--) {
		for(int j = column - 2; j > 0; j--) {
			map[i][j] = 0;
		}
	}

	map[player.y][player.x] = -1;
	
	for (int i = 0; i < 128; i++) {
		if ((enemies[i].x) && (enemies[i].y != 0)) {
			map[enemies[i].y][enemies[i].x] = -2;
		}
	}
}

void titleScreen() {

	printf("	Welcome To\n");
	printf("-------------------------------------------------------------------------------\n");
	printf("        ____                                                         ______\n");
	printf("       / __ \\  __  __   ____    ____ _  ___   ____    ____          / ____/\n");
	printf("      / / / / / / / /  / __ \\  / __ `/ / _ \\ / __ \\  / __ \\        / /     \n");
	printf("     / /_/ / / /_/ /  / / / / / /_/ / /  __// /_/ / / / / /       / /___   \n");  
	printf("    /_____/  \\__,_/  /_/ /_/  \\__, /  \\___/ \\____/ /_/ /_/        \\____/   \n");
	printf("                             /____/                                        \n");
	printf("-------------------------------------------------------------------------------\n");
	printf("	A Console Powered Survival Game\n");
	printf("\n");
	printf("	Controls(Case-Sensitive):\n");
	printf("		'w' => Walk Up         ||   'W' => Attack Up\n");
	printf("		'a' => Walk Left       ||   'A' => Attack Left\n");
	printf("		's' => Walk Down       ||   'S' => Attack Down\n");
	printf("		'd' => Walk Right      ||   'D' => Attack Right\n");
	printf("		'h' => Hold Location   ||   'controls' => See Controls\n");
	printf("		'end' => Close Game    ||\n");
	printf("\n");
	printf("	Map Key:\n");
	printf("		'[#]' => Walls     ||   ' . ' => Open Floor\n");
	printf("		' X ' => Enemies   ||   ' @ ' => The Adventurer(You)\n");
	printf("\n");
	printf("	Rules:\n");
	printf("		Both Players and Enemies can only make one move or attack per turn.\n");
	printf("		Both Players and Enemies can only move vertically or horizontally.\n");
	printf("		Enemies will always move in a straight line if possible.\n");
	printf("		Enemies cannot attack after being attacked by the player.\n");
	printf("		New Enemies will appear if all their comrades are felled.\n");
	printf("		The game will end if the Player's health goes below Zero.\n");
	printf("		(Suggestion)Resize the console so the entire title screen fits.\n");
	printf("\n");
	printf("	This dungeon has felled adventurers far greater than you... Good Luck\n");
	printf("\n");
	system("pause");
	system("cls");
}

void endScreen(int turn) {
	printf("	Thank you for playing\n");
	printf("	You died on turn: %i\n", turn);
	printf("-------------------------------------------------------------------------------\n");
	printf("        ____                                                         ______\n");
	printf("       / __ \\  __  __   ____    ____ _  ___   ____    ____          / ____/\n");
	printf("      / / / / / / / /  / __ \\  / __ `/ / _ \\ / __ \\  / __ \\        / /     \n");
	printf("     / /_/ / / /_/ /  / / / / / /_/ / /  __// /_/ / / / / /       / /___   \n");  
	printf("    /_____/  \\__,_/  /_/ /_/  \\__, /  \\___/ \\____/ /_/ /_/        \\____/   \n");
	printf("                             /____/                                        \n");
	printf("-------------------------------------------------------------------------------\n");
	printf("	Dungeon C was created in 3 days with no prior knowledge of the C language\n");
	printf("	using only standard C and Windows OS files. I hope you enjoyed. Play again\n");
	printf("	sometime soon.\n");
	printf("		- The Developer\n");
	printf("\n");
	system("pause");
}