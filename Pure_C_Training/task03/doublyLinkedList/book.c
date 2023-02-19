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
    gets(book->title);
    printf("Language: ");
    gets(book->language);
    printf("Price: ");
    scanf("%f", &book->price);
    printf("Number of page: ");
    scanf("%d", &book->numberOfPages);
    printf("Weight: ");
    scanf("%f", &book->weight);
    printf("Year: ");
    scanf("%d", &book->year);
    book->next = NULL;
    book->previous = NULL;
}

void Book_Link(struct Book *this, struct Book *other)
{
    this->next = other;
    other->previous = this;
}

void Book_PrintAll(struct Book *book)
{
    for (struct Book *thisBook = book; thisBook != NULL; thisBook = thisBook->next)
    {
        Book_Print(thisBook);
    };
}

void Book_PrintAllReverse(struct Book *book)
{
    for (struct Book *thisBook = book; thisBook != NULL; thisBook = thisBook->previous)
    {
        Book_Print(thisBook);
    };
}
