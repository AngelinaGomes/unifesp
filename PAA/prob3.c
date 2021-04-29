#include <stdio.h>
#include <stdlib.h>

void printOut(int orig[], int part[], int lenO){
    int i, j=0;

    for(i=0; i<lenO; i++){
        if(orig[i] != -1)
            printf("%d ", orig[i]);
        else{
            printf("%d ", part[j]);
            j++;
        }
    }
    printf("\n");
}


// função recursiva para permutação
void  permutation(int permut[], int setPermut[], int n, int k, int sol[], int orig[], int lenO) {
    int i;
    if(k == n)
        printOut(orig, sol, lenO);
    else {
        for (i=0; i<n ; i++){
            if(setPermut[i] == 0){
                sol[k] = permut[i];
                setPermut[i] = 1;
                permutation(permut, setPermut, n, k+1, sol, orig, lenO);
                setPermut[i] = 0;
            }
        }
    }
}


int main() {
	
	int n, m, row, col, i, j=0;
	
	scanf("%d %d", &n, &m);

    int orig[n], rowCand[n], permut[n-m], setPermt[n-m], sol[n-m];
	
    // inicio
    for(i=0; i<n; i++) {
		orig[i] = -1;
        rowCand[i] = 1; // todas as linhas são candidatas
	}
    for(i=0; i<n-m; i++) {
		permut[i] = -1;
        setPermt[i] = 0;
	}

    // verificando guaritas existentes
	for(i=0; i<m; i++) {
		scanf("%d %d", &row, &col);
		orig[col-1] = row;
        rowCand[row-1] = 0; // linha não candidata
	}		

	for(i=0; i<n; i++){
        if(rowCand[i] == 1){
            permut[j] = i+1;
            j++;
        }
	}

    permutation(permut, setPermt, n-m, 0, sol, orig, n);

	return 0;
}