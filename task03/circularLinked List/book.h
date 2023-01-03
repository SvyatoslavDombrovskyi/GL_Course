#pragma once

#define MAX_LENGHT 128

struct Book
{
    char title[MAX_LENGHT];
    char language[MAX_LENGHT];
    float price;
    int numberOfPages;
    float weight;
    int year;
    struct Book *next;    
};


void Book_Print(struct Book* this);
void Book_InitFromConsole(struct Book* this);
void Book_Link(struct Book* this, struct Book* other);
void Book_PrintAll(struct Book* this);

//void Book_Init(struct Book* this, const char* name, const char* author, int pages);