#include <stdio.h>
#include <stdlib.h>

#define MAX_BOOKS 100

typedef struct {
    char title[100];
    char author[100];
    int year;
    char condition[20];
} Book;

Book books[MAX_BOOKS];
int bookCount = 0;

void displayMenu() {
    printf("\nBook Buddies - Donate Used Books\n");
    printf("1. Donate a book\n");
    printf("2. List all donated books\n");
    printf("3. Exit\n");
}

void donateBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Sorry, no more books can be donated at the moment.\n");
        return;
    }
    
    Book newBook;
    printf("\nEnter book details:\n");
    printf("Title: ");
    scanf(" %[^\n]", newBook.title);
    printf("Author: ");
    scanf(" %[^\n]", newBook.author);
    printf("Year: ");
    scanf("%d", &newBook.year);
    printf("Condition: ");
    scanf(" %[^\n]", newBook.condition);
    
    books[bookCount++] = newBook;
    printf("Book donated successfully!\n");
}

void listBooks() {
    if (bookCount == 0) {
        printf("No books have been donated yet.\n");
        return;
    }
    
    printf("\nList of donated books:\n");
    for (int i = 0; i < bookCount; i++) {
        Book book = books[i];
        printf("%d. %s by %s, %d, Condition: %s\n", i+1, book.title, book.author, book.year, book.condition);
    }
}

int main() {
    int choice;
    
    printf("Welcome to Book Buddies - A program to donate used books to other students.\n");
    
    while (1) {
        displayMenu();
        
        printf("\nEnter your choice (1-3): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                donateBook();
                break;
            case 2:
                listBooks();
                break;
            case 3:
                printf("Thank you for using Book Buddies. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}
