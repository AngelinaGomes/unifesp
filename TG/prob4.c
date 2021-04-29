//Maximal bipartite matching
//Angelina Gomes RA:120194

#include <stdio.h>
#include <stdlib.h>

int MaxFlow(int **Graph, int N, int M);
int Matching(int **Graph, int M, int c, int check[], int matchR[]);


int main() { 
	int N, M, L;																			//numbers of children, toys and toy indications
	int c, b;																				//the child 'c' like the toy 'b'
	int i, j;
	
	scanf("%d %d %d", &N, &M, &L);															//take the values of N, M and L
	
	int **G = (int**) malloc(N*sizeof(int*));												//create a graph/matrix - child X toy
	for(i = 0; i < N; i++)
		G[i] = (int*) malloc(M*sizeof(int));
	for(i = 0; i < N; i++) {																//initialize the graph with 0's
		for(j = 0; j < M; j++)
			G[i][j] = 0;		
	}
	
	for(i = 0; i < L; i++) {																//take the toy indications
		scanf("%d %d", &c, &b);
		G[c][b] = 1;																		//put it on the graph
	}
	
	printf("%d\n", MaxFlow(G, N, M));														//print the maximal bipartite matching
	
	return 0; 
} 


int Matching(int **Graph, int M, int c, int check[], int match[]) {
	int b;
	for(b = 0; b < M; b++) { 																//for each toy b
		if(Graph[c][b] && !check[b]) {														//if the child c likes b AND hasn't check it
			check[b] = 1; 																	//mark the toy b as check
			if(match[b] < 0 || Matching(Graph, M, match[b], check, match)) {				//if b is free OR the child with b can get another toy
				match[b] = c;																//mark that the child c has taken the toy b
				return 1;
			}
		}
	}
	return 0;																				//the child c doesn't take any toys
}

int MaxFlow(int **Graph, int N, int M) {
	int i, c;
	int max = 0; 																			//counts the number of toys assigned to children
	int match[M];																			//shows which child is using each toy (-1 = no children)
	int check[M];																			//shows which toy is already checked (1 or 0)
	//this last vector is used for all children

	for(i = 0; i < M; i++)																	//mark all toys as unused
		match[i] = -1;

	for (c = 0; c < N; c++) {															//for each child
 		for(i = 0; i < M; i++)
			check[i] = 0;																	//mark all toys as unchecked

		if(Matching(Graph, M, c, check, match)) 											//check if the child c can play with a toy he likes
			max++;
	}
	return max;
} 
