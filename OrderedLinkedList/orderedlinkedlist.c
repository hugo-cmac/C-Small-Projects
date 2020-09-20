#include <stdio.h>
#include <stdlib.h>

typedef struct Ordered{
    int index;
    int data;
    struct Ordered *next;
}ordered;

ordered *list = NULL;

ordered *newNode(int index, int n){
    ordered *temp = malloc(sizeof(ordered));
    temp->data = n;
    temp->index = index;
    temp->next = NULL;
    return temp;
}

void add(int index, int n){
    ordered *temp = newNode(index, n);

    if (list == NULL){
        list = temp;
        list->next = NULL;
    }else{
        ordered *now = list;
        ordered *prev = NULL;

        while (index > now->index){
            if (now->next == NULL){
                now->next = temp;
                return;
            }
            prev = now;
            now = now->next;
        }

        if(prev != NULL){// Middle
            temp->next = now;
            prev->next = temp;
        }else{// Beginning
            temp->next = list;
            list = temp;
        }
    }
}

void preview(){
    if (list == NULL){
        printf("Error: Empty list.\n");
    }else{
        ordered *now = list;
        while (now != NULL){
            printf("Index: %d\t", now->index);
            printf("Data: %d\n", now->data);
            now = now->next;
        }
    }
}

int main(int argc, char const *argv[]){

    printf("Add 12 on pos 5\n");
    add(5, 12);

    printf("Add 1 on pos 7\n");
    add(7, 1);

    printf("Add 4 on pos 80\n");
    add(80, 4);

    printf("Add 55 on pos 35\n");
    add(35, 55);

    printf("Add 25 on pos 30\n");
    add(30, 25);

    printf("Add 56 on pos 12\n");
    add(12, 56);

    preview();

    return 0;
}
