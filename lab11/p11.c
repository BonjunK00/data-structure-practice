#include <stdio.h>
#include <stdlib.h>
typedef struct HeapStruct *PriorityQueue;
typedef struct Graph *Graph;

typedef struct Node {
    int key;
    int weight;
} Node;

Node CreateNode(int k, int w) {
    Node n;
    n.key = k;
    n.weight = w;
    return n;
}

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
            grp-> matrix[i][j] = 99999;

    return grp;
}

void InsertEdge(Graph G, int a, int b, int w) {
    int a_idx, b_idx;
    int i;
    for(i = 0; i < G->size; i++) {
        if(G->nodes[i] == a)
            a_idx = i;
        if(G->nodes[i] == b)
            b_idx = i;
    }
    G->matrix[a_idx][b_idx] = w;
}

struct HeapStruct {
    int Capacity;
    int Size;
    Node *Elements;
};

PriorityQueue CreateHeap(int cap) {
    PriorityQueue H;
    H = malloc(sizeof(struct HeapStruct));
    
    H->Capacity = cap;
    H->Size = 0;
    H->Elements = malloc((cap + 1) * sizeof(int));

    H->Elements[0].weight = -1234567890;

    return H;
}

void Insert(Node X, PriorityQueue H, FILE* fp) {
    if (H->Capacity == H->Size) {
        fprintf(fp, "heap is full.\n");
        return;
    }
    
    int i;
    for (i = ++H->Size; H->Elements[i/2].weight > X.weight; i /= 2)
        H->Elements[i] = H->Elements[i/2];
    H->Elements[i] = X;
}

Node deleteMin(PriorityQueue H) {
    Node MinElement = H->Elements[1], LastElement = H->Elements[H->Size--];
    
    int i, Child;
    for (i = 1; i*2 <= H->Size; i = Child) {
        Child = i * 2;
        if(Child != H->Size && H->Elements[Child + 1].weight < H->Elements[Child].weight)
            Child++;

        if(LastElement.weight > H->Elements[Child].weight)
            H->Elements[i] = H->Elements[Child];
        else
            break;
    }

    H->Elements[i] = LastElement;
    return MinElement;
}

void printShortestPath(Graph grp, int st, int ed, int *pred, FILE* fp) {
    if(st == ed) {
        fprintf(fp, "%d ", grp->nodes[st]);
        return ;
    }
    printShortestPath(grp, st, pred[ed], pred, fp);
    fprintf(fp, "%d ", grp->nodes[ed]);
}

void Dijkstra(int st, int ed, Graph grp, FILE* fp) {
    int d[101], pred[101];
    int i;
    for(i = 0; i < 101; i++) {
        d[i] = 99999;
        pred[i] = 0;
    }
    
    PriorityQueue Q = CreateHeap(1000);
    d[st] = 0;
    for(i = 0; i < grp->size; i++) {
        if(grp->matrix[st][i] != 99999) {
            d[i] = grp->matrix[st][i];
            pred[i] = st;
            Node newNode = CreateNode(i, d[i]);    
            Insert(newNode, Q, fp);
        }
    }

    int pathExsist = 0;
    while(Q->Size != 0) {
        Node u = deleteMin(Q);
        if(u.key == ed) {
            pathExsist = 1;
            break;
        }
        for(i = 0; i < grp->size; i++) {
            if(d[u.key] + grp->matrix[u.key][i] < d[i]) {
                d[i] = d[u.key] + grp->matrix[u.key][i];
                pred[i] = u.key;
                Node newNode = CreateNode(i, d[i]);
                Insert(newNode, Q, fp);
            }
        }
    }
    if(pathExsist == 1)
        printShortestPath(grp, st, ed, pred, fp);
    else
        fprintf(fp, "no path");
    fprintf(fp, "\n");
}


int main() {
    FILE *inFp, *outFp;
    inFp = fopen("input.txt", "r");
    outFp = fopen("output.txt", "w");

    char input[101];
    fgets(input, sizeof(input), inFp);
    
    int nodes[101];
    int i, countNode = 0, num = 0;
    for(i = 0; input[i]; i++) {
        if(input[i] == ' ') {
            nodes[countNode++] = num;
            num = 0;
        }
        else
            num  = 10*num + (input[i] - '0');
    }
    nodes[countNode] = num;
    num = 0;
    Graph grp = CreateGraph(nodes);
    
    int a = 0, b = 0, barCount = 0; 
    fgets(input, sizeof(input), inFp);
    for(i = 0; input[i]; i++) {
        if(input[i] == '-' && barCount == 0) {
            a = num;
            num = 0;
            barCount++;
            continue;
        }
        if(input[i] == '-' && barCount == 1) {
            b = num;
            num = 0;
            barCount = 0;
            continue;
        }
        if(input[i] == ' ') {
            InsertEdge(grp, a, b, num);
            a = 0;
            b = 0;
            num = 0;
            continue;
        }
        num = 10*num + (input[i] - '0');
    }

    int st, ed, stIdx, edIdx;
    fscanf(inFp, "%d%d", &st, &ed);

    for(i = 0; i < grp->size; i++) {
        if(grp->nodes[i] == st)
                stIdx = i;
        if(grp->nodes[i] == ed)
                edIdx = i;
    }
    Dijkstra(stIdx, edIdx, grp, outFp);

    fclose(inFp);
    fclose(outFp);

    return 0;
}
