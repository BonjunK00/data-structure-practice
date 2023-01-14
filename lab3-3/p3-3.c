#include <stdio.h>
#include <stdlib.h>

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

void Push(Stack S, char x, FILE* fp) {
    if(Is_full(S))
        fprintf(fp, "Full\n");
    else
        S->arr[++S->topOfStack] = x;
}

void Pop(Stack S, FILE* fp) {
    if(Is_empty(S))
        fprintf(fp, "Empty\n");
    else
        S->topOfStack--;
}
char Top(Stack S) {
    if(Is_empty(S))
        return 0;
    else
        return S->arr[S->topOfStack];
}
int Grade(char X) {
    if(X == '+' || X == '-')
        return 1;
    else if(X == '*' || X == '/' || X == '%')
        return 2;
    else if(X == '(')
        return 0;
    return -1;
}
void Compare(Stack S, char X, FILE* fp, int* k, char* post) {
    if(X == '(')
        Push(S, X, fp);
    else if(X == ')') {
        while(Top(S) != '(') {
            post[(*k)++] = Top(S);
            Pop(S, fp);
        }
        Pop(S, fp);
    }
    else if(Is_empty(S) || Grade(X) > Grade(Top(S)))
        Push(S, X, fp);
    else {
        post[(*k)++] = Top(S);
        Pop(S, fp);
        Compare(S, X, fp, k, post);
    }
}
int main() {
    FILE *inputFp, *outputFp;
    inputFp = fopen("input3-3.txt", "r");
    outputFp = fopen("output3-3.txt", "w");

    Stack S1 = CreateStack();
    
    char infix[110], postfix[110];
    fscanf(inputFp, "%s", infix);
    
    int k = 0, i;
    for(i = 0; infix[i] != '#'; i++) {
        if(infix[i] > '0' && infix[i] <= '9')
            postfix[k++] = infix[i];
        else
            Compare(S1, infix[i], outputFp, &k, postfix);
    }
    while(!Is_empty(S1)) {
        postfix[k++] = Top(S1);
        Pop(S1, outputFp);
    }
    postfix[k] = '#';
    
    Stack S2 = CreateStack();

    int eval = 0;
    for(i = 0; postfix[i] != '#'; i++) {
        if(postfix[i] > '0' && postfix[i] <= '9')
            Push(S2, postfix[i] - '0', outputFp);
        else {
            int b = Top(S2);
            Pop(S2, outputFp);
            int a = Top(S2);
            Pop(S2, outputFp);
            switch(postfix[i]) {
                case '+' :
                    Push(S2, a + b, outputFp);
                    break;
                case '-' :
                    Push(S2, a - b, outputFp);
                    break;
                case '*' :
                    Push(S2, a * b, outputFp);
                    break;
                case '/' :
                    Push(S2, a / b, outputFp);
                    break;
                case '%' :
                    Push(S2, a % b, outputFp);
            }
            printf("%d\n", Top(S2));
        }
    }
    eval = Top(S2);

    fprintf(outputFp, "Infix Form : ");
    for(i = 0; infix[i] != '#'; i++)
        fprintf(outputFp, "%c", infix[i]);
    fprintf(outputFp, "\n");

    fprintf(outputFp, "Postfix Form : ");
    for(i = 0; postfix[i] != '#'; i++)
        fprintf(outputFp, "%c", postfix[i]);
    fprintf(outputFp, "\n");
    
    fprintf(outputFp, "Evaluation Result : %d\n", eval);

    fclose(inputFp);
    fclose(outputFp);
    
    free(S1->arr);
    free(S1);
    
    free(S2->arr);
    free(S2);

    return 0;
}

