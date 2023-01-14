#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct TreeElement *Tree;
typedef struct TreeElement *Node;
struct TreeElement {
    int value;
    Node left, right;
};

Tree CreateTree() {
    Tree T = malloc(sizeof(struct TreeElement));
    T->left = T->right = NULL;
    return T;
}
Node InsertNode(int X, Tree T, FILE* fp) {
    if(T == NULL) {
        T = malloc(sizeof(struct TreeElement));
        T->value = X;
        T->left = T->right = NULL;
    }
    else if (X < T->value)
        T->left = InsertNode(X, T->left, fp);
    else if (X > T->value)
        T->right = InsertNode(X, T->right, fp);
    else
        fprintf(fp, "%d already exists.\n", X);
    return T;
}
int FindMaxAndDeleteMax(Tree T) {
    if(T == NULL)
        return 0;
    while(T->right->right)
        T = T->right;
    
    Node tmp = T->right;
    int ret = T->right->value;

    T->right = T->right->left;
    free(tmp);

    return ret;
}
Node DeleteNode(int X, Tree T, FILE* fp) {
    if(T == NULL) {
        fprintf(fp, "Deletion failed. %d does not exist.\n", X);
        return NULL;
    }
    else if(X < T->value)
        T->left = DeleteNode(X, T->left, fp);
    else if(X > T->value)
        T->right = DeleteNode(X, T->right, fp);
    else if(!T->left || !T->right) {
        Node tmp = T;
        if(!T->left) {
            T = T->right;
            free(tmp);
        }
        else {
            T = T->left;
            free(tmp);
        }
    }
    else if(!T->left->right) {
        Node tmp = T->left;
        T->left = T->left->left;
        T->value = tmp->value;
        free(tmp);
    }
    else {
        int tmp = FindMaxAndDeleteMax(T->left);
        T->value = tmp;
    }
    return T;
}
Node FindNode(int X, Tree T, FILE* fp) {
    if(T == NULL) {
        fprintf(fp, "%d is not in the tree.\n", X);
        return NULL;
    }
    else if(X < T->value)
        FindNode(X, T->left, fp);
    else if(X > T->value)
        FindNode(X, T->right, fp);
    else {
        fprintf(fp, "%d is in the tree.\n", X);
        return T;
    }
}
void PrintInorder(Tree T, FILE* fp) {
    if(T) {
        PrintInorder(T->left, fp);
        fprintf(fp, "%d ", T->value);
        PrintInorder(T->right, fp);
    }
}
void PrintPreorder(Tree T, FILE* fp) {
    if(T) {
        fprintf(fp, "%d ", T->value);
        PrintPreorder(T->left, fp);
        PrintPreorder(T->right, fp);
    }
}
void PrintPostorder(Tree T, FILE* fp) {
    if(T) {
        PrintPostorder(T->left, fp);
        PrintPostorder(T->right, fp);
        fprintf(fp, "%d ", T->value);
    }
}

int main() {
    Tree BST = CreateTree();
    FILE *inFp, *outFp;
    inFp = fopen("input.txt", "r");
    outFp = fopen("output.txt", "w");

    while(1) {
        char mode[3];
        fscanf(inFp, "%s", mode);
        if(feof(inFp)) break;

        if(!strcmp(mode, "i")) {
            int x;
            fscanf(inFp, "%d", &x);
            BST->left = InsertNode(x, BST->left, outFp);
        }
        else if(!strcmp(mode, "d")) {
            int x;
            fscanf(inFp, "%d", &x);
            DeleteNode(x, BST->left, outFp);
        }
        else if(!strcmp(mode, "f")) {
            int x;
            fscanf(inFp, "%d", &x);
            FindNode(x, BST->left, outFp);
        }
        else if(!strcmp(mode, "pi")) {
            fprintf(outFp, "pi - ");
            PrintInorder(BST->left, outFp);
            fprintf(outFp, "\n");
        }
        else if(!strcmp(mode, "pr")) {
            fprintf(outFp, "pr - ");
            PrintPreorder(BST->left, outFp);
            fprintf(outFp, "\n");
        }
        else if(!strcmp(mode, "po")) {
            fprintf(outFp, "po - ");
            PrintPostorder(BST->left, outFp);
            fprintf(outFp, "\n");
        }
    }

    fclose(inFp);
    fclose(outFp);

    return 0;
}
                
            
