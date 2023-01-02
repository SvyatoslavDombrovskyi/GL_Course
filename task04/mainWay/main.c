#include "labyrinth.h"
#include <stdio.h>

int main(void)
{
    struct Labyrinth labyrinthA;

    Labyrint_CopyShablon(&labyrinthA, myLabyrinth);
    labyrinthA.startX = 5;
    labyrinthA.startY = 0;
    labyrinthA.endX = 5;
    labyrinthA.endY = 10;

    Labyrinth_Print(labyrinthA.labyrintShablon);
    Labyrinth_Solve(&labyrinthA);

    Labyrinth_Print(labyrinthA.correctPath);
    getchar();
    return 0;
}