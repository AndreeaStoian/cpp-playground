#include <vector>
#include <iostream>
#include <Windows.h>

class GameOfLife
{
private:
	std::vector<std::vector<int>> currentGeneration;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int numberOfNeighbours(size_t x, size_t y)
	{
		int n = 0;
		if (x == 0)
		{
			if (y == 0)
			{
				n += currentGeneration[x][y + 1] + currentGeneration[x + 1][y] + currentGeneration[x + 1][y + 1];
			}
			else if (y == currentGeneration[x].size() - 1)
			{
				n += currentGeneration[x][y - 1] + currentGeneration[x + 1][y] + currentGeneration[x + 1][y - 1];
			}
			else
			{
				n += currentGeneration[x][y - 1] + currentGeneration[x + 1][y - 1] + currentGeneration[x + 1][y] + currentGeneration[x + 1][y + 1] + currentGeneration[x][y + 1];
			}
		}
		else if (x == currentGeneration.size() - 1)
		{
			if (y == 0)
			{
				n += currentGeneration[x][y + 1] + currentGeneration[x - 1][y] + currentGeneration[x - 1][y + 1];
			}
			else if (y == currentGeneration[x].size() - 1)
			{
				n += currentGeneration[x][y - 1] + currentGeneration[x - 1][y] + currentGeneration[x - 1][y - 1];
			}
			else
			{
				n += currentGeneration[x][y - 1] + currentGeneration[x - 1][y - 1] + currentGeneration[x - 1][y] + currentGeneration[x - 1][y + 1] + currentGeneration[x][y + 1];
			}
		}
		else if (y == 0)
		{
			n += currentGeneration[x - 1][y] + currentGeneration[x - 1][y + 1] + currentGeneration[x][y + 1] + currentGeneration[x + 1][y + 1] + currentGeneration[x + 1][y];
		}
		else if (y == currentGeneration[x].size() - 1)
		{
			n += currentGeneration[x - 1][y] + currentGeneration[x - 1][y - 1] + currentGeneration[x][y - 1] + currentGeneration[x + 1][y - 1] + currentGeneration[x + 1][y];
		}
		else
		{
			n += currentGeneration[x - 1][y - 1] + currentGeneration[x - 1][y] + currentGeneration[x - 1][y + 1] + currentGeneration[x][y - 1] + currentGeneration[x][y + 1] + currentGeneration[x + 1][y - 1] + currentGeneration[x + 1][y] + currentGeneration[x + 1][y + 1];
		}
		return n;
	}
	void printCurrentGeneration()
	{
		for (auto i : currentGeneration)
		{
			for (auto j : i)
			{
				if (j == 0)
				{
					SetConsoleTextAttribute(hConsole, 15 + 12 * 16);
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 15 + 2 * 16);
				}
				std::cout << j <<" ";
			}
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << std::endl;
		}
		std::cout << std::endl;
		

	}
public:
	GameOfLife()
	{
		currentGeneration = { {0,0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,1,0,0},
							{0,0,0,0,0,1,0,1,1,0},
							{0,0,0,0,0,1,0,1,0,0},
							{0,0,0,0,0,1,0,0,0,0},
							{0,0,0,1,0,0,0,0,0,0},
							{0,1,0,1,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0} };
	}

	GameOfLife( std::vector<std::vector<int>> matrix) :
		currentGeneration(matrix)
	{
	}

	void Run()
	{
		std::vector<std::vector<int>> newGeneration;
		newGeneration = currentGeneration;
		int gen = 0;
		do
		{
			gen++;
			currentGeneration = newGeneration;
			for (size_t index1 = 0; index1 < currentGeneration.size(); index1++)
			{
				for (size_t index2 = 0; index2 < currentGeneration[index1].size(); index2++)
				{
					if (currentGeneration[index1][index2] == 0)
					{
						if (numberOfNeighbours(index1, index2) == 3)
						{
							newGeneration[index1][index2] = 1;
						}
					}
					else
					{
						if (numberOfNeighbours(index1, index2) < 2 || numberOfNeighbours(index1, index2) > 3)
						{
							newGeneration[index1][index2] = 0;
						}
					}
				}
			}

			system("cls");
			std::cout << "Generation " << gen << std::endl;
			printCurrentGeneration();
			Sleep(350);
			
		} while (currentGeneration != newGeneration);
	}
};


int main()
{
	/*std::vector<std::vector<int>> matrix = { {0,0,0,0,0,0,0,0,0,0},
											 {0,0,0,0,0,0,0,1,0,0},
											 {0,0,0,0,0,1,0,1,1,0},
											 {0,0,0,0,0,1,0,1,0,0},
											 {0,0,0,0,0,1,0,0,0,0},
											 {0,0,0,1,0,0,0,0,0,0},
											 {0,1,0,1,0,0,0,0,0,0},
											 {0,0,0,0,0,0,0,0,0,0} };
	GameOfLife game(matrix);*/
	GameOfLife game;
	game.Run();
	return 0;
}
