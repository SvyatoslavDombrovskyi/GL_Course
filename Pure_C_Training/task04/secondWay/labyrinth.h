#pragma once

#define SIDE_SIZE 5
#define SIDE_FOR_PRINT_SIZE (2 * SIDE_SIZE + 1)

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

struct Labyrinth
{
    int labyrintShablon[SIDE_SIZE][SIDE_SIZE];
    int startX, startY;
    int endX, endY;

    int wasHere[SIDE_SIZE][SIDE_SIZE];
    int correctPath[SIDE_SIZE][SIDE_SIZE];
    int labyrinthForPrint[SIDE_FOR_PRINT_SIZE][SIDE_FOR_PRINT_SIZE];
};

extern int myLabyrinth[SIDE_SIZE][SIDE_SIZE];

void Labyrinth_Print(int labyrinth[SIDE_SIZE][SIDE_SIZE]);
void Labyrinth_PrintDeployed(int labyrinth[SIDE_FOR_PRINT_SIZE][SIDE_FOR_PRINT_SIZE]);
void Labyrinth_Solve(struct Labyrinth *thisLabyrinth);
int  recursiveSolve(struct Labyrinth *thisLabyrinth, int MazeShab[SIDE_SIZE][SIDE_SIZE], int x, int y, int comeFrom);
void Labyrint_CopyShablon(struct Labyrinth *thisLabyrinth, int labyrintShablon[SIDE_SIZE][SIDE_SIZE]);

void prepareLabyrinthForPrintArrArr(int MazeShab[SIDE_SIZE][SIDE_SIZE],
                                    int MazeForPrint[SIDE_FOR_PRINT_SIZE][SIDE_FOR_PRINT_SIZE]);
void addCorrectPathToLabyrinth(int MazeCorrectPath[SIDE_SIZE][SIDE_SIZE],
                               int MazeForPrint[SIDE_FOR_PRINT_SIZE][SIDE_FOR_PRINT_SIZE]);
