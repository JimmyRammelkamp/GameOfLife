// GameOfLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>  
#include <time.h>
#include <omp.h>

clock_t startTime, endTime;
float processTime;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

const int gridSizeX = 20;
const int gridSizeY = 20;

int steps = 100000;

int livingCellCount;
int deadCellCount;

bool presentCells[gridSizeX][gridSizeY];
bool futureCells[gridSizeX][gridSizeY];

bool randomBool() {
    return rand() > (RAND_MAX / 2);
}

void initialiseGrid()
{
    srand(time(NULL));
//#pragma omp parallel for //breaks randomisation
    for (int x = 0; x < gridSizeX; x++)
    {
        for (int y = 0; y < gridSizeY; y++)
        {
            presentCells[x][y] = randomBool();
        }
    }
}

void cellInfo()
{
    livingCellCount = 0;
    deadCellCount = 0;
    for (int x = 0; x < gridSizeX; x++)
    {
        for (int y = 0; y < gridSizeY; y++)
        {
            livingCellCount += presentCells[x][y];
        }
    }
    deadCellCount = (gridSizeX * gridSizeY) - livingCellCount;

    std::cout << "Living Cells: " << livingCellCount << " Dead Cells: " << deadCellCount << " Total Cells: " << livingCellCount + deadCellCount << "\n";
    
}

void overwritePresentCells()
{
#pragma omp parallel for //schedule(dynamic,10)//num_threads(20) // //schedule(guided,10) //num_threads(4)
    for (int x = 0; x < gridSizeX; x++)
    {
        for (int y = 0; y < gridSizeY; y++)
        {
            presentCells[x][y] = futureCells[x][y];
        }
    }
}

void displayCells()
{
    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {
            if (presentCells[x][y] == true)
            {
                //system("Color 0A");
                SetConsoleTextAttribute(h, 10);
                std::cout << "X ";
            }
            else
            {
                //system("Color 0C");
                SetConsoleTextAttribute(h, 12);
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    SetConsoleTextAttribute(h, 15);
    
}

void processCells()
{
#pragma omp parallel for //schedule(dynamic,10)//num_threads(20) //collapse(2) //schedule(guided,10) //num_threads(4)
    for (int x = 0; x < gridSizeX; x++)
    {
        for (int y = 0; y < gridSizeY; y++)
        {
            bool alive;
            int neighbourCount = 0;

            neighbourCount = presentCells[x - 1][y - 1] + presentCells[x][y - 1] + presentCells[x + 1][y - 1] + presentCells[x - 1][y] + presentCells[x + 1][y] + presentCells[x - 1][y + 1] + presentCells[x][y + 1] + presentCells[x + 1][y + 1];

            if (presentCells[x][y] == true)
            {
                /*if (presentCells[x - 1][y - 1] == true) neighbourCount++;
                if (presentCells[x][y - 1] == true) neighbourCount++;
                if (presentCells[x + 1][y - 1] == true) neighbourCount++;
                if (presentCells[x - 1][y] == true) neighbourCount++;
                if (presentCells[x + 1][y] == true) neighbourCount++;
                if (presentCells[x - 1][y + 1] == true) neighbourCount++;
                if (presentCells[x][y + 1] == true) neighbourCount++;
                if (presentCells[x + 1][y + 1] == true) neighbourCount++;*/


                if (neighbourCount <= 1 || neighbourCount >= 4) alive = false;
                else alive = true;
                futureCells[x][y] = alive;

            }
            else
            {
                /*if (presentCells[x - 1][y - 1] == true) neighbourCount++;
                if (presentCells[x][y - 1] == true) neighbourCount++;
                if (presentCells[x + 1][y - 1] == true) neighbourCount++;
                if (presentCells[x - 1][y] == true) neighbourCount++;
                if (presentCells[x + 1][y] == true) neighbourCount++;
                if (presentCells[x - 1][y + 1] == true) neighbourCount++;
                if (presentCells[x][y + 1] == true) neighbourCount++;
                if (presentCells[x + 1][y + 1] == true) neighbourCount++;*/

                if (neighbourCount == 3) futureCells[x][y] = true;
            }

        }
    }
}

void update()
{
    processCells();
    overwritePresentCells();
}

void updateInfo()
{
    system("cls"); //Significantly hurt performance accidently leaving it on when not displaying cells
    displayCells();
    cellInfo();
    system("pause");


    //Sleep(200);
}



int main()
{
    //Initialisation
    startTime = clock();
    initialiseGrid();
    endTime = clock();
    processTime = (((float)endTime - (float)startTime) / 1000);
    std::cout << "Grid Initialisation process time: " << processTime << " (in s)" << std::endl;
    //generateghostbordercells
    //displayCells();
    cellInfo();
    system("pause");
    
    
    //Processing
    startTime = clock();
    for (int i = 0; i < steps; i++) {
        update();
        updateInfo();
    }
    endTime = clock();
    processTime = (((float)endTime - (float)startTime) / 1000);
    std::cout << "Simulation process time: " << processTime << " (in s)" << std::endl;
    cellInfo();
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
