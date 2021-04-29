#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void busca_posto(int d[], int n, int m, int *atual){
    int i;
    for(i = *atual+1; i < n; i++){
        if(*atual < 0){ //encontrar primeiro posto
            if(d[i] > m) //posto que nao da pra ir => parar no anterior
                break;
        } else{ //encontrar demais postos
            if(d[i]-d[*atual] > m) //posto que nao da pra ir => parar no anterior
                break;
        }
    }

    *atual = --i;
}

int main(){
    int AB, N, M, i, atual = -1, nPostos = 0; //atual: ultimo posto considerado, nPostos: numero de postos onde deve parar
    scanf("%d %d %d", &AB, &N, &M);

    int d[N], postos[N];
    for(i = 0; i < N; i++)
        scanf("%d", &d[i]);
    

    memset(postos, 0, N*sizeof(int)); //inicializando com 0
    
    if(AB <= M){ //nao precisa parar em nenhum posto
        printf("0\n\n");
        return 0;
    }
    
    busca_posto(d, N, M, &atual); //primeiro posto
        
    if(atual < 0){ //impossivel ir de A a B com um tanque dessa capacidadeprintf("-1\n\n");
        return 0;
    } else{
        nPostos = 1;
        postos[0] = atual+1; //indices do vetor: 0 a n-1, resposta: 1 a n
        if(AB - d[atual] <= M){ //da pra ir do posto atual direto para B
            printf("1\n%d\n", postos[0]);
            return 0;
        }
        while(AB - d[atual] > M){ //ainda precisa parar
            busca_posto(d, N, M, &atual);
            postos[nPostos] = atual+1;
            nPostos++;
        }
        printf("%d\n", nPostos);
        for(i = 0; i < nPostos; i++)
            printf("%d ", postos[i]);
        printf("\n");
        return 0;
    }
}