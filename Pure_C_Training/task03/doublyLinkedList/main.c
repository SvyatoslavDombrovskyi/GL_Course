#include "book.h"
#include <stdio.h>

int main(void)
{
    struct Book secondBook = {"Chamber of Secrets", "English", 12.45, 251, 0.810, 1998, NULL, NULL};
    struct Book firstBook = {"Philosopher's Stone", "English", 10.25, 223, 0.760, 1997, &secondBook, NULL};
    Book_Link(&firstBook, &secondBook);

    struct Book thirdBook;

    Book_InitFromConsole(&thirdBook);
    Book_Link(&secondBook, &thirdBook);

    Book_PrintAll(&firstBook);   

    return 0;
}