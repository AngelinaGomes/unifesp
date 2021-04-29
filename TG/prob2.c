//minimum spanning tree
//implementation of Kruskal's algorithm
//Angelina Gomes RA:120194

#include <stdio.h>
#include <stdlib.h>

typedef struct edge {
	int u, v, w;      						//undirected edge (u,v) with weight = w
} Edge; 

typedef struct graph { 
	int m, n;
	Edge* edge;
} Graph;

typedef struct makeSet { 
	int p; 
	int rank; 
} MakeSet;

//prototypes
Graph* CreateGraph(int m, int n);
int FindSet(MakeSet sets[], int i);
void Union(MakeSet sets[], int x, int y);
int Compare(const void* a, const void* b);
int KruskalMST(Graph* G);


int main() {
	int m, n;                               //number of bus stops and routes
    int u, v, w;                            //u and v are bus stops w meters away
    int wG = 0, wT = 0;                     //weights of G and T
    int i, j;                               //auxiliary variables 

    //initializing
    scanf("%d %d", &m, &n);
	Graph* G = CreateGraph(m, n);           //city route map
    
	for(i = 0; i < n; i++){
		scanf("%d %d %d", &u, &v, &w);
		G->edge[i].u = u;
		G->edge[i].v = v;
		G->edge[i].w = w;
		wG = wG + w;
	}
    
	wT = KruskalMST(G); 

    printf("%d\n", wG-wT);                  //wG - wT = G's weight without excluded routes

	return 0; 
} 


//functions
Graph* CreateGraph(int m, int n) {
    Graph *G = (Graph*) malloc(sizeof(Graph));
	G->m = m;
	G->n = n;
    G->edge = (Edge*) malloc(n*sizeof(Edge));
	return G;
}

int FindSet(MakeSet sets[], int i) {
	if (sets[i].p != i) 
		sets[i].p = FindSet(sets, sets[i].p); 
	return sets[i].p; 
}

void Union(MakeSet sets[], int x, int y) {
	x = FindSet(sets, x); 
	y = FindSet(sets, y);
	if(sets[x].rank > sets[y].rank) 		//if tree(x) < tree(y)
		sets[y].p = x; 						//x becomes y's parent
	else {
		sets[x].p = y;						//else y becomes x's parent
		if(sets[x].rank == sets[y].rank)
			sets[y].rank++;					//if they had the same size
	}
} 

int Compare(const void* a, const void* b) {
	Edge* a1 = (Edge*)a;
	Edge* b1 = (Edge*)b;
	return a1->w > b1->w;
} 

int KruskalMST(Graph* G) { 
	int v, e = 0, f = 0, i, weight = 0;							//e,f control edges of T and G, respectively
	Edge T[G->m], edge; 										//MST and possible edge of T

	qsort(G->edge, G->n, sizeof(G->edge[0]), Compare); 			//sort all the edges in non-decreasing order
	MakeSet *sets = (MakeSet*) malloc((G->m)*sizeof(MakeSet));
	for(v = 0; v < G->m; v++) { 								//create a forest with m trees of single vertex v
		sets[v].p = v;
		sets[v].rank = 0; 
	}
 
	while(e < (G->m)-1 && f < (G->n)) {
		edge = G->edge[f]; 										//for each edge of G
		f++;
		int x = FindSet(sets, edge.u); 
		int y = FindSet(sets, edge.v); 
		if(x != y) { 											//if u and v are in different components
			T[e] = edge;										//add edge (u,v) to T
			e++;
			Union(sets, x, y); 
		} 
	}
	
	for(i = 0; i < e; i++)
		weight = weight + T[i].w;								//add all MST weights
	return weight;
}