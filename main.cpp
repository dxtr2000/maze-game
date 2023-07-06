#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_ROWS = 10;				   // Maximális sorok száma a labirintusban
const int MAX_COLS = 10;				   // Maximális oszlopok száma a labirintusban
const int MAX_ENEMIES = 3;				   // Maximális ellenségek száma
const int BASIC_ATTACK_ENERGY_COST = 20;   // Az alap támadás energiaköltsége
const int SPECIAL_ATTACK_ENERGY_COST = 50; // A speciális támadás energiaköltsége
const int ENERGY_REGEN_AMOUNT = 10;		   // Az energia töltésének mennyisége

class Character
{
public:
	string name;
	int health;
	int attack;
	int defense;
	int energy;
	int row;
	int col;

	Character(string n, int h, int a, int d, int e, int r, int c)
	{
		name = n;
		health = h;
		attack = a;
		defense = d;
		energy = e;
		row = r;
		col = c;
	}

	void printStats()
	{
		cout << "Name: " << name << endl;
		cout << "Health: " << health << endl;
		cout << "Attack: " << attack << endl;
		cout << "Defense: " << defense << endl;
		cout << "Energy: " << energy << endl;
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

	void basicAttack(Character &enemy)
	{
		int damage = calculateDamage();
		enemy.takeDamage(damage);
		energy -= BASIC_ATTACK_ENERGY_COST;
		cout << name << " performs a basic attack on " << enemy.name << " for " << damage << " damage." << endl;
		cout << "Energy decreased by " << BASIC_ATTACK_ENERGY_COST << "." << endl;
	}

	void specialAttack(Character &enemy)
	{
		if (energy >= SPECIAL_ATTACK_ENERGY_COST)
		{
			int damage = calculateDamage() * 2;
			enemy.takeDamage(damage);
			energy -= SPECIAL_ATTACK_ENERGY_COST;
			cout << name << " performs a special attack on " << enemy.name << " for " << damage << " damage." << endl;
			cout << "Energy decreased by " << SPECIAL_ATTACK_ENERGY_COST << "." << endl;
		}
		else
		{
			cout << "Not enough energy for special attack. Perform a basic attack instead." << endl;
			basicAttack(enemy);
		}
	}

	void regenerateEnergy()
	{
		energy += ENERGY_REGEN_AMOUNT;
		if (energy < 0)
		{
			energy = 0;
		}
		cout << name << " regenerated " << ENERGY_REGEN_AMOUNT << " energy." << endl;
	}
};

class Enemy : public Character
{
public:
	Enemy(string n, int h, int a, int d, int e, int r, int c) : Character(n, h, a, d, e, r, c) {}

	void performRandomAttack(Character &player)
	{
		if (energy >= SPECIAL_ATTACK_ENERGY_COST)
		{
			int attackType = rand() % 2; // Véletlenszerűen választunk az alap és speciális támadás között
			if (attackType == 0)
			{
				basicAttack(player);
			}
			else
			{
				specialAttack(player);
			}
		}
		else
		{
			basicAttack(player);
		}
	}
};

void clearConsole()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void drawMaze(char maze[MAX_ROWS][MAX_COLS], Character player, Enemy enemies[], int numEnemies)
{
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; j++)
		{
			if (i == player.row && j == player.col)
			{
				cout << "P"; // Játékos karakter
			}
			else
			{
				bool isEnemyTile = false;
				for (int k = 0; k < numEnemies; k++)
				{
					if (i == enemies[k].row && j == enemies[k].col && enemies[k].isAlive())
					{
						isEnemyTile = true;
						break;
					}
				}
				if (isEnemyTile)
				{
					bool enemyDefeated = false;
					for (int k = 0; k < numEnemies; k++)
					{
						if (enemies[k].row == i && enemies[k].col == j && !(enemies[k].isAlive()))
						{
							enemyDefeated = true;
							break;
						}
					}
					if (enemyDefeated)
					{
						cout << " "; // Üres mező az eltávolított ellenségek helyére
					}
					else
					{
						cout << "E"; // Ellenség mező
					}
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
	for (int i = 0; i < numEnemies; i++)
	{
		int row, col;
		do
		{
			row = rand() % MAX_ROWS;
			col = rand() % MAX_COLS;
		} while (maze[row][col] == '#' || (row == 0 && col == 0)); // Ellenőrzés a falak és a játékos kezdőpozíciója ellen

		enemies[i].row = row;
		enemies[i].col = col;
	}
}

void fight(Character &player, Enemy &enemy, char maze[MAX_ROWS][MAX_COLS])
{
	clearConsole();
	cout << "You encountered an enemy! Prepare for battle!" << endl;
	while (player.isAlive() && enemy.isAlive())
	{
		cout << "---------------------------" << endl;
		player.printStats();
		enemy.printStats();

		int choice;
		cout << "Choose an action:" << endl;
		cout << "1. Basic Attack" << endl;
		cout << "2. Special Attack" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		clearConsole();

		switch (choice)
		{
		case 1: // Basic Attack
			player.basicAttack(enemy);
			break;
		case 2: // Special Attack
			player.specialAttack(enemy);
			break;
		default:
			cout << "Invalid choice. Try again." << endl;
			continue;
		}

		if (enemy.isAlive())
		{
			enemy.performRandomAttack(player);
		}

		if (player.energy <= 0)
		{
			cout << "Your energy has depleted. Game over." << endl;
			return;
		}
		if (enemy.energy <= 0)
		{
			cout << "You defeated the enemy!" << endl;
			maze[enemy.row][enemy.col] = ' '; // Eltávolítjuk az ellenséget a labirintusból
			enemy.row = -1;					  // Ellenőrzéshez használjuk, hogy az ellenség eltűnt
			enemy.col = -1;
			return;
		}

		cout << endl;
		cout << "Press Enter to continue..." << endl;
		cin.ignore();
		cin.get();
		clearConsole();
	}

	cout << "You were defeated!" << endl;

	cout << "Press Enter to continue..." << endl;
	cin.ignore();
	cin.get();
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	char maze[MAX_ROWS][MAX_COLS] = {
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', '#'},
		{'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', '#'},
		{'#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', '#'},
		{'#', ' ', '#', '#', '#', '#', '#', ' ', '#', '#'},
		{'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{'#', ' ', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}};

	Character player("Player", 100, 10, 5, 100, 1, 1); // Játékos kezdőpozíciója

	Enemy enemies[MAX_ENEMIES] = {
		Enemy("Enemy1", 50, 8, 2, 100, 3, 3),
		Enemy("Enemy2", 60, 7, 3, 100, 5, 7),
		Enemy("Enemy3", 70, 6, 4, 100, 7, 2)};

	cout << "Welcome to the Console RPG Game!" << endl;

	placeEnemies(enemies, MAX_ENEMIES, maze); // Ellenségek helyének inicializálása

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

		clearConsole();

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
				if (newRow == enemies[i].row && newCol == enemies[i].col && enemies[i].isAlive())
				{
					validMove = false;
					fight(player, enemies[i], maze);
					break;
				}
			}

			if (validMove)
			{
				if (maze[newRow][newCol] != ' ')
				{
					player.regenerateEnergy();
				}
				maze[player.row][player.col] = ' '; // Eltávolítjuk a játékost az előző mezőről
				player.row = newRow;
				player.col = newCol;
			}
		}

		if (player.energy <= 0)
		{
			cout << "Your energy has depleted. Game over." << endl;
			return 0;
		}
	}

	cout << "Game over. You were defeated." << endl;
	return 0;
}
