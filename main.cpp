#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_ROWS = 10;   // Maximális sorok száma a labirintusban
const int MAX_COLS = 10;   // Maximális oszlopok száma a labirintusban
const int MAX_ENEMIES = 3; // Maximális ellenségek száma

class Character
{
public:
	string name;
	int health;
	int attack;
	int defense;
	int row;
	int col;

	Character(string n, int h, int a, int d)
	{
		name = n;
		health = h;
		attack = a;
		defense = d;
		row = 1; // Kezdő pozíció sor indexe
		col = 1; // Kezdő pozíció oszlop indexe
	}

	void printStats()
	{
		cout << "Name: " << name << endl;
		cout << "Health: " << health << endl;
		cout << "Attack: " << attack << endl;
		cout << "Defense: " << defense << endl;
		cout << endl;
	}

	void takeDamage(int damage)
	{
		int actualDamage = damage - defense;
		if (actualDamage > 0)
		{
			health -= actualDamage;
		}
	}

	bool isAlive()
	{
		return health > 0;
	}

	int calculateDamage()
	{
		return rand() % attack + 1;
	}
};

class Enemy : public Character
{
public:
	Enemy(string n, int h, int a, int d) : Character(n, h, a, d) {}
};

void drawMaze(char maze[MAX_ROWS][MAX_COLS], Character player, Enemy enemies[], int numEnemies)
{
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; j++)
		{
			bool isEnemy = false;
			for (int k = 0; k < numEnemies; k++)
			{
				if (i == enemies[k].row && j == enemies[k].col)
				{
					cout << "E"; // Ellenség karakter
					isEnemy = true;
					break;
				}
			}
			if (!isEnemy)
			{
				if (i == player.row && j == player.col)
				{
					cout << "P"; // Játékos karakter
				}
				else
				{
					cout << maze[i][j];
				}
			}
		}
		cout << endl;
	}
}

void placeEnemies(Enemy enemies[], int numEnemies, char maze[MAX_ROWS][MAX_COLS])
{
	int placedEnemies = 0;
	while (placedEnemies < numEnemies)
	{
		int row = rand() % MAX_ROWS;
		int col = rand() % MAX_COLS;
		if (maze[row][col] == ' ')
		{
			enemies[placedEnemies].row = row;
			enemies[placedEnemies].col = col;
			placedEnemies++;
		}
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	char maze[MAX_ROWS][MAX_COLS] = {
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', '#', ' ', '#', ' ', '#', '#', ' ', '#'},
		{'#', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', '#'},
		{'#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', '#'},
		{'#', ' ', '#', '#', '#', '#', '#', ' ', ' ', '#'},
		{'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', '#'},
		{'#', ' ', ' ', ' ', '#', '#', '#', ' ', '#', '#'},
		{'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}};

	Character player("Player", 100, 10, 5);
	Enemy enemies[MAX_ENEMIES] = {
		Enemy("Enemy1", 50, 8, 2),
		Enemy("Enemy2", 60, 7, 3),
		Enemy("Enemy3", 70, 6, 4)};

	placeEnemies(enemies, MAX_ENEMIES, maze);

	cout << "Welcome to the Console RPG Game!" << endl;

	while (player.isAlive())
	{
		cout << "---------------------------" << endl;
		player.printStats();
		drawMaze(maze, player, enemies, MAX_ENEMIES);

		int choice;
		cout << "Choose an action:" << endl;
		cout << "1. Move Up" << endl;
		cout << "2. Move Down" << endl;
		cout << "3. Move Left" << endl;
		cout << "4. Move Right" << endl;
		cout << "5. Quit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		int newRow = player.row;
		int newCol = player.col;

		switch (choice)
		{
		case 1: // Move Up
			newRow--;
			break;
		case 2: // Move Down
			newRow++;
			break;
		case 3: // Move Left
			newCol--;
			break;
		case 4: // Move Right
			newCol++;
			break;
		case 5: // Quit
			cout << "Game over. You quit the game." << endl;
			return 0;
		default:
			cout << "Invalid choice. Try again." << endl;
			continue;
		}

		if (maze[newRow][newCol] != '#')
		{
			bool validMove = true;
			for (int i = 0; i < MAX_ENEMIES; i++)
			{
				if (newRow == enemies[i].row && newCol == enemies[i].col)
				{
					validMove = false;
					break;
				}
			}
			if (validMove)
			{
				player.row = newRow;
				player.col = newCol;
			}
		}

		int playerDamage = player.calculateDamage();
		for (int i = 0; i < MAX_ENEMIES; i++)
		{
			if (player.row == enemies[i].row && player.col == enemies[i].col)
			{
				cout << "Player attacks enemy " << i + 1 << " for " << playerDamage << " damage." << endl;
				enemies[i].takeDamage(playerDamage);
				if (!enemies[i].isAlive())
				{
					cout << "Enemy " << i + 1 << " defeated!" << endl;
					// Add logic to reward player with stats points
				}
				break;
			}
		}

		for (int i = 0; i < MAX_ENEMIES; i++)
		{
			if (player.row == enemies[i].row && player.col == enemies[i].col && enemies[i].isAlive())
			{
				int enemyDamage = enemies[i].calculateDamage();
				cout << "Enemy " << i + 1 << " attacks the player for " << enemyDamage << " damage." << endl;
				player.takeDamage(enemyDamage);
				if (!player.isAlive())
				{
					cout << "You were defeated!" << endl;
					break;
				}
			}
		}
	}

	return 0;
}
