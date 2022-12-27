#include "book.h"

#include <stdio.h>

void Book_Print(struct Book *book)
{
    printf("\nTitle: %s \n", book->title);
    printf("Language: %s\n", book->language);
    printf("Price: %f \n", book->price);
    printf("Number of page: %d \n", book->numberOfPages);
    printf("Weight: %f \n", book->weight);
    printf("Year: %d \n", book->year);
}

void Book_InitFromConsole(struct Book *book)
{
    printf("\nTitle: ");
    scanf("%s", &book->title);
    printf("Language: ");
    scanf("%s", &book->language);
    printf("Price: ");
    scanf("%f", &book->price);
    printf("Number of page: ");
    scanf("%d", &book->numberOfPages);
    printf("Weight: ");
    scanf("%f", &book->weight);
    printf("Year: ");
    scanf("%d", &book->year);
    book->next = NULL;
}

void Book_Link(struct Book *book, struct Book *other)
{
    book->next = other;
}

void Book_PrintAll(struct Book *book)
{
    for (struct Book *thisBook = book; thisBook != NULL; thisBook = thisBook->next)
    {
        Book_Print(thisBook);
    };
}
