#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

int N;

// for checking if array is initialized correctly, and if array has been sorted correctly
void printArray(int *arr){
  int i;

  for (i = 0; i < N; ++i){
    printf("%d ", arr[i]);
  }

  printf("\n");
}

// setting randomized values ranging from [-N/2, N/2] inclusive
void randomize(int *arr){
  int i;
  for (i = 0; i < N; ++i){
    arr[i] = (rand() % (N)) - (N/2);  
  }
}

void merge(int * arr, int start, int mid, int end) {  
   int i = start;
   int j = mid + 1;
   int k;
   int index = start;  
   int * temp = (int *) malloc(sizeof(int)*(N));  

   while(i<=mid && j<=end){  
     if(arr[i] < arr[j]){  
         temp[index] = arr[i];  
         i = i+1;  
     } else{  
         temp[index] = arr[j];  
         j = j+1;   
     }  
     
     index++;  
   }  
 
   if(i>mid){  
      while(j<=end){  
         temp[index] = arr[j];  
         index++;  
         j++;  
      }  
   } else {  
      while(i<=mid){  
         temp[index] = arr[i];  
         index++;  
         i++;  
      }  
   }  

   k = start;  

   while(k < index) {  
      arr[k]=temp[k];  
      k++;  
   }

   free(temp);
}

void mergeSort(int * arr, int start, int end)  
{  
   int mid;  
   if(start < end) {  
      mid = (start+end)/2;  
      mergeSort(arr,start,mid);  
      mergeSort(arr,mid+1,end);  
      merge(arr,start,mid,end);  
    }  
} 

int main() { 

   FILE *fp;

   fp = fopen("mergeSort.csv", "w+");
   fprintf(fp, "N,seconds\n");

   for (N = 1000; N != 501000; N = N + 1000)
   {
      int *arr = (int *) malloc(sizeof(int)*N);
      randomize(arr);

      struct timespec t1, t2;
      long sec, nano;
      double elapsed;

      clock_gettime(CLOCK_REALTIME, &t1);

      mergeSort(arr, 0, N-1);

      clock_gettime(CLOCK_REALTIME, &t2);

      sec = t2.tv_sec - t1.tv_sec;
      nano = t2.tv_nsec - t1.tv_nsec;
      elapsed = sec + nano*1e-9;

      printf("Time elapsed for Merge Sort at N = %d is %f seconds\n\n", N, elapsed);
      fprintf(fp, "%d,%f\n", N, elapsed);

      free(arr);
   }

   
}