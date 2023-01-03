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
}

void Book_Link(struct Book *this, struct Book *other)
{
    struct Book *tmp = this->next;
    this->next = other;
    other->next = tmp;
}

void Book_PrintAll(struct Book *book)
{
    struct Book *thisBook = book;

    do
    {
        Book_Print(thisBook);
        thisBook = thisBook->next;
    } while (thisBook != book);
}
