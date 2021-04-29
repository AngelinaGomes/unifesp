#include <stdio.h>
#include <stdlib.h>

void mergesort(int a[],int i,int j);
void merge(int a[],int i1,int j1,int i2,int j2);

int main(){
    int N, C, i, j, max=0;

	// entradas
	scanf("%d %d", &N, &C);
    
    int t[N];
	for(i=0; i<N; i++)
		scanf("%d", &t[i]);

	i = 0; j = N-1;
	mergesort(t, 0, N-1); //ordenando entrada: nlogn

	while(i < j) {
		if(t[i] + t[j] < C) {
			if(t[i] + t[j] > max) max = t[i] + t[j];
			i++;
		} else if(t[i] + t[j] > C) {
			j--;
		} else {
			max = t[i] + t[j];
			break;
		}
	}

	if(max == 0) printf("NONE\n");
	else printf("%d\n", max);

    return 0;
}

void mergesort(int a[],int i,int j) {
	int mid;
		
	if(i<j)
	{
		mid=(i+j)/2;
		mergesort(a,i,mid);
		mergesort(a,mid+1,j);
		merge(a,i,mid,mid+1,j);
	}
}
 
void merge(int a[],int i1,int j1,int i2,int j2) {
	int temp[50];
	int i,j,k;
	i=i1;
	j=i2;
	k=0;
	
	while(i<=j1 && j<=j2)
	{
		if(a[i]<a[j])
			temp[k++]=a[i++];
		else
			temp[k++]=a[j++];
	}
	
	while(i<=j1)
		temp[k++]=a[i++];
		
	while(j<=j2)
		temp[k++]=a[j++];
		
	for(i=i1,j=0;i<=j2;i++,j++)
		a[i]=temp[j];
}