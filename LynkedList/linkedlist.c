#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node{
    int id;
    int data;
    struct Node *next;
}node;

int numberNodes = 0;

void saveFile(node *head){
    node *now = NULL;
    FILE *fp = fopen("data.txt", "w");

    if (fp == NULL){
        printf("Error opening file\n");
    }else{
        now = head;
        while (now != NULL){
            fwrite(&now->id, sizeof(int), 1, fp);
            fwrite(&now->data, sizeof(int), 1, fp);
            now = now->next;
        }
    }
}

node *newNode(int id, int data){
    node *temp = malloc(sizeof(node));
    temp->id = id;
    temp->data = data;
    temp->next = NULL;
    return temp;
}

node *addNode(node *head, int id, int data){
    node *new = newNode(id, data);
    node *now = NULL;

    if (head == NULL){// Empty list
        head = new;
    }else{
        now = head;
        while (now->next != NULL){// While exists next
            now = now->next;
        }
        now->next = new;
    }
    return head;
}

node *readFile(node *head){
    int id = 0, data = 0;

    FILE *fp = fopen("data.txt", "r");
    if (fp == NULL){
        printf("Error opening file");
    }else{
        while (fread(&id, sizeof(int), 1, fp) == 1){
            fread(&data, sizeof(int), 1, fp);
            head = addNode(head, id, data);
            numberNodes = id;
        }
    }
    return head;
}

node *delNode(node *head, int id){
    node *now = NULL;
    node *prev = NULL;

    if (head == NULL){// Empty list
        printf("Error: Empty list.\n");
    }else{
        now = head;
        while (now->id != id){
            if (now->next == NULL){// Node not found
                printf("Error: node not found.\n");
                return head;
            }else{
                prev = now;
                now = now->next;
            }
        }
        // Node found
        if (now == head){
            head = head->next;
        }else{
            prev->next = now->next;
        }
    }   
    return head;
}

void editNode(node *head, int id){
    node *now = NULL;

    if (head == NULL){// Empty list
        printf("Error: Empty list.\n");
    }else{
        now = head;
        while (now->id != id){
            if (now->next == NULL){
                printf("Error: node not found.\n");
            }else{
                now = now->next;
            }
        }
        //Node found, and edit example
        now->data = now->data * 10;
    }
}

void previewNode(node *head){
    node *now = NULL;
    
    if (head == NULL){// Empty list
        printf("Error: Empty list.\n");
    }else{
        now = head;
        while (now != NULL){
            printf("ID: %d\n", now->id);
            printf("Data: %d\n", now->data);
            now = now->next;
        }
    }
}

int main(int argc, char const *argv[]){
    int option = 1, id = 0;
    node *head = NULL;

    head = readFile(head);

    while (option){
        printf("1. New\n2. Delete\n3. Edit\n4. Preview\n0. Leave\n>");
        scanf("%d", &option);
        switch (option){
            case 1:
                numberNodes++;
                head = addNode(head, numberNodes, numberNodes*10);
                break;
            case 2:
                printf("ID: ");
                scanf("%d", &id);
                head = delNode(head, id);
                break;
            case 3:
                printf("ID: ");
                scanf("%d", &id);
                editNode(head, id);
                break;
            case 4:
                previewNode(head);
                break;
            case 0:
                saveFile(head);
                printf("Bye!\n");
                break;
            default:
                printf("Error: invalid option.\n");
                break;
        }
        id = 0;
    }
    return 0;
}
