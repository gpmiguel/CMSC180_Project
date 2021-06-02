#include <stdio.h>
#include <stdlib.h>
#define N 20


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

	printf("List before sorting\n");

	printArray(arr);

	shellsort(arr, N);

	printf("\nList after sorting\n");

	printArray(arr);
}