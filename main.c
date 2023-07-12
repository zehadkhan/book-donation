#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_BOOKS 100

typedef struct {
    char title[100];
    char author[100];
    int year;
    char condition[20];
    bool donated;
    char donor[100];
} Book;

typedef struct {
    char username[100];
    char password[100];
    char contactNumber[20];
    char email[100];
    bool loggedIn;
} User;

User users[MAX_USERS];
int userCount = 0;

Book books[MAX_BOOKS];
int bookCount = 0;

void displayMenu(User* user) {
    printf("\nBook Buddies - Donate Used Books\n");
    if (user && user->loggedIn) {
        printf("1. Donate a book\n");
        printf("2. List all donated books\n");
        printf("3. Request a book\n");
        printf("4. Logout (Currently logged in as: %s)\n", user->username);
        printf("5. Exit\n");
    } else {
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Donate a book\n");
        printf("4. List all donated books\n");
        printf("5. Request a book\n");
        printf("6. Exit\n");
    }
}


void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("Sorry, no more registrations can be accepted at the moment.\n");
        return;
    }

    User newUser;
    printf("\nEnter registration details:\n");
    printf("Username: ");
    scanf(" %[^\n]", newUser.username);
    printf("Password: ");
    scanf(" %[^\n]", newUser.password);
    printf("Contact Number: ");
    scanf(" %[^\n]", newUser.contactNumber);
    printf("Email: ");
    scanf(" %[^\n]", newUser.email);

    users[userCount++] = newUser;
    printf("Registration successful!\n");

    saveUsersToFile();
}

User* login() {
    char username[100];
    char password[100];
    printf("\nEnter login details:\n");
    printf("Username: ");
    scanf(" %[^\n]", username);
    printf("Password: ");
    scanf(" %[^\n]", password);

    for (int i = 0; i < userCount; i++) {
        User* user = &users[i];
        if (strcmp(user->username, username) == 0 && strcmp(user->password, password) == 0) {
            user->loggedIn = true;
            return user;
        }
    }

    return NULL;
}

void donateBook(User* user) {
    if (!user || !user->loggedIn) {
        printf("You must be logged in to donate a book.\n");
        return;
    }

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

    strcpy(newBook.donor, user->username);
    newBook.donated = false;

    books[bookCount++] = newBook;
    printf("Book donated successfully!\n");

    saveBooksToFile();
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

void requestBook(User* user) {
    if (!user || !user->loggedIn) {
        printf("You must be logged in to request a book.\n");
        return;
    }

    int bookIndex;
    printf("\nEnter the book number you want to request: ");
    scanf("%d", &bookIndex);
    bookIndex -= 1; // Adjusting to zero-based indexing

    if (bookIndex < 0 || bookIndex >= bookCount) {
        printf("Invalid book number. Please try again.\n");
        return;
    }

    Book* requestedBook = &books[bookIndex];

    if (requestedBook->donated) {
        printf("Sorry, the book has already been donated.\n");
        return;
    }

    printf("\nBook Details:\n");
    printf("Title: %s\n", requestedBook->title);
    printf("Author: %s\n", requestedBook->author);
    printf("Year: %d\n", requestedBook->year);
    printf("Condition: %s\n", requestedBook->condition);
    printf("Donor: %s\n", requestedBook->donor);

    printf("Do you want to request this book? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        requestedBook->donated = true;
        printf("Book requested successfully! Contact the donor for further details.\n");
    } else {
        printf("Book request canceled.\n");
    }
}

void saveUsersToFile() {
    FILE* file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("Failed to open users.txt for writing.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        User* user = &users[i];
        fprintf(file, "%s;%s;%s;%s;%d\n", user->username, user->password, user->contactNumber, user->email, user->loggedIn);
    }

    fclose(file);
    printf("Users data saved to users.txt.\n");
}

void saveBooksToFile() {
    FILE* file = fopen("books.txt", "w");
    if (file == NULL) {
        printf("Failed to open books.txt for writing.\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        Book book = books[i];
        fprintf(file, "%s;%s;%d;%s;%d;%s\n", book.title, book.author, book.year, book.condition, book.donated, book.donor);
    }

    fclose(file);
    printf("Books data saved to books.txt.\n");
}

void loadUsersFromFile() {
    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Failed to open users.txt for reading.\n");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* username = strtok(line, ";");
        char* password = strtok(NULL, ";");
        char* contactNumber = strtok(NULL, ";");
        char* email = strtok(NULL, ";");
        int loggedIn = atoi(strtok(NULL, ";\n"));

        User newUser;
        strcpy(newUser.username, username);
        strcpy(newUser.password, password);
        strcpy(newUser.contactNumber, contactNumber);
        strcpy(newUser.email, email);
        newUser.loggedIn = loggedIn;

        users[userCount++] = newUser;
    }

    fclose(file);
}

void loadBooksFromFile() {
    FILE* file = fopen("books.txt", "r");
    if (file == NULL) {
        printf("Failed to open books.txt for reading.\n");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* title = strtok(line, ";");
        char* author = strtok(NULL, ";");
        int year = atoi(strtok(NULL, ";"));
        char* condition = strtok(NULL, ";");
        int donated = atoi(strtok(NULL, ";"));
        char* donor = strtok(NULL, ";\n");

        Book newBook;
        strcpy(newBook.title, title);
        strcpy(newBook.author, author);
        newBook.year = year;
        strcpy(newBook.condition, condition);
        newBook.donated = donated;
        strcpy(newBook.donor, donor);

        books[bookCount++] = newBook;
    }

    fclose(file);
}

void logout(User* user) {
    if (!user || !user->loggedIn) {
        printf("You are not currently logged in.\n");
        return;
    }

    user->loggedIn = false;
    printf("Logged out successfully.\n");
}

int main() {
    int choice;
    User* loggedInUser = NULL;

    printf("Welcome to Book Buddies - A program to donate used books to other students.\n");

    loadUsersFromFile();
    loadBooksFromFile();

    while (1) {
        displayMenu(loggedInUser);

        printf("\nEnter your choice (1-6): ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                registerUser();
                break;
            case 2: {
                loggedInUser = login();
                if (loggedInUser) {
                    printf("Login successful!\n");
                } else {
                    printf("Invalid username or password. Please try again.\n");
                }
                break;
            }
            case 3:
                donateBook(loggedInUser);
                break;
            case 4:
                listBooks();
                break;
            case 5:
                requestBook(loggedInUser);
                break;
            case 6:
                if (loggedInUser && loggedInUser->loggedIn) {
                    logout(loggedInUser);
                    loggedInUser = NULL;
                } else {
                    saveUsersToFile();
                    saveBooksToFile();
                    printf("Thank you for using Book Buddies. Goodbye!\n");
                    exit(0);
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
