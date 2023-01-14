#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int *Disj_Sets;

int Init(int N, Disj_Sets S) {
    int i;
    for(i = 1; i <= N; i++)
        S[i] = 0;
}
int Find (int X, Disj_Sets S) {
    if(S[X] <= 0)
        return X;
    return S[X] = Find(S[X], S);
}
void Union (int r1, int r2, Disj_Sets S) {
    if(S[r2] < S[r1])
        S[r1] = r2;
    else if(S[r2] > S[r1])
        S[r2] = r1;
    else
        S[r2] = r1, S[r1]--;
}

void PrintMaze(int N, int *Walls, FILE* fp) {
    int i, j;
    for(i = 0; i < N; i++)
        fprintf(fp, "+-");
    fprintf(fp, "+\n");

    for(i = 0; i < N; i++) {
        if(i != 0)
            fprintf(fp, "|");
        else
            fprintf(fp, " ");
        fprintf(fp, " ");

        for(j = 0; j < N-1; j++) {
            if(Walls[(N-1)*i + j] == 1)
                fprintf(fp, " ");
            else
                fprintf(fp, "|");
            fprintf(fp, " ");
        }
        

        if(i == N-1) {
            fprintf(fp, " \n");
            break;
        }
        fprintf(fp, "|\n");

        for(j = 0; j < N; j++) {
            fprintf(fp, "+");
            if(Walls[N*(N - 1) + i*N + j] == 1)
                fprintf(fp, " ");
            else
                fprintf(fp, "-");
        }
        fprintf(fp, "+\n");
    }
    
    for(i = 0; i < N; i++)
        fprintf(fp, "+-");
    fprintf(fp, "+\n");
}



int main() {
    srand((unsigned int)time(NULL));

    FILE *inFp, *outFp;
    inFp = fopen("input.txt", "r");
    outFp = fopen("output.txt", "w");
    
    int Sets[110], N, Walls[200];
    // Walls[i] : i = 0 ~ N(N-1)-1 right walls {(1,2), (2,3), (3,4), ...} (if N is 6)
    // Walls[i] : i = N*(N-1) ~ 2*N(N-1)-1 under walls {(1,7), (2,8), (3,9), ...} (if N is 6)
    
    fscanf(inFp, "%d", &N);
    if(N > 10) {
        fprintf(outFp, "Too big to make a maze\n");
        return 0;
    }
    if(N == 1) {
        fprintf(outFp, "+-+\n\n+-+\n");
        return 0;
    }

    Init(N * N, Sets);
    Init(2 * N * (N-1), Walls);
    
    while(1) {
        int x = rand()%(2*N*(N-1));
        if(Walls[x] == 1)
            continue;

        int a, b;
        if(x < N*(N-1)) {
            a = N*(x/(N-1)) + x%(N-1) + 1;
            b = a + 1;
        }
        else {
            int y = x - N*(N-1);
            a = y + 1;
            b = a + N;
        }
        
        int a_root = Find(a, Sets), b_root = Find(b, Sets);
        if(a_root != b_root) {
            Union(a_root, b_root, Sets);
            Walls[x] = 1;
        }

        if(Find(1, Sets) == Find(N*N, Sets))
            break;
    }
    
    PrintMaze(N, Walls, outFp);

    fclose(inFp);
    fclose(outFp);

    return 0;
}
