//
// APS105 Lab 9
//
// This is a program written to maintain a personal music library,
// using a linked list to hold the songs in the library.
//
// Author: Jordan Greenberg
// Student Number: 1005046530
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// A node in the linked list

// Each string in the node is declared as a character pointer variable,
// so they need to be dynamically allocated using the malloc() function,
// and deallocated using the free() function after use.

typedef struct node {
    char *songName;
    char *artist;
    char *genre;
    struct node *next;
} Node;

// Declarations of linked list functions
typedef struct linkedList {
    Node *head;
} LinkedList;

// Declarations of support functions
// See below the main function for descriptions of what these functions do

void inputStringFromUser(char *prompt, char *s, int arraySize);
void songNameDuplicate(char *songName);
void songNameFound(char *songName);
void songNameNotFound(char *songName);
void songNameDeleted(char *songName);
void artistFound(char *artist);
void artistNotFound(char *artist);
void printMusicLibraryEmpty(void);
void printMusicLibraryTitle(void);

const int MAX_LENGTH = 1024;

//Initialize list
void initList(LinkedList *list) {
    list -> head = NULL;
}

//Determines if list is empty
bool isEmpty(LinkedList *songLibrary) {
    if (songLibrary -> head == NULL) {
        return true;
    }
    return false;
}

//Creates a node
Node *createNode (char *inputTitle, char *inputArtist, char *inputGenre) {
    Node *newSong = (Node *)malloc(sizeof(Node));
    newSong -> songName = (char *)malloc(strlen(inputTitle) * sizeof(char));
    newSong -> artist = (char *)malloc(strlen(inputArtist) * sizeof(char));
    newSong -> genre = (char *)malloc(strlen(inputGenre) * sizeof(char));
    newSong -> next = NULL;
    return newSong;
}

//Determines if song already exists in list (is a duplicate)
bool isDuplicate(LinkedList *songLibrary, char *inputTitle) {
    Node *current = songLibrary -> head;
    bool duplicateFound = false;
    
    //Go through list, determine if there is already an existing song
    while (current != NULL && current -> songName != NULL) {
        if (strcmp(current -> songName, inputTitle) == 0) {
            duplicateFound = true;
            break;
        }
        current = current -> next;
    }
    return duplicateFound;
}

//Inserts a song into the library
void insertSong(LinkedList *songLibrary, char *inputTitle, char *inputArtist, char *inputGenre) {
    //Create the node to store the song data
    Node *newSong = createNode(inputTitle, inputArtist, inputGenre);
    
    //Copy the data into the node
    strcpy(newSong -> songName, inputTitle);
    strcpy(newSong -> artist, inputArtist);
    strcpy(newSong -> genre, inputGenre);
    
    //If the list is currently empty, the head of the list becomes the node
    if (isEmpty(songLibrary)) {
        songLibrary -> head = newSong;
    }
    
    //If the list is not empty, traverse the list to find where to put the new song
    else {
        //Keep track of the node being looked at, and the node directly before
        Node *current = songLibrary -> head;
        Node *parent = NULL;
        
        //Traverse list until a null is reached, or the new song title is before the current song title
        while (current != NULL && strcmp(current -> songName, newSong -> songName) < 0) {
            parent = current;
            current = current -> next;
        }
        
        //The new song comes before the current song (or the end of the list)
        newSong -> next = current;
        
        //If replacing the head, set the head to the new song
        if (parent == NULL) {
            songLibrary -> head = newSong;
        }
        //Set the link of the previous song to the new song
        else {
            parent -> next = newSong;
        }
    }
}

//Prints out library
void printLibrary(LinkedList *songLibrary) {
    
    //Determine if library is empty
    if (isEmpty(songLibrary)) {
        printMusicLibraryEmpty();
    }
    
    //If library is not empty, print out each song entry
    else {
        printMusicLibraryTitle();
        Node *current = songLibrary -> head;
        
        while (current != NULL) {
            printf("\n");
            printf("%s\n", current -> songName);
            printf("%s\n", current -> artist);
            printf("%s\n", current -> genre);
            current = current -> next;
        }
    }
}

//Searches for song based on title, returns null if not found
Node *searchSong(LinkedList *songLibrary, char *title) {
    Node *current = songLibrary -> head;
    
    while (current != NULL) {
        if (strcmp(current -> songName, title) == 0) {
            return current;
        }
        current = current -> next;
    }
    return NULL;
}

//Deletes the entire list
void deleteAll(LinkedList *songLibrary) {
    Node *current = songLibrary -> head;
    Node *temp = NULL;
    
    while (current != NULL) {
        songNameDeleted(current -> songName);
        free(current -> songName);
        free(current -> artist);
        free(current -> genre);
        temp = current;
        current = current -> next;
        free(temp);
    }
    songLibrary -> head = NULL;
}

//Attempts to delete song
bool deleteSong(LinkedList *songLibrary, char *title) {
    //Attempt to locate song
    Node *current = songLibrary -> head;
    Node *parent = NULL;
    
    while (current != NULL && strcmp(current -> songName, title) != 0) {
        parent = current;
        current = current -> next;
    }
    
    //If song was not found, return false
    if (current == NULL) {
        return false;
    }
    
    //If the parent is still NULL, the song was found and the head needs to be deleted
    if (parent == NULL) {
        songLibrary -> head = current -> next;
    }
    
    //If song was found and it's not the head, remove from list
    else {
        parent -> next = current -> next;
    }
    
    //Delete all data
    free(current -> songName);
    free(current -> artist);
    free(current -> genre);
    free(current);
    
    return true;
}

