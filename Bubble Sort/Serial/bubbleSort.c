#include <stdio.h>
#include <stdlib.h>
#define N 20

// for checking if array is initialized correctly, and if array has been sorted correctly
void printArray(int *arr){
  int i;

  for (i = 0; i < N; ++i){
    printf("%s ", arr[i]);
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
    arr[i] = (rand() % (2*N)) - N;  
  }
}

// Driver program to test above functions
int main(){
  int *arr = (int *) malloc(sizeof(int)*N);

  randomize(arr);
  bubbleSort(arr, N);
  
  

  return 0;
}