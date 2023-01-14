#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct QueueElement *Queue;
typedef struct Graph *Graph;

struct Graph {
    int size;
    int* nodes;
    int** matrix;
};

Graph CreateGraph(int* vertex) {
    Graph grp = malloc(sizeof(struct Graph));
    
    int i, j;
    for(i = 0; i < 10; i++)
        if(vertex[i] == 0)
            break;
    
    grp->size = i;
    grp->nodes = vertex;
    
    grp->matrix = malloc(sizeof(int*) * grp->size);
    for(i = 0; i < grp->size; i++)
        grp->matrix[i] = malloc(sizeof(int) * grp->size);
    
    for(i = 0; i < grp->size; i++)
        for(j = 0; j < grp->size; j++)
            grp-> matrix[i][j] = 0;

    return grp;
}

void InsertEdge(Graph G, int a, int b) {
    int a_idx, b_idx;
    int i;
    for(i = 0; i < G->size; i++) {
        if(G->nodes[i] == a)
            a_idx = i;
        if(G->nodes[i] == b)
            b_idx = i;
    }
    G->matrix[a_idx][b_idx] = 1;
}

void PrintGraph(Graph G, FILE* fp) {
    fprintf(fp, "Adjacency matrix\n");
    int i, j;
    fprintf(fp, "  ");
    for(i = 0; i < G->size; i++)
        fprintf(fp, "%d ", G->nodes[i]);
    fprintf(fp,"\n");
    for(i = 0; i < G->size; i++) {
        fprintf(fp, "%d ", G->nodes[i]);
        for(j = 0; j < G->size; j++)
            fprintf(fp, "%d ", G->matrix[i][j]);
        fprintf(fp,"\n");
    }
}

struct QueueElement {
    int max_queue_size;
    int size;
    int first;
    int rear;
    int *arr;
};
Queue CreateQueue() {
    Queue Q = malloc(sizeof(struct QueueElement));

    Q->max_queue_size = 100;
    Q->size = 0;
    Q->first = 0;
    Q->rear = -1;

    Q->arr = malloc(sizeof(int) * 100);

    return Q;
}
int Full_Queue(Queue Q) {
    if(Q->size == Q->max_queue_size)
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
        Q->rear = (Q->rear + 1) % Q->max_queue_size;
        Q->arr[Q->rear] = X;
        Q->size++;
    }
}
int Dequeue(FILE* fp, Queue Q) {
    int ret = -1;
    if(Empty_Queue(Q))
        fprintf(fp, "Empty\n");
    else {
        ret = Q->arr[Q->first];
        Q->first = (Q->first + 1) % Q->max_queue_size;
        Q->size--;
    }
    return ret;
}

void Topsort(Graph G, FILE* fp) {
    Queue Q = CreateQueue();
    int indegree[30];

    int i, j;
    for(j = 0; j < G->size; j++) {
        int count = 0;
        for(i = 0; i < G->size; i++)
            if(G->matrix[i][j] == 1)
                count++;
        indegree[j] = count;
    }

    for(i = 0; i < G->size; i++)
        if(indegree[i] == 0)
            Enqueue(fp, i, Q);
    
    fprintf(fp, "TopSort Result : ");
    while(!Empty_Queue(Q)) {
        int v = Dequeue(fp, Q);
        fprintf(fp, "%d ", G->nodes[v]);
        for(i = 0; i < G->size; i++)
            if(G->matrix[v][i] == 1 && --indegree[i] == 0)
                Enqueue(fp, i, Q);
    }
    fprintf(fp,"\n");
}



int main() {
    Queue Q = CreateQueue();
    FILE *inFp, *outFp;
    inFp = fopen("input.txt", "r");
    outFp = fopen("output.txt", "w");

    char inputNum[30];
    int vertex[20] = {};
    fgets(inputNum, sizeof(inputNum), inFp);
    int i, j = 0;
    for(i = 0; inputNum[i]; i++)
        if(inputNum[i] >= '0' && inputNum[i] <= '9')
            vertex[j++] = inputNum[i] - '0';
    
    Graph grp = CreateGraph(vertex);
    
    char inputEdge[100];
    fgets(inputEdge, sizeof(inputEdge), inFp);
    for(i = 0; inputEdge[i]; i++)
        if(inputEdge[i] == '-')
            InsertEdge(grp, inputEdge[i - 1] - '0', inputEdge[i + 1] - '0');
    
    PrintGraph(grp, outFp);
    fprintf(outFp, "\n\n");
    Topsort(grp, outFp);

    fclose(inFp);
    fclose(outFp);

    return 0;
}

