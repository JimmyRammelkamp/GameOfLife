// GameOfLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>  

const int gridSize = 30 ;

int steps = 1000;

bool presentCells[gridSize][gridSize];
bool futureCells[gridSize][gridSize];

bool randomBool() {
    return rand() > (RAND_MAX / 2);
}

void initialiseGrid()
{
    srand(time(NULL));
    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
            presentCells[x][y] = randomBool();
        }
    }
}

void overwritePresentCells()
{
    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
            presentCells[x][y] = futureCells[x][y];
        }
    }
}

void displayCells()
{
    for (int y = 0; y < gridSize; y++)
    {
        for (int x = 0; x < gridSize; x++)
        {
            if (presentCells[x][y] == true) std::cout << "X ";
            else std::cout << "O ";
        }
        std::cout << "\n";
    }

              std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";
    
}

void update()
{
    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
            bool alive;
            int neighbourCount = 0;
            if (presentCells[x][y] == true)
            {
                if (presentCells[x-1][y-1] == true) neighbourCount++;
                if (presentCells[x][y-1] == true) neighbourCount++;
                if (presentCells[x+1][y-1] == true) neighbourCount++;
                if (presentCells[x-1][y] == true) neighbourCount++;
                if (presentCells[x+1][y] == true) neighbourCount++;
                if (presentCells[x-1][y+1] == true) neighbourCount++;
                if (presentCells[x][y+1] == true) neighbourCount++;
                if (presentCells[x+1][y+1] == true) neighbourCount++;


                if (neighbourCount <= 1 || neighbourCount >= 4) alive = false;
                else alive = true;
                futureCells[x][y] = alive;

            }
            else
            {
                if (presentCells[x - 1][y - 1] == true) neighbourCount++;
                if (presentCells[x][y - 1] == true) neighbourCount++;
                if (presentCells[x + 1][y - 1] == true) neighbourCount++;
                if (presentCells[x - 1][y] == true) neighbourCount++;
                if (presentCells[x + 1][y] == true) neighbourCount++;
                if (presentCells[x - 1][y + 1] == true) neighbourCount++;
                if (presentCells[x][y + 1] == true) neighbourCount++;
                if (presentCells[x + 1][y + 1] == true) neighbourCount++;

                if (neighbourCount == 3) futureCells[x][y] = true;
            }

        }
    }
    overwritePresentCells();
    displayCells();
    //system("pause");
    Sleep(100);
    system("cls");
}



int main()
{
    initialiseGrid();
    //generateghostbordercells
    displayCells();
    for (int i = 0; i < steps; i++) {
        update();
    }
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
