#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Each node stores one unique character and its occurrence count */
typedef struct Node {
    char ch;
    int count;
    struct Node *next;
} Node;

/* Direct-access lookup table indexed by ASCII value for O(1) search (bonus) */
Node *table[256];

/* Head and tail of the linked list (maintained in insertion order) */
Node *head = NULL;
Node *tail = NULL;

/* Insert a character: O(1) lookup via direct-access table */
/* If character already exists, increment count; otherwise create a new node */
void insert(char c) {
    unsigned char idx = (unsigned char)c;

    if (table[idx] != NULL) {
        table[idx]->count++;
    } else {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->ch = c;
        newNode->count = 1;
        newNode->next = NULL;

        if (tail == NULL) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        table[idx] = newNode;
    }
}

/* Print all nodes in insertion order */
void print_list() {
    Node *cur = head;
    while (cur != NULL) {
        printf("%c : %d\n", cur->ch, cur->count);
        cur = cur->next;
    }
}

/* Free all allocated nodes */
void free_list() {
    Node *cur = head;
    while (cur != NULL) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

int main() {
    /* Initialize lookup table to NULL */
    memset(table, 0, sizeof(table));

    /* Open this source file itself using the __FILE__ macro */
    FILE *fp = fopen(__FILE__, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot open %s\n", __FILE__);
        return 1;
    }

    int c;
    while ((c = fgetc(fp)) != EOF) {
        /* Skip whitespace characters: space, tab, newline, carriage return */
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            continue;
        }
        insert((char)c);
    }

    fclose(fp);

    /* Print character frequency list */
    print_list();

    free_list();
    return 0;
}
