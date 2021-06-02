/*
Run the following algorithms and time them:
1. Insertion sort
2. Shell sort
3. Merge sort
4. Quick sort (built-in)

Run for x, 2x, 3x, and 4x inputs (where x is your initial array size n).
Run for ascending, descending, random input
Replicate each run three (3) times. Record the average.
Graph the results.
Compare the actual running times to the supposed theoretical running times.
Do they match? Analyze your results.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <malloc.h>

// Initialization of data
void init(int * a, int n){
   int i;

   for(i=0; i<n; i++)
      a[i] = (rand() % (2*n)) - n; 
}

// Merge sort
void merge (int * a, int lower, int mid, int upper){
   int *temp,i,j,k;
   temp=(int *)malloc((upper-lower+1)*sizeof(int));
   for(i=0,j=lower,k=mid+1; j<=mid || k<=upper; i++)
      temp[i]=((j <= mid) && (k > upper || a[j] <= a[k]))?a[j++]:a[k++];
   for(i=0,j=lower;j<=upper; i++, j++)
   a[j]=temp[i];
   free(temp);
}

void msort(int * a, int lower, int upper){
   int mid;
   if (upper-lower>0) {
      mid=(lower+upper)/2;
      msort(a, lower, mid);
      msort(a, mid+1, upper);
      merge(a, lower, mid, upper);
   }
}

int main(){
   int n = 20;
   int *a;
   int i;

   a = (int*)malloc(sizeof(int)*n);
   init(a,n);

   for(i=0;i<n;i++) printf("%d ", a[i]);
   printf("\n");

   msort(a, 0, n);

   for(i=0;i<n;i++) printf("%d ", a[i]);
   printf("\n");
   
   free(a);

   return 0;
}
