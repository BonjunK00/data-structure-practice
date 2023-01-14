#include <stdio.h>
#include <stdlib.h>

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;

struct AVLNode {
    int Element;
    AVLTree Left;
    AVLTree Right;
    int Height;
};

int Max(int a, int b) {
    if(a > b)
        return a;
    else
        return b;
}
int Height(Position P) {
    if(P == NULL)
        return -1;
    else
        return P->Height;
}

AVLTree CreateTree() {
    AVLTree T = malloc(sizeof(struct AVLNode));
    T->Left = T->Right = NULL;
}

Position SingleRotateWithLeft(Position K2) {
    Position K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;

    return K1;
}
Position SingleRotateWithRight(Position K2) {
    Position K1 = K2->Right;
    K2->Right = K1->Left;
    K1->Left = K2;
                    
    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
                            
    return K1;
}
Position DoubleRotateWithLeft(Position K3) {
    K3->Left = SingleRotateWithRight(K3->Left);

    return SingleRotateWithLeft(K3);
}
Position DoubleRotateWithRight(Position K3) {
    K3->Right = SingleRotateWithLeft(K3->Right);

    return SingleRotateWithRight(K3);
}

AVLTree Insert(int X, AVLTree T, FILE* fp) {
    if (T == NULL) {
        T = malloc(sizeof(struct AVLNode));
     
        T->Element = X;
        T->Height = 0;
        T->Left = T->Right = NULL;
    }
    else if (X < T->Element) {
        T->Left = Insert(X, T->Left, fp);
        if(Height(T->Left) - Height(T->Right) == 2)
            if(X < T->Left->Element)
                T = SingleRotateWithLeft(T);
            else
                T = DoubleRotateWithLeft(T);
    }
    else if (X > T->Element) {
        T->Right = Insert(X, T->Right, fp);
        if(Height(T->Right) - Height(T->Left) == 2)
            if(X > T->Right->Element)
                T = SingleRotateWithRight(T);
            else
                T = DoubleRotateWithRight(T);
    }
    
    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

int CheckTree(int X, AVLTree T) {
    while(T) {
        if(X < T->Element)
            T = T->Left;
        else if (X > T->Element)
            T = T->Right;
        else
            return 1;
    }
    return 0;
}
void InorderTraversal(AVLTree T, FILE* fp) {
    if(T) {
        InorderTraversal(T->Left, fp);
        fprintf(fp, "%d(%d) ", T->Element, T->Height);
        InorderTraversal(T->Right, fp);
    }
}

int main() {
    FILE *inFp, *outFp;
    inFp = fopen("input.txt", "r");
    outFp = fopen("output.txt", "w");

    AVLTree T = CreateTree();

    while(1) {
        int x;
        fscanf(inFp, "%d", &x);
        
        if(CheckTree(x, T->Left))
            fprintf(outFp, "%d already in the tree!\n", x);
        else {
            T->Left = Insert(x, T->Left, outFp);
            InorderTraversal(T->Left, outFp);

            fprintf(outFp, "\n");
        }

        if(feof(inFp)) break;
    }

    fclose(inFp);
    fclose(outFp);

    return 0;
}


