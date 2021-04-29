#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long long int calc_um(long long int num){
	long long int um, div, mod;
	int i;

	um = 0;
	for(i = 0; i <= 56; i++){
		div = num / (long long int)pow(2, i+1);
		um += div * (long long int)pow(2, i);
		mod = num % (long long int)pow(2, i+1);
		if(mod >= (long long int)pow(2, i)){
			um += (mod - (long long int)(pow(2, i) - 1));
		}
	}

	return um;
}

int main(){
	long long int x, y, um;

	scanf("%lld %lld", &x, &y);

	um = calc_um(y) - calc_um(x-1);

	printf("%lld\n", um);

	return 0;
}