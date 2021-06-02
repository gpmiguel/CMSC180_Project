// Bucket sort in C

#include <stdio.h>
#include <stdlib.h>

#define N 40   // Array size
#define BUCKETS 2  // Number of buckets

typedef struct Bucket {
  int upperBound;
  int size;
  int bucket[N];
}buckets;

void BucketSort(int * arr);
void printArray(int * arr);
int getBucketIndex(int value, int interval);
void mergeSort(int * arr, int start, int end);
void merge(int * arr, int start, int mid, int end);

// Sorting function
void BucketSort(int * arr) {
  int i, j, interval, index = 0;
  buckets bucketArray[BUCKETS];

  // Create buckets
  interval = (2*N) / BUCKETS;

  for (i = 0; i < BUCKETS; ++i){
      bucketArray[i].upperBound = (-N) + (i+1)*interval;
      bucketArray[i].size = 0;
  }

  // Fill the buckets with respective elements
  for (i = 0; i < N; ++i){
      for (j = 0; j < BUCKETS; ++j){
          if (arr[i] <= bucketArray[j].upperBound){
              bucketArray[j].bucket[bucketArray[j].size] = arr[i];
              bucketArray[j].size += 1;
              break;
          }
      }
  }

  // Sort the elements of each bucket
  for (i = 0; i < BUCKETS; ++i) {
      mergeSort(bucketArray[i].bucket, 0, (bucketArray[i].size)-1);
  }

  // Print the buckets 

  // for (int i = 0; i < BUCKETS; ++i){
  //   for (int j = 0; j < bucketArray[i].size; ++j){
  //     printf("%d ", bucketArray[i].bucket[j]);
  //   }
  //   printf("\n");
  // }

  // Put sorted elements on arr
  for (i = 0; i < BUCKETS; ++i) {
      for (j = 0; j < bucketArray[i].size; ++j){
          arr[index++] = bucketArray[i].bucket[j];
      }
  } 
}

int getBucketIndex(int value, int interval) {
  return value / interval;
}

void printArray(int *arr){
  int i;

  for (i = 0; i < N; ++i){
    printf("%d ", arr[i]);
  }

  printf("\n");
}

void merge(int * arr, int start, int mid, int end) {  
   int i = start;
   int j = mid + 1;
   int k;
   int index = start;  
   int * temp = (int *) malloc(sizeof(int)*(end-start + 1));  

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

void randomize(int *arr){
    int i;
    for (i = 0; i < N; ++i){
      arr[i] = ((rand() % (2*N)) - N);  
    }
}

// Driver code
int main(void) {
  int * arr = (int *) malloc(sizeof(int)*N);
  randomize(arr);

  printf("Initial array: ");
  printArray(arr);
  printf("-------------\n");

  BucketSort(arr);
  printf("-------------\n");
  printf("Sorted array: ");
  printArray(arr);
  return 0;
}