#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b){
	int tmp=*b;
	*b=*a;
	*a=tmp;
	return;
}

//sector is expressed by [low, hi)
int quick_sort(int *d, int low, int hi){
	if(hi-low<=1)return;

	int l=low+1, h=hi-1, mid=(low+hi)/2;
	swap(&d[low], &d[mid]);
	int pivot=d[low];

	while(l<h){
		//invariant 1 l<h
		//invariant 2 [low+1, l) is less than or equal to pivot
		//invariant 3 [h-1, hi) is bigger than or equal to pivot

		while(d[l] <= pivot && l<h){
			//invariant 4 d[l] is less than or equal to pivot
			++l;
		}
		//break invariant 4 or 1, now there are 2 possible conditions:
		//d[l] is bigger than pivot, or
		//l==h
		//
		while(d[h] >= pivot && l<h){
			//invariant 5 d[h] is bigger than or equal to pivot
			--h;
		}
		//break invariant 5 or 1, now there are 2 possible conditions:
		//d[h] is less than pivot, or
		//l==h

		if(l<h){
			//the first condition: we don't break invariant 1 so l<h
			//and we are sure that d[l] > d[h]
			//so correct the invariant 4 and 5
			swap(&d[l], &d[h]);
		}
		//the second condition: we break invariant 1 so l==h
		//and we are not sure about the relation of the d[l] and d[h]
		//so we don't know if invarian 4 and 5 are broken or not
	}
	if(pivot>d[l]){
		//check invariant 4 and 5 then make the correction
		swap(&d[low], &d[l]);
	}

	quick_sort(d, low, l);
	quick_sort(d, h, hi);

	return 0;
}

void print_arr(int *d, int n){
	int i=0;
	for(i=0; i<n; ++i){
		printf("%d ", d[i]);
	}
	printf("\n");
	return;
}

int check_sorted(int *d, int n){
	if(n<=1){
		return 1;
	}
	int pre=0, i=1;
	for(i=1; i<n; pre=i, ++i){
		if(d[pre]>d[i]){
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[]){
	int arr[10000]={0};
	int n=sizeof(arr)/sizeof(int);
	int i=0;
	time_t now=time(NULL);
	unsigned int seed=now & 0xFFFFFFFF;

	srandom(seed);

	for(i=0; i<n; ++i){
		arr[i]=random();
	}

	quick_sort(arr, 0, n);

	/*print_arr(arr, n);*/

	if(check_sorted(arr, n)){
		printf("sort ok\n");
	}else{
		printf("sort failed\n");
	}

	return 0;
}

