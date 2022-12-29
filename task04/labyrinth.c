#include "labyrinth.h"
#include <stdio.h>
#include <string.h>

// 1 - border, 0 - passage
int myLabyrinth[SIDE_SIZE][SIDE_SIZE] = {
    {1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1}, 
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1}, 
    {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1}, 
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1}, 
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1}, 
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1}, 
    {1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1}
};

// enum Tileset
// {
//     passage = 0,
//     border = 1,
//     start_end = 2,
//     path = 3
// };

void Labyrinth_Print(int labyrinth[SIDE_SIZE][SIDE_SIZE])
{
    printf("\n");
    for (int i = 0; i < SIDE_SIZE; i++)
    {
        for (int j = 0; j < SIDE_SIZE; j++)
        {
            if (labyrinth[i][j] == 1)
            {
                printf("# ");
            }
            else if (labyrinth[i][j] == 0)
            {
                printf("  ");
            }
            else if (labyrinth[i][j] == 2)
            {
                printf(ANSI_COLOR_GREEN);
                printf("^ ");
                printf(ANSI_COLOR_RESET);
            }
            else if (labyrinth[i][j] == 3)
            {
                printf(ANSI_COLOR_RED);
                printf(". ");
                printf(ANSI_COLOR_RESET);
            }
        }
        printf("\n");
    }
}

void Labyrint_CopyShablon(struct Labyrinth *thisLabyrinth, int labyrintShablon[SIDE_SIZE][SIDE_SIZE])
{
    memcpy(thisLabyrinth->labyrintShablon, labyrintShablon, sizeof(thisLabyrinth->labyrintShablon));
}

void Labyrinth_Solve(struct Labyrinth *thisLabyrinth)
{
    for (int row = 0; row < SIDE_SIZE; row++)
    {
        for (int col = 0; col < SIDE_SIZE; col++)
        {
            thisLabyrinth->wasHere[row][col] = 0;
        }
    }

    memcpy(thisLabyrinth->correctPath, thisLabyrinth->labyrintShablon, sizeof(thisLabyrinth->labyrintShablon));

    recursiveSolve(thisLabyrinth, thisLabyrinth->startX, thisLabyrinth->startY);

    thisLabyrinth->correctPath[thisLabyrinth->startY][thisLabyrinth->startX] = 2;
    thisLabyrinth->correctPath[thisLabyrinth->endY][thisLabyrinth->endX] = 2;
}

int recursiveSolve(struct Labyrinth *thisLabyrinth, int x, int y)
{
    if (x == thisLabyrinth->endX && y == thisLabyrinth->endY)
        return 1;
    if (myLabyrinth[y][x] == 1 || thisLabyrinth->wasHere[y][x])
        return 0;
    
    thisLabyrinth->wasHere[y][x] = 1;
    if (x != 0) // left
        if (recursiveSolve(thisLabyrinth, x - 1, y))
        {
            thisLabyrinth->correctPath[y][x] = 3;
            return 1;
        }
    if (x != SIDE_SIZE - 1) //right
        if (recursiveSolve(thisLabyrinth, x + 1, y))
        {
            thisLabyrinth->correctPath[y][x] = 3;
            return 1;
        }
    if (y != 0) // top
        if (recursiveSolve(thisLabyrinth, x, y - 1))
        {
            thisLabyrinth->correctPath[y][x] = 3;
            return 1;
        }
    if (y != SIDE_SIZE - 1) // bottom
        if (recursiveSolve(thisLabyrinth, x, y + 1))
        {
            thisLabyrinth->correctPath[y][x] = 3;
            return 1;
        }
    return 0;
}