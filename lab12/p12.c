#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct ListNode *Position;
typedef Position List;
typedef struct HashTbl *HashTable;

struct ListNode {
    int Element;
};
struct HashTbl {
    int TableSize;
    List Table;
};

HashTable CreateHash(int size) {
    HashTable H = malloc(sizeof(struct HashTbl));

    H->TableSize = size;
    H->Table = malloc(size * sizeof(struct ListNode));

    int i;
    for(i = 0; i < size; i++)
        H->Table[i].Element  = 0;

    return H;
}

int Hash(int value, int Size, int i, int solution) {
    if(solution == 1)
        return ((value % Size) + i) % Size;
    else if(solution == 2)
        return ((value % Size) + i*i) % Size;
    else if(solution == 3)
        return ((value % Size) + i*(7 - (value % 7))) % Size;
    else
        return -1;
}

void Insert(HashTable H, int value, int solution, FILE* fp) {
    int i, h = 0, idx = -1, exist = 0;
    
    for(i = 0; i < H->TableSize; i++) {
        h = Hash(value, H->TableSize, i, solution);
        if(H->Table[h].Element == 0 && idx == -1)
            idx = h;
        if(H->Table[h].Element == value)
            exist = 1;
    }

    if(exist == 1) {
        fprintf(fp, "Already exists\n");
        return;
    }
    if(idx == -1) {
        fprintf(fp, "Full table\n");
        return;
    }
    
    H->Table[idx].Element = value;
    fprintf(fp, "Inserted %d\n", value);
}

void Delete(HashTable H, int value, int solution, FILE* fp) {
    int i, h = 0, exist = 0;

    for(i = 0; i < H->TableSize; i++) {
        h = Hash(value, H->TableSize, i, solution);
        if(H->Table[h].Element == value) {
            exist = 1, H->Table[h].Element = 0;
            break;
        }
    }
            
    if(exist == 0) {
        fprintf(fp, "%d not exists\n", value);
        return;
    }
                    
    fprintf(fp, "Deleted %d\n", value);
}

void Find(HashTable H, int value, int solution, FILE* fp) {
    int i, h = 0, exist = 0, idx = 0;

    for(i = 0; i < H->TableSize; i++) {
        h = Hash(value, H->TableSize, i, solution);
        if(H->Table[h].Element == value) {
            exist = 1, idx = h;
            break;
        }
    }

    if(exist == 0) {
        fprintf(fp, "Not found\n");
        return;
    }
                                    
    fprintf(fp, "Found %d, index : %d\n", value, idx);
}

void Print(HashTable H, FILE* fp) {
    int i;
    for(i = 0; i < H->TableSize; i++)
        fprintf(fp, "%d ", H->Table[i].Element);
    fprintf(fp, "\n\n");
}

int main() {
    FILE *inFp, *outFp;
    inFp = fopen("input.txt", "r");
    outFp = fopen("output.txt", "w");

    int testCase;
    fscanf(inFp, "%d", &testCase);
    
    int i;
    for(i = 0; i < testCase; i++) {
        char sol[15];
        int solution;

        fscanf(inFp, "%s", sol);
        if(!strcmp(sol, "Linear"))
            solution = 1;
        else if(!strcmp(sol, "Quadratic"))
            solution = 2;
        else if(!strcmp(sol, "Double"))
            solution = 3;
        else
            continue;

        int size;
        fscanf(inFp, "%d", &size);

        HashTable H = CreateHash(size);
        
        fprintf(outFp, "%s\n", sol);

        while(1) {
            char mode;
            fscanf(inFp, "%c", &mode);

            if(mode == 'i') {
                int x;
                fscanf(inFp, "%d", &x);
                Insert(H, x, solution, outFp);
            }
            else if(mode == 'd') {
                int x;
                fscanf(inFp, "%d", &x);
                Delete(H, x, solution, outFp);
            }
            else if(mode == 'f') {
                int x;
                fscanf(inFp, "%d", &x);
                Find(H, x, solution, outFp);
            }
            else if(mode == 'p') {
                Print(H, outFp);
            }
            else if(mode == 'q') {
                break;
            }
        }

        free(H->Table);
        free(H);
    }
    
    fclose(inFp);
    fclose(outFp);

    return 0;
}
