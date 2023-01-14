#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

typedef struct {
    int studentID;
    char studentFirstName[32];
    char studentLastName[32];
} ElementType;

struct Node {
    ElementType element;
    PtrToNode next;
};

Position Find(int findID, List L) {
    Position P = L;
    while(P->next != NULL && P->next->element.studentID != findID)
        P = P->next;
    return P;
}

void ShowCurrentList (FILE* fp, List L) {
    Position P = L;
    fprintf(fp, "Current List > ");
    while(P->next != NULL) {
        P = P->next;
        fprintf(fp, "%d %s %s", P->element.studentID, P->element.studentFirstName, P->element.studentLastName);
        if(P->next != NULL)
            fprintf(fp, "-");
    }
    fprintf(fp, "\n");
}

void Insert (FILE* fp, int ID, char* firstName, char* secondName, List L) {
    Position P = L;
    while(P->next != NULL && ID > P->next->element.studentID)
        P = P->next;
    if(P->next != NULL && P->next->element.studentID == ID)
        fprintf(fp, "Insertion Failed. ID %d already exists.\n", ID);
    else {
        Position TmpCell;
        TmpCell = malloc(sizeof(struct Node));
        TmpCell->element.studentID = ID;
        strcpy(TmpCell->element.studentFirstName, firstName);
        strcpy(TmpCell->element.studentLastName, secondName);
        TmpCell->next = P->next;
        P->next = TmpCell;
        fprintf(fp, "Insertion Success : %d\n", ID);
        ShowCurrentList(fp, L);
    }
}

int Delete (FILE* fp, int delID, List L) {
    Position P = Find(delID, L);
    Position TmpCell = P->next;
    if(P->next == NULL) {
        fprintf(fp, "Deletion Failed : Student ID %d is not in the list.\n", delID);
        return 1;
    }
    else {
        P->next = TmpCell->next;
        free(TmpCell);
        fprintf(fp, "Deletion Success : %d\n", delID);
        ShowCurrentList(fp, L);
        return 0;
    }
}

void PrintElement (FILE* fp, int ID, List L) {
    Position P = Find(ID, L);
    if (P->next == NULL)
        fprintf(fp, "Find %d Failed. There is no student ID\n", ID);
    else
        fprintf(fp, "Find Success : %d %s %s\n", ID, P->next->element.studentFirstName, P->next->element.studentLastName);
}

void PrintList (FILE* fp, List L) {
    Position P = L->next;
    fprintf(fp, "-----LIST-----\n");
    while(P != NULL) {
       fprintf(fp, "%d %s %s\n", P->element.studentID, P->element.studentFirstName, P->element.studentLastName);
       P = P->next;
    }
    fprintf(fp, "--------------\n");
}

int main(){
    List studentList = malloc(sizeof(struct Node));
    studentList->next = NULL;
    FILE *fp, *outFp;
    fp = fopen("input.txt", "r");
    outFp = fopen("output.txt", "w");
    char mode;
    while (fscanf(fp, "%c", &mode) != EOF) {
        if (mode == 'i') {
            int ID;
            char firstName[32], lastName[32];
            fscanf(fp, "%d%s%s", &ID, firstName, lastName);
            Insert(outFp, ID, firstName, lastName, studentList);
        }
        else if (mode == 'd') {
            int ID;
            fscanf(fp, "%d", &ID);
            Delete(outFp, ID, studentList);
        }
        else if (mode == 'f') {
           int ID;
           fscanf(fp, "%d", &ID);
           PrintElement(outFp, ID, studentList);
        }
        else if (mode == 'p') {
            PrintList(outFp, studentList);
        }
    }
    fclose(fp);
    fclose(outFp);
}
