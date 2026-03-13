#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char data;
    struct Node* next;
} Node;

void append(Node** head, char c) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = c;
    newNode->next = NULL;
    
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    
    Node* last = *head;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = newNode;
}

int main() {

    FILE *file = fopen("main.c", "r");
    if (file == NULL) {
        printf("Error: Could not open file!\n");
        return 1;
    }

    Node* head = NULL;
    char c;


    while ((c = fgetc(file)) != EOF) {
        append(&head, c);
    }
    fclose(file);


    printf("Scanner Output (from Linked-List): \n");
    Node* current = head;
    int count=0;
    while (current != NULL) {
        if (count++)
        	printf(", ");
        
        if (current->data == '\n') {
            printf("\\n");
        } else if (current->data == '\t') {
            printf("\\t");
        } else if (current->data == ' ') {
            printf(" ");
        } else {
            printf("%c", current->data);
        }
        
        current = current->next;
    }
    printf("\n");

    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
