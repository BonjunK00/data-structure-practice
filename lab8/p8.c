#include <stdio.h>
#include <stdlib.h>
typedef struct HeapStruct *PriorityQueue;

struct HeapStruct {
    int Capacity;
    int Size;
    int *Elements;
};

PriorityQueue CreateHeap(int cap) {
    PriorityQueue H;
    H = malloc(sizeof(struct HeapStruct));
    
    H->Capacity = cap;
    H->Size = 0;
    H->Elements = malloc((cap + 1) * sizeof(int));

    H->Elements[0] = 1234567890;

    return H;
}

void Insert(int X, PriorityQueue H, FILE* fp) {
        if (H->Capacity == H->Size) {
        fprintf(fp, "heap is full.\n");
        return;
    }
    
    int i;
    for (i = 1; i <= H->Size; i++) {
        if (H->Elements[i] == X) {
            fprintf(fp, "%d is already in the heap.\n", X);
            return;
        }
    }

    for (i = ++H->Size; H->Elements[i/2] < X; i /= 2)
        H->Elements[i] = H->Elements[i/2];
    H->Elements[i] = X;

    fprintf(fp, "insert %d\n", X);
}
void Find(int X, PriorityQueue H, FILE* fp) {
    int i;
    for (i = 1; i <= H->Size; i++) {
        if(H->Elements[i] == X) {
            fprintf(fp, "%d is in the heap.\n", X);
            return;
        }
    }
    fprintf(fp, "%d is not in the heap.\n", X);
}
void Print(PriorityQueue H, FILE* fp) {
    int i;
    for (i = 1; i <= H->Size; i++)
        fprintf(fp, "%d ", H->Elements[i]);
    fprintf(fp, "\n");
}

int main() {
    FILE *inFp, *outFp;
    inFp = fopen("input.txt", "r");
    outFp = fopen("output.txt", "w");

    int heapSize;
    fscanf(inFp, "%d", &heapSize);

    PriorityQueue Heap = CreateHeap(heapSize);

    while(1) {
        char mode;
        fscanf(inFp, "%c", &mode);
    
        if(feof(inFp)) break;

        if(mode == 'i') {
            int x;
            fscanf(inFp, "%d", &x);
            Insert(x, Heap, outFp);
        }
        else if(mode == 'f') {
            int x;
            fscanf(inFp, "%d", &x);
            Find(x, Heap, outFp);
        }
        else if(mode == 'p')
            Print(Heap, outFp);
    }

    free(Heap->Elements);
    free(Heap);

    fclose(inFp);
    fclose(outFp);

    return 0;
}
