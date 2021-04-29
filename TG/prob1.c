//knigth graph problem
//Angelina Gomes RA:120194

#include <stdio.h>
#include <stdlib.h>
 
struct node {
    int x, y;
    int level;
    struct node *link;
} *front, *rear;

void Insert(int x, int y, int level);
void Delete();
int IsEmpty();
void GetFirst(int *a, int *b, int *d);
int IsValid(int x, int y);
int Search(int startPos[], int endPos[]);
 
int main() {
    int startPos[2], endPos[2];                            //numerical coordinates for the start and end positions
    int row[2];                                            //row[start, end]
    char column[2];                                        //alphabetic column[start, end]
    int finalDist;

    //ler start and end pos
    scanf("%c%d %c%d", &column[0], &row[0],
                       &column[1], &row[1]);               //INPUT: start:ColumnRow end:ColumnRow
    startPos[0] = row[0] - 1;
    startPos[1] = (int)column[0] - 97;
    endPos[0] = row[1] - 1;
    endPos[1] = (int)column[1] - 97;
    
    finalDist = Search(startPos, endPos);
    printf("%d\n", finalDist);                             //OUTPUT: distance between the start and end positions

    return 0;
}
 
void Insert(int x, int y, int level) {
    struct node *temp;
    temp = (struct node*)malloc(sizeof(struct node));      //new node
    temp->x = x;
    temp->y = y;
    temp->level = level;
    temp->link = NULL;
    
    if (front  ==  NULL) {                                 //empty queue
        front = temp;                                      //front and rear receive new node
        rear = temp;
    } else {                                               //non-empty queue
        rear->link = temp;                                 //the last element and rear receive new node
        rear = temp;
    }
}
 
void Delete() {
    struct node *temp;
    temp = front;                                          //get first element
    
    if (front == NULL) {
        front = NULL;                                      //empty queue
        rear = NULL;
    } else {
        front = front->link;                               //new front element
        free(temp);                                        //delete temp 
    }
}
 
int IsEmpty() {
    if (front == NULL) return 1;                           //queue is empty
    else return 0;                                         //queue isn't empty
}

void GetFirst(int *a, int *b, int *d) {
    if (front != NULL) {
        *a = front->x;
        *b = front->y;
        *d = front->level;
    }
}

int IsValid(int x, int y) { 
    if (x >= 0 && x < 8 && y >= 0 && y < 8) 
        return 1; 
    return 0; 
} 

int Search(int startPos[], int endPos[]) {
    int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2}; 
    int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1};               //possible moves
    
    int xu, yu, lu, xv, yv, i, j;                             //aux values

    int a=0, b=0, c=0;

    int visit[8][8];                                       //matrix of visited elements
    for(i = 0; i < 8; i++)                                 //initializing matrix
        for(j = 0; j < 8; j++) 
            visit[i][j] = 0;                               //mark all as unvisited

    visit[startPos[0]][startPos[1]] = 1;                   //mark start position as visited
    Insert(startPos[0], startPos[1], 0);                   //enqueue start position and set level = 0

    while (!IsEmpty()) {
        GetFirst(&xu, &yu, &lu);                           //getting values from first queue position
        Delete();                                          //dequeue

        if(xu == endPos[0] && yu == endPos[1])             //if it gets the end position
            return lu;                                     //return it level
        
        for(i = 0; i < 8; i++) {                           //for each possible edge (u, v)
            xv = xu + dx[i];
            yv = yu + dy[i];
            
            if (IsValid(xv, yv) && !visit[xv][yv]) {       //if it is a valid edge and isn't marked
                visit[xv][yv] = 1;                         //mark as visited
                Insert(xv, yv, lu+1);                      //enqueue and set it level
                GetFirst(&a, &b, &c);
            }
        }
    }
}