#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sysinfo.h>
#define N 500000

// for checking if array is initialized correctly, and if array has been sorted correctly
void printArray(int *arr){
  int i;

  for (i = 0; i < N; ++i){
    printf("%d ", arr[i]);
  }

  printf("\n");
}

// swapping function
void swap(int *x, int *y){
  int temp = *x;
  *x = *y;
  *y = temp;
}

// actual sorting algorithm
void bubbleSort(int arr[], int n){
  int i, j;

  for (i = 0; i < n-1; i++){
      for (j = 0; j < n-i-1; j++){
        if (arr[j] > arr[j+1]) swap(&arr[j], &arr[j+1]);
      }
        
  }     
}

// setting randomized values ranging from [-N, N] inclusive
void randomize(int *arr){
  int i;
  for (i = 0; i < N; ++i){
    arr[i] = (rand() % (N)) - (N/2); 
  }
}

// Driver program to test above functions
int main(){
  int *arr = (int *) malloc(sizeof(int)*N);

  randomize(arr);

  struct timespec t1, t2;
  long sec, nano;
  double elapsed;

  clock_gettime(CLOCK_REALTIME, &t1);

  bubbleSort(arr, N);
  
  clock_gettime(CLOCK_REALTIME, &t2);

  sec = t2.tv_sec - t1.tv_sec;
  nano = t2.tv_nsec - t1.tv_nsec;
  elapsed = sec + nano*1e-9;

  printf("Time elapsed for Serial Bubble Sort at N = %d is %f seconds\n\n", N, elapsed);


  return 0;
}