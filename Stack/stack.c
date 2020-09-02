#include <stdio.h>
#include <stdlib.h>

typedef struct Stack{
    int data;
    struct Stack *next;
}stack;

stack *list = NULL;

void push(int n){
    stack *temp = malloc(sizeof(stack));
    temp->data = n;
    if (list == NULL){
        list = temp;
        list->next = NULL;
    }else{
        temp->next = list;
        list = temp;
    }
}

int pop(){
    int n = 0;
    if (list == NULL){
        return -1; //meaning that the stack is empty
    }else{
        n = list->data;
        list = list->next;
    }
    return n;
}

int main(int argc, char const *argv[]){

    for (int i = 0; i < 10; i++){
        printf("Push %d\n", i);
        push(i);
    }


    int n = 0;
    while (n != -1){
        n = pop();
        if (n == -1){
            printf("Stack empty\n");
        }else{
            printf("Poped %d\n", n);
        }
    }
    return 0;
}
