#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_AUTHOR 50

typedef struct Book {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int isBorrowed;
    struct Book *next;
} Book;

Book *head = NULL;

// Function to add a new book
void addBook(int id, const char *title, const char *author) {
    Book *newBook = (Book*)malloc(sizeof(Book));
    newBook->id = id;
    strncpy(newBook->title, title, MAX_TITLE);
    strncpy(newBook->author, author, MAX_AUTHOR);
    newBook->isBorrowed = 0;
    newBook->next = head;
    head = newBook;
}

// Function to display all books
void displayBooks(void) {
    Book *temp = head;
    printf("\n--- Library Books ---\n");
    if (!temp) {
        printf("No books available.\n");
        return;
    }
    while (temp) {
        printf("ID: %d | Title: %s | Author: %s | %s\n",
               temp->id, temp->title, temp->author,
               temp->isBorrowed ? "Borrowed" : "Available");
        temp = temp->next;
    }
}

// Search book by ID
Book* searchBook(int id) {
    Book *temp = head;
    while (temp) {
        if (temp->id == id) return temp;
        temp = temp->next;
    }
    return NULL;
}

// Borrow a book
void borrowBook(int id) {
    Book *book = searchBook(id);
    if (book && !book->isBorrowed) {
        book->isBorrowed = 1;
        printf("Book borrowed successfully!\n");
    } else {
        printf("Book not available.\n");
    }
}

// Return a book
void returnBook(int id) {
    Book *book = searchBook(id);
    if (book && book->isBorrowed) {
        book->isBorrowed = 0;
        printf("Book returned successfully!\n");
    } else {
        printf("Invalid return.\n");
    }
}

// Save books to file
void saveToFile(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error saving file.\n");
        return;
    }
    Book *temp = head;
    while (temp) {
        fprintf(fp, "%d,%s,%s,%d\n", temp->id, temp->title, temp->author, temp->isBorrowed);
        temp = temp->next;
    }
    fclose(fp);
}

// Load books from file
void loadFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;
    int id, borrowed;
    char title[MAX_TITLE], author[MAX_AUTHOR];
    while (fscanf(fp, "%d,%49[^,],%49[^,],%d\n", &id, title, author, &borrowed) == 4) {
        addBook(id, title, author);
        Book *b = searchBook(id);
        if (b) b->isBorrowed = borrowed;
    }
    fclose(fp);
}

// Main menu
int main(void) {
    int choice, id;
    char title[50], author[50];
    
    loadFromFile("data.txt");

    while (1) {
        printf("\n--- Library Menu ---\n");
        printf("1. Add Book\n2. Display Books\n3. Search Book\n4. Borrow Book\n5. Return Book\n6. Save & Exit\nChoice: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        switch (choice) {
            case 1:
                printf("Enter ID: ");
                scanf("%d", &id);
                getchar();
                printf("Enter Title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0; // remove newline
                printf("Enter Author: ");
                fgets(author, sizeof(author), stdin);
                author[strcspn(author, "\n")] = 0;
                addBook(id, title, author);
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                printf("Enter ID: ");
                scanf("%d", &id);
                Book *b = searchBook(id);
                if (b) printf("Found: %s by %s (%s)\n", b->title, b->author, b->isBorrowed ? "Borrowed" : "Available");
                else printf("Not found!\n");
                break;
            case 4:
                printf("Enter ID: ");
                scanf("%d", &id);
                borrowBook(id);
                break;
            case 5:
                printf("Enter ID: ");
                scanf("%d", &id);
                returnBook(id);
                break;
            case 6:
                saveToFile("data.txt");
                printf("Data saved. Exiting...\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
}
