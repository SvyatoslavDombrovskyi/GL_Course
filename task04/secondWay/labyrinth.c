#include "labyrinth.h"
#include <stdio.h>
#include <string.h>

int myLabyrinth[SIDE_SIZE][SIDE_SIZE] = {{0xD, 0x5, 0x2, 0xD, 0x3},
                                         {0x9, 0x3, 0xC, 0x5, 0x2},
                                         {0xE, 0x8, 0x5, 0x1, 0x2},
                                         {0x9, 0x6, 0xD, 0x6, 0xA},
                                         {0xC, 0x5, 0x3, 0xD, 0x6}};

enum Tileset
{
    passage = 0,
    border = 1,
    start_end = 2,
    path = 3
};

enum WayCameFrom
{
    top = 0b0001,    // 1
    bottom = 0b0010, // 2
    left = 0b0100,   // 4
    right = 0b1000   // 8
};

enum WayDirection
{
    top_bottom = 0b0011,
    left_right = 0b1100, 
    bottom_right = 0b1010,
    left_bottom = 0b0110,
    top_left = 0b0101,
    right_top = 0b1001
};

void prepareLabyrinthForPrintArrArr(int MazeShab[SIDE_SIZE][SIDE_SIZE],
                                    int MazeForPrint[SIDE_FOR_PRINT_SIZE][SIDE_FOR_PRINT_SIZE])
{
    for (int i = 0; i < SIDE_FOR_PRINT_SIZE; i++)
    {
        for (int j = 0; j < SIDE_FOR_PRINT_SIZE; j++)
        {
            MazeForPrint[i][j] = 0;
        }
    }

    for (int i = 0; i < SIDE_SIZE; i++)
    {
        for (int j = 0; j < SIDE_SIZE; j++)
        {
            if (((MazeShab[i][j] >> 3) & 1) == 1) // left - 8 - 1000
            {
                MazeForPrint[2 * i][2 * j] = 1;
                MazeForPrint[2 * i + 1][2 * j] = 1;
                MazeForPrint[2 * i + 2][2 * j] = 1;
            }
            if (((MazeShab[i][j] >> 1) & 1) == 1) // right - 2 - 0010
            {
                MazeForPrint[2 * i][2 * j + 2] = 1;
                MazeForPrint[2 * i + 1][2 * j + 2] = 1;
                MazeForPrint[2 * i + 2][2 * j + 2] = 1;
            }
            if (((MazeShab[i][j]) & 1) == 1) // top - 1 - 0001
            {
                MazeForPrint[2 * i][2 * j] = 1;
                MazeForPrint[2 * i][2 * j + 1] = 1;
                MazeForPrint[2 * i][2 * j + 2] = 1;
            }
            if (((MazeShab[i][j] >> 2) & 1) == 1) // bottom - 4 - 0100
            {
                MazeForPrint[2 * i + 2][2 * j] = 1;
                MazeForPrint[2 * i + 2][2 * j + 1] = 1;
                MazeForPrint[2 * i + 2][2 * j + 2] = 1;
            }
        }
    }
}

void prepareLabyrinthForPrint(struct Labyrinth *thisLabyrinth)
{
    int *MazeShab = &thisLabyrinth->labyrintShablon[0][0];
    int *MazeForPrint = &thisLabyrinth->labyrinthForPrint[0][0];
    int cols = SIDE_FOR_PRINT_SIZE;

    for (int i = 0; i < SIDE_FOR_PRINT_SIZE; i++)
    {
        for (int j = 0; j < SIDE_FOR_PRINT_SIZE; j++)
        {
            *(MazeForPrint + i * cols + j) = 0;
        }
    }

    for (int i = 0; i < SIDE_SIZE; i++)
    {
        for (int j = 0; j < SIDE_SIZE; j++)
        {
            int k = 2 * i;
            int l = 2 * j;
            if (((*(MazeShab + i * SIDE_SIZE + j) >> 3) & 1) == 1) // left - 8 - 1000
            {
                *(MazeForPrint + (k * cols) + l) = 1;
                *(MazeForPrint + ((k + 1) * cols) + l) = 1;
                *(MazeForPrint + ((k + 2) * cols) + l) = 1;
            }
            if (((*(MazeShab + i * SIDE_SIZE + j) >> 1) & 1) == 1) // right - 2 - 0010
            {
                *(MazeForPrint + (k * cols) + l + 2) = 1;
                *(MazeForPrint + ((k + 1) * cols) + l + 2) = 1;
                *(MazeForPrint + ((k + 2) * cols) + l + 2) = 1;
            }
            if (((*(MazeShab + i * SIDE_SIZE + j)) & 1) == 1) // top - 1 - 0001
            {
                *(MazeForPrint + (k * cols) + l) = 1;
                *(MazeForPrint + (k * cols) + l + 1) = 1;
                *(MazeForPrint + (k * cols) + l + 2) = 1;
            }
            if (((*(MazeShab + i * SIDE_SIZE + j) >> 2) & 1) == 1) // bottom - 4 - 0100
            {
                *(MazeForPrint + ((k + 2) * cols) + l) = 1;
                *(MazeForPrint + ((k + 2) * cols) + l + 1) = 1;
                *(MazeForPrint + ((k + 2) * cols) + l + 2) = 1;
            }
        }
    }
}

