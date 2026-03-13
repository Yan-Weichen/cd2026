#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char ch;
    int count;
    struct Node *next;
} Node;

Node *table[256];
Node *head = NULL;
Node *tail = NULL;

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

void print_list() {
    Node *cur = head;
    while (cur != NULL) {
        printf("%c", cur->ch);
        if (cur->next != NULL) {
            printf(",");
        }
        cur = cur->next;
    }
    printf("\n");
}

void free_list() {
    Node *cur = head;
    while (cur != NULL) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

int main() {
    memset(table, 0, sizeof(table));

    FILE *fp = fopen(__FILE__, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot open %s\n", __FILE__);
        return 1;
    }

    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            continue;
        }
        insert((char)c);
    }

    fclose(fp);

    print_list();

    free_list();
    return 0;
}
