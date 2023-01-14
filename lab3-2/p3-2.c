#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct QueueElement *Queue;
struct QueueElement {
    int capacity;
    int size;
    int front;
    int rear;
    int *arr;
};
Queue CreateQueue() {
    Queue Q = malloc(sizeof(struct QueueElement));

    Q->capacity = 100;
    Q->size = 0;
    Q->front = 0;
    Q->rear = -1;

    Q->arr = malloc(sizeof(int) * 100);

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
void Enqueue(FILE* fp, int X, Queue Q) {
    if(Full_Queue(Q))
        fprintf(fp, "Full\n");
    else {
        Q->rear = (Q->rear + 1) % Q->capacity;
        Q->arr[Q->rear] = X;
        Q->size++;
    }
}
void Dequeue(FILE* fp, Queue Q) {
    if(Empty_Queue(Q))
        fprintf(fp, "Empty\n");
    else {
        fprintf(fp, "%d\n", Q->arr[Q->front]);
        Q->front = (Q->front + 1) % Q->capacity;
        Q->size--;
    }
}

int main() {
    Queue Q = CreateQueue();
    FILE *inFp, *outFp;
    inFp = fopen("input3-2.txt", "r");
    outFp = fopen("output3-2.txt", "w");

    int banbok;
    fscanf(inFp, "%d", &banbok);

    int i;
    for(i = 0; i < banbok; i++) {
        char func[4];
        fscanf(inFp, "%s", func);
        
        if(!strcmp(func, "enQ")) {
            int x;
            fscanf(inFp, "%d", &x);
            Enqueue(outFp, x, Q);
        }
        else if(!strcmp(func, "deQ")) {
            Dequeue(outFp, Q);
        }
    }

    fclose(inFp);
    fclose(outFp);
    
    free(Q->arr);
    free(Q);

    return 0;
}

