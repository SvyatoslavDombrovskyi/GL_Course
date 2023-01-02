#include "labyrinth.h"
#include <stdio.h>

int main(void)
{
    struct Labyrinth labyrinthA;

    Labyrint_CopyShablon(&labyrinthA, myLabyrinth);    
    labyrinthA.startX = 2;
    labyrinthA.startY = 0;
    labyrinthA.endX = 2;
    labyrinthA.endY = 4;

    prepareLabyrinthForPrintArrArr(labyrinthA.labyrintShablon, labyrinthA.labyrinthForPrint);
    Labyrinth_Print(labyrinthA.labyrintShablon);
    Labyrinth_PrintDeployed(labyrinthA.labyrinthForPrint);
    
    Labyrinth_Solve(&labyrinthA);
    
    addCorrectPathToLabyrinth(labyrinthA.correctPath, labyrinthA.labyrinthForPrint);
    Labyrinth_Print(labyrinthA.correctPath);
    Labyrinth_PrintDeployed(labyrinthA.labyrinthForPrint);

    getchar();
    return 0;
}