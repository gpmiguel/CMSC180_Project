#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 20000


void swap(int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

void randomize(int *arr){
	int i;
	for (i = 0; i < N; ++i){
	arr[i] = (rand() % (2*N)) - N;  
	}
}

void printArray(int *arr){
	int i;

	for (i = 0; i < N; ++i){
	printf("%d ", arr[i]);
	}

	printf("\n");
}

void shellsort(int * a, int n){
	int hk, d, i, j;

	for(hk = n/2 ; hk >= 1; hk /= 2)
		for(i = hk ; i < n ; i++)
			for(j = i ; j > hk-1 ; j -= hk)
				if(a[j-hk] > a[j])
					swap(&a[j-hk], &a[j]);
				else
					break;
}

int main(){
	int *arr = (int *) malloc(sizeof(int)*N);
	randomize(arr);

	struct timespec t1, t2;
  	long sec, nano;
  	double elapsed;

  	clock_gettime(CLOCK_REALTIME, &t1);

	shellsort(arr, N);

	clock_gettime(CLOCK_REALTIME, &t2);

	sec = t2.tv_sec - t1.tv_sec;
	nano = t2.tv_nsec - t1.tv_nsec;
	elapsed = sec + nano*1e-9;

	printf("Time elapsed for Shell Sort at N = %d is %f seconds\n\n", N, elapsed);


}