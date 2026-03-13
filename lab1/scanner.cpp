#include <stdio.h>
#include <stdlib.h>

/* 定義 Linked-List 節點結構  */
typedef struct Node {
    char data;
    struct Node* next;
} Node;

/* 新增節點到串列末尾的函式 */
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
    /* 開啟原始碼檔案（例如讀取自己這個檔案） */
    FILE *file = fopen("main.c", "r");
    if (file == NULL) {
        printf("無法開啟檔案！\n");
        return 1;
    }

    Node* head = NULL;
    char c;

    /* 階段 1：逐字元讀取檔案並存入 Linked-List [cite: 60, 65] */
    while ((c = fgetc(file)) != EOF) {
        append(&head, c);
    }
    fclose(file);

    /* 階段 2：從串列中輸出每一個字元（若不從串列輸出則沒分數） */
    printf("Scanner 輸出 (從 Linked-List): \n");
    Node* current = head;
    int count=0;
    while (current != NULL) {
        /* 每個char用,隔開  (包含空格)*/
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

    /* 釋放記憶體 */
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
