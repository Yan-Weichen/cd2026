#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Token type definitions */
#define TYPE_TOKEN          1
#define MAIN_TOKEN          2
#define LEFTPAREN_TOKEN     3
#define REFTPAREN_TOKEN     4
#define LEFTBRACE_TOKEN     5
#define REFTBRACE_TOKEN     6
#define ID_TOKEN            7
#define ASSIGN_TOKEN        8
#define LITERAL_TOKEN       9
#define SEMICOLON_TOKEN     10
#define IF_TOKEN            11
#define ELSE_TOKEN          12
#define WHILE_TOKEN         13
#define EQUAL_TOKEN         14
#define GREATEREQUAL_TOKEN  15
#define LESSEQUAL_TOKEN     16
#define GREATER_TOKEN       17
#define LESS_TOKEN          18
#define PLUS_TOKEN          19
#define MINUS_TOKEN         20

/* Token node for linked list (based on hw0 Node structure) */
typedef struct Node {
    char value[256];
    int token_type;
    struct Node *next;
} Node;

/* Head and tail of the token linked list (maintained in insertion order) */
Node *head = NULL;
Node *tail = NULL;

/* Token name lookup (returns string name for a token type) */
const char *token_name(int type) {
    switch (type) {
        case TYPE_TOKEN:         return "TYPE_TOKEN";
        case MAIN_TOKEN:         return "MAIN_TOKEN";
        case LEFTPAREN_TOKEN:    return "LEFTPAREN_TOKEN";
        case REFTPAREN_TOKEN:    return "REFTPAREN_TOKEN";
        case LEFTBRACE_TOKEN:    return "LEFTBRACE_TOKEN";
        case REFTBRACE_TOKEN:    return "REFTBRACE_TOKEN";
        case ID_TOKEN:           return "ID_TOKEN";
        case ASSIGN_TOKEN:       return "ASSIGN_TOKEN";
        case LITERAL_TOKEN:      return "LITERAL_TOKEN";
        case SEMICOLON_TOKEN:    return "SEMICOLON_TOKEN";
        case IF_TOKEN:           return "IF_TOKEN";
        case ELSE_TOKEN:         return "ELSE_TOKEN";
        case WHILE_TOKEN:        return "WHILE_TOKEN";
        case EQUAL_TOKEN:        return "EQUAL_TOKEN";
        case GREATEREQUAL_TOKEN: return "GREATEREQUAL_TOKEN";
        case LESSEQUAL_TOKEN:    return "LESSEQUAL_TOKEN";
        case GREATER_TOKEN:      return "GREATER_TOKEN";
        case LESS_TOKEN:         return "LESS_TOKEN";
        case PLUS_TOKEN:         return "PLUS_TOKEN";
        case MINUS_TOKEN:        return "MINUS_TOKEN";
        default:                 return "UNKNOWN_TOKEN";
    }
}

/* Add a token to the linked list (modified from hw0 insert) */
void add_token(const char *value, int type) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    strncpy(newNode->value, value, 255);
    newNode->value[255] = '\0';
    newNode->token_type = type;
    newNode->next = NULL;

    if (tail == NULL) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

/* Print all tokens in order (modified from hw0 print_list) */
void print_list() {
    Node *cur = head;
    while (cur != NULL) {
        printf("%s: %s\n", cur->value, token_name(cur->token_type));
        cur = cur->next;
    }
}

/* Free all allocated nodes (same as hw0 free_list) */
void free_list() {
    Node *cur = head;
    while (cur != NULL) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

/* Check if character is a letter or underscore */
int is_alpha(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

/* Check if character is a digit */
int is_digit(int c) {
    return c >= '0' && c <= '9';
}

/* Determine if a word is a keyword; return its token type */
int keyword_or_id(const char *word) {
    if (strcmp(word, "int") == 0) return TYPE_TOKEN;
    if (strcmp(word, "main") == 0) return MAIN_TOKEN;
    if (strcmp(word, "if") == 0) return IF_TOKEN;
    if (strcmp(word, "else") == 0) return ELSE_TOKEN;
    if (strcmp(word, "while") == 0) return WHILE_TOKEN;
    return ID_TOKEN;
}

int main(int argc, char *argv[]) {
    char filepath[512];

    if (argc >= 2) {
        /* File path provided as command-line argument */
        strncpy(filepath, argv[1], 511);
        filepath[511] = '\0';
    } else {
        /* Prompt user to enter file path */
        printf("Enter file path to scan: ");
        if (fgets(filepath, sizeof(filepath), stdin) == NULL) {
            fprintf(stderr, "Error: failed to read file path\n");
            return 1;
        }
        /* Remove trailing newline */
        int len = strlen(filepath);
        if (len > 0 && filepath[len - 1] == '\n') filepath[len - 1] = '\0';
    }

    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot open %s\n", filepath);
        return 1;
    }

    int c;
    while ((c = fgetc(fp)) != EOF) {
        /* Skip whitespace: space, tab, newline, carriage return */
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            continue;
        }

        /* Identifier or keyword: starts with letter or underscore */
        if (is_alpha(c)) {
            char buf[256];
            int len = 0;
            buf[len++] = (char)c;
            while ((c = fgetc(fp)) != EOF && (is_alpha(c) || is_digit(c))) {
                if (len < 255) buf[len++] = (char)c;
            }
            buf[len] = '\0';
            if (c != EOF) ungetc(c, fp);
            add_token(buf, keyword_or_id(buf));
        }
        /* Integer literal: starts with digit */
        else if (is_digit(c)) {
            char buf[256];
            int len = 0;
            buf[len++] = (char)c;
            while ((c = fgetc(fp)) != EOF && is_digit(c)) {
                if (len < 255) buf[len++] = (char)c;
            }
            buf[len] = '\0';
            if (c != EOF) ungetc(c, fp);
            add_token(buf, LITERAL_TOKEN);
        }
        /* Two-character relational operators and assignment */
        else if (c == '=') {
            int next = fgetc(fp);
            if (next == '=') {
                add_token("==", EQUAL_TOKEN);
            } else {
                if (next != EOF) ungetc(next, fp);
                add_token("=", ASSIGN_TOKEN);
            }
        }
        else if (c == '>') {
            int next = fgetc(fp);
            if (next == '=') {
                add_token(">=", GREATEREQUAL_TOKEN);
            } else {
                if (next != EOF) ungetc(next, fp);
                add_token(">", GREATER_TOKEN);
            }
        }
        else if (c == '<') {
            int next = fgetc(fp);
            if (next == '=') {
                add_token("<=", LESSEQUAL_TOKEN);
            } else {
                if (next != EOF) ungetc(next, fp);
                add_token("<", LESS_TOKEN);
            }
        }
        /* Punctuation tokens */
        else if (c == '(') add_token("(", LEFTPAREN_TOKEN);
        else if (c == ')') add_token(")", REFTPAREN_TOKEN);
        else if (c == '{') add_token("{", LEFTBRACE_TOKEN);
        else if (c == '}') add_token("}", REFTBRACE_TOKEN);
        else if (c == ';') add_token(";", SEMICOLON_TOKEN);
        /* Arithmetic operator tokens */
        else if (c == '+') add_token("+", PLUS_TOKEN);
        else if (c == '-') add_token("-", MINUS_TOKEN);
        /* Skip any unrecognized characters */
    }

    fclose(fp);

    print_list();
    free_list();
    return 0;
}
