// Bucket sort in C

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <time.h>
#include <sys/sysinfo.h>

int N;

int *arr;
int BUCKETS;

typedef struct Bucket {
  int lowerBound;
  int upperBound;
  int size;
  int * bucket;
}buckets;

typedef struct ARG {
  int iteration;
  int end;
  int *arr;
}args;

// Sorting function

int getBucketIndex(int value, int interval){
  return value / interval;
}

void printArray(int *arr){
  int i;

  for (i = 0; i < N; ++i){
    printf("%d ", arr[i]);
  }

  printf("\n");
}

int get_cpu_count(){
    return get_nprocs_conf();
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

void * BucketSort(void * argument) {
  args * temp = (args *) argument;
  BUCKETS = get_cpu_count();

  int i, j, interval, index = 0;
  buckets bucketArray[BUCKETS];

  // Create buckets
  interval = (2*N) / BUCKETS;

  for (i = 0; i < BUCKETS; ++i){
      bucketArray[i].lowerBound = (-N) + (i)*interval;
      bucketArray[i].upperBound = (-N) + (i+1)*interval;
      bucketArray[i].bucket= (int *) malloc(sizeof(int)*N);
      bucketArray[i].size = 0;
  }

  // Fill the buckets with respective elements
  for (i = 0; i < (N/BUCKETS); ++i){
      for (j = 0; j < BUCKETS; ++j){
          if (temp->arr[i] < bucketArray[j].upperBound && temp->arr[i] >= bucketArray[j].lowerBound){
              bucketArray[j].bucket[bucketArray[j].size] = temp->arr[i];
              bucketArray[j].size += 1;
              break;
          }
      }
  }

  // Sort the elements of each bucket
  for (i = 0; i < BUCKETS; ++i) {
      mergeSort(bucketArray[i].bucket, 0, (bucketArray[i].size)-1);
  }

  // Put sorted elements on arr
  for (i = 0; i < BUCKETS; ++i) {
      for (j = 0; j < bucketArray[i].size; ++j){
          temp->arr[index++] = bucketArray[i].bucket[j];
      }
  }
} 

void randomize(int *arr){
    int i;
    for (i = 0; i < N; ++i){
      arr[i] = (rand() % (N)) - (N/2);  
    }
}

// Driver code
int main(void) {

  FILE *fp;

  fp = fopen("pBucketSort.csv", "w+");
  fprintf(fp, "N,seconds\n");

  for (N = 1000; N != 501000; N = N + 1000)
  {
      arr = (int *) malloc(sizeof(int)*N);
      randomize(arr);

      // printArray(arr);
      //printf("\n");

      int partitions = get_cpu_count();
      int start = 0;
      int iterator = 0;
      int end = N/partitions;
      int i, j;

      args * arguments = (args *) malloc(sizeof(args)*partitions);
      pthread_t * tid = (pthread_t *) malloc(sizeof(pthread_t)*partitions);

      struct timespec t1, t2;
      long sec, nano;
      double elapsed;

      clock_gettime(CLOCK_REALTIME, &t1);

      while(iterator != partitions){
        arguments[iterator].iteration = 0;
        arguments[iterator].end = end;
        arguments[iterator].arr = (int *) malloc(sizeof(int) * (N/partitions));

        for(j = 0; j < end; j++) arguments[iterator].arr[j] = arr[j+(iterator*(N/partitions))];

        cpu_set_t cpuset; 

       //the CPU we whant to use
        int cpu = iterator;

        CPU_ZERO(&cpuset);       //clears the cpuset
        CPU_SET(cpu, &cpuset); // sets CPU

        pthread_create(&(tid[iterator]), NULL, BucketSort, (void *) &arguments[iterator]);
        pthread_setaffinity_np(tid[iterator], sizeof(cpuset), &cpuset);

        iterator++;
      }

      for (int i = 0; i < partitions; i++){
        pthread_join(tid[i], NULL); // to wait for thread execution to finish for each node before proceeding
      }

      for (i = 0; i < N; i++){
        int smallest = (N+1);
        int index;
        for(j = 0; j < partitions; j++){
           if (arguments[j].arr[arguments[j].iteration] < smallest && arguments[j].iteration != arguments[j].end){
              smallest = arguments[j].arr[arguments[j].iteration];
              index = j;
           }
        }

        arr[i] = smallest;
        arguments[index].iteration += 1;
      }

      clock_gettime(CLOCK_REALTIME, &t2);

      sec = t2.tv_sec - t1.tv_sec;
      nano = t2.tv_nsec - t1.tv_nsec;
      elapsed = sec + nano*1e-9;

      printf("Time elapsed for Parallel Bucket Sort at N = %d is %f seconds\n\n", N, elapsed);
      fprintf(fp, "%d,%f\n", N, elapsed);

      //printArray(arr);

      free(arr);
      free(arguments);
      free(tid);
  }

  return 0;
}