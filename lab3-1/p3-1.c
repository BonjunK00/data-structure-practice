#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StackElement *Stack;

struct StackElement {
    int capacity;
    int topOfStack;
    int *arr;
};

Stack CreateStack() {
    Stack S = malloc(sizeof(struct StackElement));
    
    S->capacity = 100;
    S->topOfStack = -1;
    S->arr = malloc(sizeof(int) * 100);

    return S;
}

int Is_full(Stack S) {
    if(S->topOfStack == S->capacity - 1)
        return 1;
    return 0;
}

int Is_empty(Stack S) {
    if(S->topOfStack == -1)
        return 1;
    return 0;
}

void Push(Stack S, int x, FILE* fp) {
    if(Is_full(S))
        fprintf(fp, "Full\n");
    else
        S->arr[++S->topOfStack] = x;
}

void Pop(Stack S, FILE* fp) {
    if(Is_empty(S))
        fprintf(fp, "Empty\n");
    else
        fprintf(fp, "%d\n", S->arr[S->topOfStack--]);
}

int main() {
    FILE *inputFp, *outputFp;
    inputFp = fopen("input3-1.txt", "r");
    outputFp = fopen("output3-1.txt", "w");

    Stack S = CreateStack();
     
    int banbok = 0;
    fscanf(inputFp, "%d", &banbok);
    
    int i;
    for(i = 0;i < banbok; i++) {
        char func[5];
        fscanf(inputFp, "%s", func);
        if(!strcmp(func, "push")) {
            int x;
            fscanf(inputFp, "%d", &x);
            Push(S, x, outputFp);
        }
        else if(!strcmp(func, "pop")) {
            Pop(S, outputFp);
        }
    }
    
    fclose(inputFp);
    fclose(outputFp);
    
    free(S->arr);
    free(S);

    return 0;
}

