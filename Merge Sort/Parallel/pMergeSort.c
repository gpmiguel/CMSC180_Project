#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <time.h>
#include <sys/sysinfo.h>
#define N 1000000

// global variables

int *arr;

typedef struct ARG{
   int iteration;
   int end;
   int * arr;
}args;

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

void mergeSort(int * arr, int start, int end)  {  
   int mid;  
   if(start < end) {  
      mid = (start+end)/2;  
      mergeSort(arr,start,mid);  
      mergeSort(arr,mid+1,end);  
      merge(arr,start,mid,end);  
   }  
} 

int get_cpu_count(){
    return get_nprocs_conf();
}

void * mergeHelper(void * argument){
   args * temp = (args *) argument;

   mergeSort(temp->arr, 0, (temp->end)-1);
}

int main() { 
   arr = (int *) malloc(sizeof(int)*N);
   randomize(arr);

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

      pthread_create(&(tid[iterator]), NULL, mergeHelper, (void *) &arguments[iterator]);
      pthread_setaffinity_np(tid[iterator], sizeof(cpuset), &cpuset);

      iterator++;
   }

   for (int i = 0; i < partitions; i++){
      pthread_join(tid[i], NULL); // to wait for thread execution to finish for each node before proceeding
   }

   free(tid);

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

   printf("Time elapsed for Parallel Merge Sort at N = %d is %f seconds\n\n", N, elapsed);
   

}
