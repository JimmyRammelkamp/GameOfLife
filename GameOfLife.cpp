// GameOfLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>  
#include <time.h>
#include <omp.h>
//#include <random>
//
//std::random_device rd;
//std::mt19937 gen(rd());
//std::uniform_int_distribution<> dis(0, 31);

clock_t startTime, endTime;
float processTime;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

const int gridSizeX = 40;
const int gridSizeY = 40;
const int threads = 16;

int steps = 100000;
int stepCount = 0;

int presentCells[gridSizeX][gridSizeY];
int futureCells[gridSizeX][gridSizeY];

int randomSpawner() 
{
    int randNum = (rand() % 4) - 1;
    if (randNum > 0) randNum = 1;
    return randNum;
}

void initialiseGrid()
{
    srand(time(NULL));
    for (int x = 0; x < gridSizeX; x++)
    {
        for (int y = 0; y < gridSizeY; y++)
        {
            presentCells[x][y] = randomSpawner();
        }
    }
}

void initialiseTest()
{
    for (int x = 0; x < gridSizeX; x++)
    {
        for (int y = 0; y < gridSizeY; y++)
        {
            presentCells[x][y] = false;
        }
    }

    //Spaced sharks for testing random death
    for (int x = 0; x < gridSizeX/3; x += 3)
    {
        for (int y = 0; y < gridSizeY/3; y += 3)
        {
            presentCells[x][y] = -1;
        }
    }

    //Glider for game of life
    //presentCells[2][0] = true;
    //presentCells[3][1] = true;
    //presentCells[1][2] = true; presentCells[2][2] = true; presentCells[3][2] = true;

}

void cellInfo()
{
    int fishCount = 0;
    int sharkCount = 0;
    int emptyCount = 0;
    for (int x = 0; x < gridSizeX; x++)
    {
        for (int y = 0; y < gridSizeY; y++)
        {
            if (presentCells[x][y] > 0) fishCount++;
            else if (presentCells[x][y] < 0) sharkCount++;
        }
    }
    emptyCount = (gridSizeX * gridSizeY) - fishCount - sharkCount;

    std::cout << "Fish: " << fishCount << " Sharks: " << sharkCount << " Empty: " << emptyCount << " Iterations: " << stepCount << "\n";
    
}



void displayCells()
{
    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {
            if (presentCells[x][y] > 0) //fish
            {
                SetConsoleTextAttribute(h, 11);
                if (presentCells[x][y] > 9) std::cout << presentCells[x][y] << " ";
                else std::cout << presentCells[x][y] << "  ";
            }
            else if (presentCells[x][y] < 0) //shark
            {
                SetConsoleTextAttribute(h, 12);
                if (presentCells[x][y] < -9) std::cout << presentCells[x][y]*-1 << " ";
                else std::cout << presentCells[x][y]*-1 << "  ";
            }
            else //empty
            {
                SetConsoleTextAttribute(h, 1);
                std::cout << ".  ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    SetConsoleTextAttribute(h, 15);
    
}


int getCell(int i, int j) 
{    
    return presentCells[(i + gridSizeX) % gridSizeX][(j + gridSizeY) % gridSizeY];
}

void processCells()
{
#pragma omp parallel for //schedule(static,1) //num_threads(threads) //schedule(dynamic,10)  //collapse(2) //schedule(guided,10) //num_threads(4)
    for (int x = 0; x < gridSizeX; x++)
    {
        for (int y = 0; y < gridSizeY; y++)
        {
            int neighbours[8];
            neighbours[0] = getCell(x - 1, y - 1);
            neighbours[1] = getCell(x, y - 1);
            neighbours[2] = getCell(x + 1, y - 1);
            neighbours[3] = getCell(x - 1, y);
            neighbours[4] = getCell(x + 1, y);
            neighbours[5] = getCell(x - 1, y + 1);
            neighbours[6] = getCell(x, y + 1);
            neighbours[7] = getCell(x + 1, y + 1);

            int neighbourFishCount = 0;
            int neighbourFishBreedableCount = 0;
            int neighbourSharkCount = 0;
            int neighbourSharkBreedableCount = 0;
            int neighbourEmptyCount = 0;

            for (int i = 0; i < 8; i++) 
            {
                if (neighbours[i] >= 2) { neighbourFishBreedableCount++; neighbourFishCount++; }
                else if (neighbours[i] > 0) neighbourFishCount++;
                else if (neighbours[i] <= -3) { neighbourSharkBreedableCount++; neighbourSharkCount++; }
                else if (neighbours[i] < 0) neighbourSharkCount++;
                else neighbourEmptyCount++;
            }

            if (presentCells[x][y] > 0) //Fish Rules
            {
                if (neighbourSharkCount >= 5) futureCells[x][y] = 0;  // Eaten Death
                else if (neighbourFishCount == 8) futureCells[x][y] = 0; // Overcrowding Death
                else if (presentCells[x][y] > 10) futureCells[x][y] = 0; // Lifespan Death
                else futureCells[x][y] = presentCells[x][y] + 1; // Ageing
            }
            else if(presentCells[x][y] < 0) //Shark Rules
            {
                if (neighbourSharkCount >= 6 && neighbourFishCount == 0) futureCells[x][y] = 0; // Overcrowding/Underfead Death
                else if (/*dis(gen)*/int(rand()%32) == 0) futureCells[x][y] = 0; // 1/32 chance of death
                else if (presentCells[x][y] < -20) futureCells[x][y] = 0; // Lifespan Death
                else futureCells[x][y] = presentCells[x][y] - 1; // Ageing
            }
            else //Breeding Rules
            {
                if (neighbourFishCount >= 4 && neighbourFishBreedableCount >= 3 && neighbourSharkCount < 4) futureCells[x][y] = 1; // Spawn Fish
                else if (neighbourSharkCount >= 4 && neighbourSharkBreedableCount >= 3 && neighbourFishCount < 4) futureCells[x][y] = -1; // Spawn Shark
            }

        }
    }
    stepCount++;
}

void overwritePresentCells()
{
#pragma omp parallel for //num_threads(threads)//schedule(dynamic,10)// // //schedule(guided,10) //num_threads(4)
    for (int x = 0; x < gridSizeX; x++)
    {
        for (int y = 0; y < gridSizeY; y++)
        {
            presentCells[x][y] = futureCells[x][y];
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
    system("cls");
    displayCells();
    cellInfo();
    system("pause");
}



int main()
{
    //Initialisation
    startTime = clock();
    initialiseGrid();
    //initialiseTest();
    endTime = clock();
    processTime = (((float)endTime - (float)startTime) / 1000);
    std::cout << "Grid Initialisation process time: " << processTime << " (in s)" << std::endl;
    displayCells();
    cellInfo();
    system("pause");
    
    
    //Processing
    startTime = clock();
    
    //1 step at a time
    for (int i = 0; i < steps; i++) {
        update();
        updateInfo();
    }

    //Iterate 100 steps at a time
    /*for (int i = 0; i < steps/100; i++) {
        for(int i = 0; i < 100; i++)
            update();
        updateInfo();
    }*/

    //Run through all steps in one go
    /*for (int i = 0; i < steps; i++) {
        update();
    }*/


    endTime = clock();
    processTime = (((float)endTime - (float)startTime) / 1000);
    displayCells();
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
