#include <stdio.h>
#include <stdlib.h>
#define N 20

// for checking if array is initialized correctly, and if array has been sorted correctly
void printArray(int *arr){
  int i;

  for (i = 0; i < N; ++i){
    printf("%d ", arr[i]);
  }

  printf("\n");
}

// setting randomized values ranging from [-N, N] inclusive
void randomize(int *arr){
  int i;
  for (i = 0; i < N; ++i){
    arr[i] = ((rand() % (2*N)) - N);  
  }
}

void merge(int * a, int start, int mid, int end)  {  
   int i = start;
   int j = mid + 1;
   int k;
   int index = start;  
   int * temp = (int *) malloc(sizeof(int)*(end-start + 1));  

   while(i<=mid && j<=end){  
     if(a[i]<a[j]){  
         temp[index] = a[i];  
         i = i+1;  
     } else{  
         temp[index] = a[j];  
         j = j+1;   
     }  
     
     index++;  
   }  
 
   if(i>mid){  
      while(j<=end){  
         temp[index] = a[j];  
         index++;  
         j++;  
      }  
   } else {  
      while(i<=mid){  
         temp[index] = a[i];  
         index++;  
         i++;  
      }  
   }  

   k = start;  

   while(k < index) {  
      a[k]=temp[k];  
      k++;  
   }
}

void mergeSort(int a[], int start, int end)  
{  
   int mid;  
   if(start < end) {  
      mid = (start+end)/2;  
      mergeSort(a,start,mid);  
      mergeSort(a,mid+1,end);  
      merge(a,start,mid,end);  
    }  
} 

int main() { 
   int *arr = (int *) malloc(sizeof(int)*N);
   randomize(arr);

   printf("List before sorting\n");
   
   printArray(arr);

   mergeSort(arr, 0, N-1);

   printf("\nList after sorting\n");
   
   printArray(arr);
}