void addCorrectPathToLabyrinth(int MazeCorrectPath[SIDE_SIZE][SIDE_SIZE],
                               int MazeForPrint[SIDE_FOR_PRINT_SIZE][SIDE_FOR_PRINT_SIZE])
{
    for (int i = 0; i < SIDE_SIZE; i++)
    {
        for (int j = 0; j < SIDE_SIZE; j++)
        {
            if (MazeCorrectPath[i][j] == top_bottom)
            {
                MazeForPrint[2 * i][2 * j + 1] = path;
                MazeForPrint[2 * i + 1][2 * j + 1] = path;
                MazeForPrint[2 * i + 2][2 * j + 1] = path;
            }
            if (MazeCorrectPath[i][j] == left_right)
            {
                MazeForPrint[2 * i + 1][2 * j] = path;
                MazeForPrint[2 * i + 1][2 * j + 1] = path;
                MazeForPrint[2 * i + 1][2 * j + 2] = path;
            }
            if (MazeCorrectPath[i][j] == bottom_right)
            {
                MazeForPrint[2 * i + 1][2 * j + 1] = path;
                MazeForPrint[2 * i + 1][2 * j + 2] = path;
                MazeForPrint[2 * i + 2][2 * j + 1] = path;
            }
            if (MazeCorrectPath[i][j] == left_bottom)
            {
                MazeForPrint[2 * i + 1][2 * j] = path;
                MazeForPrint[2 * i + 1][2 * j + 1] = path;
                MazeForPrint[2 * i + 2][2 * j + 1] = path;
            }
            if (MazeCorrectPath[i][j] == top_left)
            {
                MazeForPrint[2 * i][2 * j + 1] = path;
                MazeForPrint[2 * i + 1][2 * j] = path;
                MazeForPrint[2 * i + 1][2 * j + 1] = path;
            }
            if (MazeCorrectPath[i][j] == right_top)
            {
                MazeForPrint[2 * i][2 * j + 1] = path;
                MazeForPrint[2 * i + 1][2 * j + 1] = path;
                MazeForPrint[2 * i + 1][2 * j + 2] = path;
            }
        }
    }
}

void Labyrinth_PrintDeployed(int labyrinth[SIDE_FOR_PRINT_SIZE][SIDE_FOR_PRINT_SIZE])
{
    printf("\n");
    for (int i = 0; i < SIDE_FOR_PRINT_SIZE; i++)
    {
        for (int j = 0; j < SIDE_FOR_PRINT_SIZE; j++)
        {
            if (labyrinth[i][j] == border)
            {
                printf("# ");
            }
            else if (labyrinth[i][j] == passage)
            {
                printf("  ");
            }
            else if (labyrinth[i][j] == start_end)
            {
                printf(ANSI_COLOR_GREEN);
                printf("^ ");
                printf(ANSI_COLOR_RESET);
            }
            if (labyrinth[i][j] == path)
            {
                printf(ANSI_COLOR_RED);
                printf(". ");
                printf(ANSI_COLOR_RESET);
            }
        }
        printf("\n");
    }
}

void Labyrinth_Print(int labyrinth[SIDE_SIZE][SIDE_SIZE])
{
    printf("\n");
    for (int i = 0; i < SIDE_SIZE; i++)
    {
        for (int j = 0; j < SIDE_SIZE; j++)
        {
            printf("%x ", labyrinth[i][j]);
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
            thisLabyrinth->correctPath[row][col] = 0;
        }
    }

    recursiveSolve(thisLabyrinth, thisLabyrinth->labyrintShablon, thisLabyrinth->startX, thisLabyrinth->startY, top);
}

int placementCorrectPathElement(int comeFrom, int comeTo)
{
    return comeFrom | comeTo;
}

int recursiveSolve(struct Labyrinth *thisLabyrinth, int MazeShab[SIDE_SIZE][SIDE_SIZE], int x, int y, int comeFrom)
{
    if (x == thisLabyrinth->endX && y == thisLabyrinth->endY)
    {
        thisLabyrinth->correctPath[y][x] = placementCorrectPathElement(comeFrom, bottom);
        return 1;
    }

    if (thisLabyrinth->wasHere[y][x])
    {
        return 0;
    }

    thisLabyrinth->wasHere[y][x] = 1;
    if (((MazeShab[y][x] >> 3) & 1) == 0)                             // left - 8 - 1000
        if (recursiveSolve(thisLabyrinth, MazeShab, x - 1, y, right)) // move left --> came from right
        {
            thisLabyrinth->correctPath[y][x] = placementCorrectPathElement(comeFrom, left);
            return 1;
        }
    if (((MazeShab[y][x] >> 1) & 1) == 0) // right - 2 - 0010
        if (recursiveSolve(thisLabyrinth, MazeShab, x + 1, y, left))
        {
            thisLabyrinth->correctPath[y][x] = placementCorrectPathElement(comeFrom, right);
            return 1;
        }
    if (((MazeShab[y][x]) & 1) == 0)                                   // top - 1 - 0001
        if (recursiveSolve(thisLabyrinth, MazeShab, x, y - 1, bottom)) // move top --> came from bottom
        {
            thisLabyrinth->correctPath[y][x] = placementCorrectPathElement(comeFrom, top);
            return 1;
        }
    if (((MazeShab[y][x] >> 2) & 1) == 0) // bottom - 4 - 0100
        if (recursiveSolve(thisLabyrinth, MazeShab, x, y + 1, top))
        {
            thisLabyrinth->correctPath[y][x] = placementCorrectPathElement(comeFrom, bottom);
            return 1;
        }
    return 0;
}