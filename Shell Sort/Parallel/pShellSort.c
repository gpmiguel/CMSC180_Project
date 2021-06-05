#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/sysinfo.h>
#define N 2000000


int * arr;

typedef struct ARG{
	int iteration;
	int n;
	int *arr;
}args;

void swap(int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

int get_cpu_count(){
    return get_nprocs_conf();
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

//void * shellsort(int * a, int n)
void * parallelShellSort(void * argument){
	args * temp = (args *) argument;

	int hk, d, i, j;

	for(hk = (temp->n)/2 ; hk >= 1; hk /= 2)
		for(i = hk ; i < (temp->n) ; i++)
			for(j = i ; j > hk-1 ; j -= hk)
				if((temp->arr[j-hk]) > (temp->arr[j]))
					swap(&(temp->arr[j-hk]), &(temp->arr[j]));
				else
					break;
}

int main(){
	arr = (int *) malloc(sizeof(int) * N);
	int partitions = get_cpu_count();
	int i, j;

	args * arguments = (args *) malloc(sizeof(args) * partitions);
	pthread_t *tid = (pthread_t *) malloc(partitions * sizeof(pthread_t));

	randomize(arr);

	int **temp = (int **) malloc(sizeof(int *) * partitions);
	for(i = 0; i < partitions; i++){
		temp[i] = (int *) malloc(sizeof(int) * (N/partitions));
	}

	int start = 0;
	int iterator = 0; 
	int end = (N/partitions);

	struct timespec t1, t2;
	long sec, nano;
	double elapsed;

	clock_gettime(CLOCK_REALTIME, &t1);


	while(iterator != partitions){
		int incrementer = 0;
	  	arguments[iterator].iteration = 0;
	  	arguments[iterator].n = (N/partitions);
	  	arguments[iterator].arr = (int *) malloc(sizeof(int) * (N/partitions));

	  	for(j = start; j < end; j++){
	  		temp[iterator][incrementer++] = arr[j];
	  	}

	  	arguments[iterator].arr = temp[iterator];

	  	cpu_set_t cpuset; 

		  //the CPU we whant to use
	    int cpu = iterator;

	    CPU_ZERO(&cpuset);       //clears the cpuset
	    CPU_SET(cpu, &cpuset);	// sets CPU

	    pthread_create(&(tid[iterator]), NULL, parallelShellSort, (void *) &arguments[iterator]);
	    pthread_setaffinity_np(tid[iterator], sizeof(cpuset), &cpuset);

	    start += (N/partitions);
	    end += (N/partitions);
	    iterator++;
    }

    for (int i = 0; i < partitions; i++){
		pthread_join(tid[i], NULL); // to wait for thread execution to finish for each node before proceeding
	}

	for (i = 0; i < N; i++){
	  	int smallest = (N+1);
	  	int index;
	  	for(j = 0; j < partitions; j++){
	  		if (arguments[j].arr[arguments[j].iteration] < smallest){
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

	printf("Time elapsed for Parallel Shell Sort at N = %d is %f seconds\n\n", N, elapsed);
		
}