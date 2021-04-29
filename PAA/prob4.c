#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int P, N, i, j, k;
    scanf("%d %d", &P, &N);

    int t[N], v[N], r[P+1], rt[P+1];
    for(i=0; i<N; i++)
        scanf("%d %d", &t[i], &v[i]);
    memset(r, 0, (P+1)*sizeof(int));
    memset(rt, 0, (P+1)*sizeof(int));
    
    for(i=1; i<=P; i++){
        for(j=0; j<i; j++){
            for(k=0; k<N; k++){
                if((t[k] <= (i-rt[j])) && (r[i] < r[j]+v[k])){
                    r[i] = r[j]+v[k];
                    rt[i] = rt[j]+t[k];
                }
            }
        }
    }

    printf("%d\n", r[P]);

    return 0;
}