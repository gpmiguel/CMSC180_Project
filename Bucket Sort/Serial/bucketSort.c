// Bucket sort in C

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int N;

#define BUCKETS 4  // Number of buckets

typedef struct Bucket {
  int upperBound;
  int size;
  int *bucket;
}buckets;

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

// 'Bucketing' function
void BucketSort(int * arr) {
  int i, j, interval, index = 0;
  buckets bucketArray[BUCKETS];

  // Create buckets
  interval = (2*N) / BUCKETS;

  for (i = 0; i < BUCKETS; ++i){
      bucketArray[i].upperBound = (-N) + (i+1)*interval;
      bucketArray[i].size = 0;
      bucketArray[i].bucket = (int *) malloc(sizeof(int) * N);

  }

  for (i = 0; i < N; ++i){
      for (j = 0; j < BUCKETS; ++j){
          if (arr[i] <= bucketArray[j].upperBound){
              bucketArray[j].bucket[bucketArray[j].size] = arr[i];
              bucketArray[j].size += 1;
              break;
          }
      }
  }

  for (i = 0; i < BUCKETS; ++i) {
      mergeSort(bucketArray[i].bucket, 0, (bucketArray[i].size)-1);
  }

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

void randomize(int *arr){
    int i;
    for (i = 0; i < N; ++i){
      arr[i] = ((rand() % (2*N)) - N);  
    }
}

// Driver code
int main(void) {

  FILE *fp;

  fp = fopen("bucketSort.csv", "w+");
  fprintf(fp, "N,seconds\n");

  for (N = 1000; N != 501000 ; N = N + 1000)
  {
    int * arr = (int *) malloc(sizeof(int)*N);
    randomize(arr);

    struct timespec t1, t2;
    long sec, nano;
    double elapsed;

    clock_gettime(CLOCK_REALTIME, &t1);

    BucketSort(arr);
    
    clock_gettime(CLOCK_REALTIME, &t2);

    sec = t2.tv_sec - t1.tv_sec;
    nano = t2.tv_nsec - t1.tv_nsec;
    elapsed = sec + nano*1e-9;

    printf("Time elapsed for Bucket Sort at N = %d is %f seconds\n\n", N, elapsed);
    fprintf(fp, "%d,%f\n", N, elapsed);

    free(arr);
  }

  

  return 0;
}