int main(void) {
    // Declare the head of the linked list.
    LinkedList songLibrary;
    initList(&songLibrary);

    // Announce the start of the program
    printf("Personal Music Library.\n\n");
    printf("%s", "Commands are I (insert), D (delete), S (search by song name),\n"
           "P (print), Q (quit).\n");

    char response;
    char input[MAX_LENGTH + 1];
    do {
        inputStringFromUser("\nCommand", input, MAX_LENGTH);

        // Response is the first character entered by user.
        // Convert to uppercase to simplify later comparisons.
        response = toupper(input[0]);

        if (response == 'I') {
            // Insert a song into the linked list.
            // Maintain the list in alphabetical order by song name.
            char inputTitle[MAX_LENGTH + 1];
            char inputArtist[MAX_LENGTH + 1];
            char inputGenre[MAX_LENGTH + 1];
            

            // USE THE FOLLOWING STRINGS WHEN PROMPTING FOR DATA:
            char *promptName = "Song name" ;
            char *promptArtist =  "Artist" ;
            char *promptGenre = "Genre" ;
            
            //Get data from user
            inputStringFromUser(promptName, inputTitle, MAX_LENGTH);
            inputStringFromUser(promptArtist, inputArtist, MAX_LENGTH);
            inputStringFromUser(promptGenre, inputGenre, MAX_LENGTH);
            
            //Ensure the song does not already exist
            if (!isEmpty(&songLibrary) && isDuplicate(&songLibrary, inputTitle)) {
                songNameDuplicate(inputTitle);
            }
            //If it is not a duplicate, insert the song
            else {
                insertSong(&songLibrary, inputTitle, inputArtist, inputGenre);
            }

        }
        else if (response == 'D') {
            // Delete a song from the list.
            char inputTitle[MAX_LENGTH + 1];
            char *prompt = "\nEnter the name of the song to be deleted" ;

            //Get song name from user
            inputStringFromUser(prompt, inputTitle, MAX_LENGTH);
            
            //Attempt to delete song
            bool deleted = deleteSong(&songLibrary, inputTitle);
            
            //If song was deleted, alert user
            if (deleted) {
                songNameDeleted(inputTitle);
            }
            //If not, print error
            else {
                songNameNotFound(inputTitle);
            }

        }
        else if (response == 'S') {
            // Search for a song by its name.
            char inputTitle[MAX_LENGTH + 1];
            char *prompt = "\nEnter the name of the song to search for" ;

            //Get name from user
            inputStringFromUser(prompt, inputTitle, MAX_LENGTH);
            
            //Attempt to locoate song
            Node *song = searchSong(&songLibrary, inputTitle);
            
            //If song was located, print out information
            if (song != NULL) {
                songNameFound(inputTitle);
                printf("\n");
                printf("%s\n", song -> songName);
                printf("%s\n", song -> artist);
                printf("%s\n", song -> genre);
            }
            
            //If song was not located, print error
            else {
                songNameNotFound(inputTitle);
            }

        }
        else if (response == 'P') {
            // Print the music library.
            printLibrary(&songLibrary);
        }
        else if (response == 'Q') {
            ; // do nothing, we'll catch this below
        }
        else {
            // do this if no command matched ...
            printf ("\nInvalid command.\n");
        }
    } while (response != 'Q');

    // Delete the entire linked list.
    deleteAll(&songLibrary);

    // Print the linked list to confirm deletion.
    printLibrary(&songLibrary);

    return 0;
}

// Support Function Definitions

// Prompt the user for a string safely, without buffer overflow
void inputStringFromUser(char *prompt, char *s, int maxStrLength) {
    int i = 0;
    char c;

    printf("%s --> ", prompt);
    while (i < maxStrLength && (c = getchar()) != '\n')
        s[i++] = c;
    s[i] = '\0';
}

// Function to call when the user is trying to insert a song name
// that is already in the personal music library.
void songNameDuplicate(char *songName) {
    printf("\nA song with the name '%s' is already in the music library.\n"
           "No new song entered.\n", songName);
}

// Function to call when a song name was found in the personal music library.
void songNameFound(char *songName) {
    printf("\nThe song name '%s' was found in the music library.\n",
           songName);
}

// Function to call when a song name was not found in the personal music library.
void songNameNotFound(char *songName) {
    printf("\nThe song name '%s' was not found in the music library.\n",
           songName);
}

// Function to call when a song name that is to be deleted
// was found in the personal music library.
void songNameDeleted(char *songName) {
    printf("\nDeleting a song with name '%s' from the music library.\n",
           songName);
}

// Function to call when printing an empty music library.
void printMusicLibraryEmpty(void) {
    printf("\nThe music library is empty.\n");
}

// Function to call to print a title when the entire music library is printed.
void printMusicLibraryTitle(void) {
    printf("\nMy Personal Music Library: \n");
}