#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_ROWS = 10; // Maximális sorok száma a labirintusban
const int MAX_COLS = 10; // Maximális oszlopok száma a labirintusban

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

void drawMaze(char maze[MAX_ROWS][MAX_COLS], Character player, Enemy enemy)
{
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; j++)
		{
			if (i == player.row && j == player.col)
			{
				cout << "P"; // Játékos karakter
			}
			else if (i == enemy.row && j == enemy.col)
			{
				cout << "E"; // Ellenség karakter
			}
			else
			{
				cout << maze[i][j];
			}
		}
		cout << endl;
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
	Enemy enemy("Enemy", 50, 8, 2);

	cout << "Welcome to the Console RPG Game!" << endl;

	while (player.isAlive())
	{
		cout << "---------------------------" << endl;
		player.printStats();
		drawMaze(maze, player, enemy);

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
			player.row = newRow;
			player.col = newCol;
		}

		int playerDamage = player.calculateDamage();
		int enemyDamage = enemy.calculateDamage();

		cout << "Player attacks the enemy for " << playerDamage << " damage." << endl;
		enemy.takeDamage(playerDamage);
		if (!enemy.isAlive())
		{
			cout << "Enemy defeated!" << endl;
			// Add logic to reward player with stats points
		}
		else
		{
			cout << "Enemy attacks the player for " << enemyDamage << " damage." << endl;
			player.takeDamage(enemyDamage);
			if (!player.isAlive())
			{
				cout << "You were defeated!" << endl;
				break;
			}
		}
	}

	return 0;
}
