#include <stdio.h>
#include <stdlib.h>

#define order 3

struct B_node {
    int n_keys;
    struct B_node *child[order];
    int key[order - 1];
};

struct B_node *CreateNode() {
    struct B_node *ptr = malloc(sizeof(struct B_node));
    ptr->n_keys = 0;
    int i;
    for(i = 0; i < order - 1; i++) {
        ptr->child[i] = NULL;
        ptr->key[i] = 1234567890;
    }
    ptr->child[i] = NULL;
    return ptr;
}

int Insert(int key, struct B_node *ptr, FILE* fp) {
    int i, j;
    int tmp = -1;
    for(i = 0; i < ptr->n_keys; i++)
        if(ptr->key[i] > key)
            break;
    if(ptr->child[i] != NULL)
        tmp = Insert(key, ptr->child[i], fp);
    else if(ptr->n_keys < order - 1) {
        for(j = ptr->n_keys; j > i; j--) {
            ptr->child[j + 1] = ptr->child[j];
            ptr->key[j] = ptr->key[j - 1];
        }
        ptr->key[i] = key;

        ptr->n_keys++;
        return -1;
    }
    else
        tmp = key;

    if(tmp != -1) {
        for(i = 0; i < ptr->n_keys; i++)
            if(ptr->key[i] > tmp)
                break;
        if(ptr->n_keys != order - 1) {
            for(j = ptr->n_keys; j > i; j--) {
                ptr->child[j + 1] = ptr->child[j];
                ptr->key[j] = ptr->key[j - 1];
            }
            struct B_node *temp_node = ptr->child[j];
            ptr->child[j + 1] = temp_node->child[1];
            ptr->child[j] = temp_node->child[0];
            ptr->key[i] = tmp;
            free(temp_node);
            ptr->n_keys++;
        }
        else {
            int largest_key;
            struct B_node *largest_child = NULL;
            if(i == ptr->n_keys) {
                if(ptr->child[order - 1] != NULL) {
                    struct B_node *temp_node = ptr->child[order - 1];
                    largest_child = temp_node->child[1];
                    ptr->child[order - 1] = temp_node->child[0];
                    free(temp_node);
                }
                else
                    largest_child = NULL;
                largest_key = tmp;
            }
            else {
                largest_key = ptr->key[order - 2];
                largest_child = ptr->child[order - 1];
                for(j = ptr->n_keys - 1; j > i; j--) {
                    ptr->child[j + 1] = ptr->child[j];
                    ptr->key[j] = ptr->key[j - 1];
                }
                if(ptr->child[j] != NULL) {
                    struct B_node *temp_node = ptr->child[j];
                    ptr->child[j + 1] = temp_node->child[1];
                    ptr->child[j] = temp_node->child[0];
                    free(temp_node);
                }
                ptr->key[i] = tmp;

            }

            struct B_node *a = CreateNode();
            struct B_node *b = CreateNode();

            for(i = 0; i < order / 2; i++) {
                a->child[i] = ptr->child[i];
                a->key[i] = ptr->key[i];
            }
            a->child[i] = ptr->child[i];
            for(i = 0;i < (order - 1) / 2 - 1; i++) {
                b->child[i] = ptr->child[i + order/2 + 1];
                b->key[i] = ptr->key[i + order/2 + 1];
            }
            b->child[i] = ptr->child[i + order/2 + 1];
            b->child[i + 1] = largest_child;
            b->key[i] = largest_key;
            a->n_keys = order / 2;
            b->n_keys = (order - 1) / 2;

            ptr->child[0] = a;
            ptr->child[1] = b;
            ptr->key[0] = ptr->key[order / 2];
            ptr->n_keys = 1;
            for(i = 1; i < order - 1; i++) {
                ptr->child[i + 1] = NULL;
                ptr->key[i] = 1234567890;
            }
            return ptr->key[0];
        }
    }
    return -1;
}

void Inorder(struct B_node *ptr, FILE* fp) {
    if(ptr != NULL) {
        int i;
        for(i = 0; i < ptr->n_keys; i++) {
             Inorder(ptr->child[i], fp);
             fprintf(fp, "%d ", ptr->key[i]);
        }
        Inorder(ptr->child[i], fp);
    }
}

int main() {
    FILE *inFp, *outFp;
    inFp = fopen("input.txt", "r");
    outFp = fopen("output.txt", "w");

    struct B_node* Btree = CreateNode();

    while(1) {
        char mode, delete_enter;
        fscanf(inFp, "%c", &mode);
        
        if(feof(inFp)) break;

        if(mode == 'i') {
            int num;
            fscanf(inFp, "%d", &num);
            Insert(num, Btree, outFp);
        }
        else if(mode == 'p') {
            Inorder(Btree, outFp);
            fprintf(outFp,"\n");
        }
    
    }
    
    fclose(inFp);
    fclose(outFp);

    return 0;

}
