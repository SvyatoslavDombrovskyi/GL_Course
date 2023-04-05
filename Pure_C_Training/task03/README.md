# Pure C Training 03: Linked list

Based on the given task, a linked list was created. In total, 3 implementations of it were created with some differences in the connection between the elements.   
Namely, [**single**](#singly-linked-list), [**double**](#doubly-linked-list) and [**circular**](#circular-linked-list) lists.

## Code review

### **Struct**

To describe and interact with the list, a struct has been created that is almost the same for all 3 methods.

```C
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
```
This structure allows you to specify basic information about the book, as well as its connection with the next book.

### **Function**

In order to implement list operations, 4 main functions have been created: 

```C  
void Book_Print(struct Book* this);
void Book_InitFromConsole(struct Book* this);
void Book_Link(struct Book* this, struct Book* other);
void Book_PrintAll(struct Book* this);
```
 
All 3 methods will have different function definitions, except for one. Namely, `Book_Print`, which displays information about a specific book.

#### ***Book_Print***

Definition of `Book_Print`:

```C
void Book_Print(struct Book *book)
{
    printf("\nTitle: %s \n", book->title);
    printf("Language: %s\n", book->language);
    printf("Price: %f \n", book->price);
    printf("Number of page: %d \n", book->numberOfPages);
    printf("Weight: %f \n", book->weight);
    printf("Year: %d \n", book->year);
}
```


## Singly Linked List

*A singly linked list* is a linear data structure in which the elements are not stored in contiguous memory locations and each element is connected only to its next element using a pointer.

### **Function**

#### ***Book_InitFromConsole***

This function receives information from the console and enters it into the struct.  

```C
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
```

#### ***Book_Link***

Creates a connection between books, forming a singly linked list.

```C
void Book_Link(struct Book *book, struct Book *other)
{
    book->next = other;
}
```

#### ***Book_PrintAll***

Displays all books starting from a specific one and ending at the end of the list.

```C
void Book_PrintAll(struct Book *book)
{
    for (struct Book *thisBook = book; thisBook != NULL; thisBook = thisBook->next)
    {
        Book_Print(thisBook);
    };
}
```


## Doubly Linked List

 *A doubly linked list* contains an extra pointer, typically called previous pointer, together with next pointer and data which are there in singly linked list. 

### **Struct**

From the description of the doubly linked list, it follows that the structure for it will be slightly different, namely, field `struct Book *previous` has been added.

```C
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
    struct Book *previous;    
};
```
Thanks to this field, you can access the previous book, which expands the functionality of this list.

 ### **Function**

#### ***Book_InitFromConsole***

This function receives information from the console and enters it into the struct. But the pointer to the previous book is also reset here. 

```C
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
```

#### ***Book_Link***

Creates a link between the previous and next books, forming a doubly linked list.

```C
void Book_Link(struct Book *this, struct Book *other)
{
    this->next = other;
    other->previous = this;
}
```

#### ***Book_PrintAll***

Displays all books starting from a specific one and ending at the end of the list. Similarly as in a singly linked list.

```C
void Book_PrintAll(struct Book *book)
{
    for (struct Book *thisBook = book; thisBook != NULL; thisBook = thisBook->next)
    {
        Book_Print(thisBook);
    };
}
```

#### ***Book_PrintAllReverse***

Displays all books starting with a specific one and up to the beginning of the list. This possibility appears due to the fact that the list is doubly linked.

```C
void Book_PrintAllReverse(struct Book *book)
{
    for (struct Book *thisBook = book; thisBook != NULL; thisBook = thisBook->previous)
    {
        Book_Print(thisBook);
    };
}
```

## Circular Linked List

*Circular linked list* is a variation of linked list in which the last element points to the first element.

 ### **Function**

#### ***Book_InitFromConsole***

This function receives information from the console and enters it into the struct. Just like in a singly linked list.

```C
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
```

#### ***Book_Link***

Creates a link between books so that the last book in the list always points to the first.

```C
void Book_Link(struct Book *this, struct Book *other)
{
    struct Book *tmp = this->next;
    this->next = other;
    other->next = tmp;
}
```

#### ***Book_PrintAll***

Displays all available books in the list, starting with a specific one and ending when the entire list has not been completed.

```C
void Book_PrintAll(struct Book *book)
{
    struct Book *thisBook = book;

    do
    {
        Book_Print(thisBook);
        thisBook = thisBook->next;
    } while (thisBook != book);
}
```

## Example

The same data was entered for the three methods and the results were the same. That is, thanks to them, you can correctly implement the needs of the task.

<img src="../../photoForReadme/Pure_C_Training/task03/Harry%20Potter%20books.png"  width="230"/>