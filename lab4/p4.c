#include <stdio.h>
#include <stdlib.h>
typedef struct QueueElement *Queue;
typedef struct threaded_tree *threaded_ptr;
struct QueueElement {
        int capacity;
        int size;
        int front;
        int rear;
        threaded_ptr *arr;
};
Queue CreateQueue() {
        Queue Q = malloc(sizeof(struct QueueElement));

        Q->capacity = 100;
        Q->size = 0;
        Q->front = 0;
        Q->rear = -1;
        
        Q->arr = malloc(sizeof(threaded_ptr) * 100);
        
        return Q;
}
int Full_Queue(Queue Q) {
        if(Q->size == Q->capacity)
            return 1;
        return 0;
}
int Empty_Queue(Queue Q) {
        if(Q->size == 0)
            return 1;
        return 0;
}
void Enqueue(threaded_ptr X, Queue Q) {
        if(!Full_Queue(Q)) {
                Q->rear = (Q->rear + 1) % Q->capacity;
                Q->arr[Q->rear] = X;
                Q->size++;
        }
}
void Dequeue(Queue Q) {
        if(!Empty_Queue(Q)) {
            Q->front = (Q->front + 1) % Q->capacity;
            Q->size--;
        }
}
threaded_ptr Top_Queue (Queue Q) {
    if(!Empty_Queue(Q))
        return Q->arr[Q->front];
    return NULL;
}

struct threaded_tree {
    short int left_thread;
    threaded_ptr left_child;
    char data;
    threaded_ptr right_child;
    short int right_thread;
};

void InsertNode(threaded_ptr node, threaded_ptr tree) {
    if(!tree->left_child) {
        tree->left_child = node;
        node->left_child = tree;
        node->right_child = tree;
    }
    else {
        Queue Q = CreateQueue();
        Enqueue(tree->left_child, Q);
        int a = 0; 
        while(!Empty_Queue(Q)) {
            threaded_ptr tmp = Top_Queue(Q);
            Dequeue(Q);
            
            if(tmp->left_thread) {
                node->left_child = tmp->left_child;
                node->right_child = tmp;
                tmp->left_child = node;
                tmp->left_thread = 0;
                break;
            }
            if(tmp->right_thread) {
                node->left_child = tmp;
                node->right_child = tmp->right_child;
                tmp->right_child = node;
                tmp->right_thread = 0;
                break;
            }

            Enqueue(tmp->left_child, Q);
            Enqueue(tmp->right_child, Q);
        }
        free(Q->arr);
        free(Q);
    }
}
threaded_ptr CreateTree() {
    threaded_ptr tree = malloc(sizeof(struct threaded_tree));
    tree->left_thread = 0;
    tree->right_thread = 0;
    tree->right_child = tree;
    
    return tree;
}
threaded_ptr CreateNode(char X) {
    threaded_ptr node = malloc(sizeof(struct threaded_tree));
    node->left_thread = 1;
    node->right_thread = 1;
    node->data = X;

    return node;
}
threaded_ptr insucc(threaded_ptr tree) {
    threaded_ptr tmp = tree->right_child;
    if(!tree->right_thread)
        while(!tmp->left_thread)
            tmp = tmp->left_child;
    return tmp;
}
void tinorder(threaded_ptr tree, FILE* fp) {
    threaded_ptr tmp = tree;
    for(;;) {
        tmp = insucc(tmp);
        if(tmp == tree) break;
        fprintf(fp, "%c ", tmp->data);
    }
}

int main() {
    FILE *inFp, *outFp;
    inFp = fopen("input.txt", "r");
    outFp = fopen("output.txt", "w");

    threaded_ptr tree = CreateTree();
    
    int n;
    fscanf(inFp, "%d", &n);
    
    int i;
    for(i = 0; i < n; i++) {
        char data, rm_blank;
        fscanf(inFp, "%c%c", &rm_blank, &data);
        threaded_ptr node = CreateNode(data);
        InsertNode(node, tree);
    }

    tinorder(tree, outFp);

    fclose(inFp);
    fclose(outFp);

    return 0;

}
