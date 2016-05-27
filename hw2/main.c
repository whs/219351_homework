#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define N 100000

typedef struct arg{
    int *array;
    int start;
    int size;
} arg;

void swap(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(int arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

void merge(int arr[], int groups, int n){
    int groupSize = n/groups;
    int left = 0;
    int right = groupSize;
    int leftEnd = groupSize;

    while(left < leftEnd && right < n){
        if(arr[left] > arr[right]){
            int tmp = arr[right];
            // shift right
            memmove(&arr[left + 1], &arr[left], (right-left) * sizeof(int));
            arr[left] = tmp;
            leftEnd++;
            right++;
        }
        left++;
    }
}

void parallelBubbleSortThread(arg *args){
//    printf("thread work start %d size %d\n", args->start, args->size);
    bubbleSort(&(args->array[args->start]), args->size);
    free(args);
}

void parallelBubbleSort(int arr[], int n) {
    const int threadCount = 2;
    int workSize = n/threadCount;

    pthread_t threads[threadCount-1];
    for(int i = 0; i < threadCount; i++){
        if(i == threadCount - 1){
            bubbleSort(&(arr[i*workSize]), n - (i*workSize));
        }else {
            arg *args = (arg *) malloc(sizeof(arg));
            args->array = arr;
            args->start = i * workSize;
            args->size = workSize;
            pthread_create(&threads[i], NULL, (void *) &parallelBubbleSortThread, (void *) args);
        }
    }
    for(int i = 0; i < threadCount-1; i++){
        pthread_join(threads[i], NULL);
    }
    merge(arr, threadCount, n);
}

void printArray(int arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void benchmark(int threaded){
    int i, n;
    int *A;
    clock_t start, end;
    double elapsed_time;

    A = (int *) malloc(sizeof(int) * N);
    if (A == NULL) {
        printf("Fail to malloc\n");
        exit(0);
    }
    for (i = N - 1; i >= 0; i--) {
        A[N - 1 - i] = i;
    }

    // print the last ten elements
    printArray(&A[N - 10], 10);
    start = clock();
    if(threaded){
        parallelBubbleSort(A, N);
    }else{
        bubbleSort(A, N);
    }
    end = clock();

    // print the last ten elements
    printArray(&A[N - 10], 10);
//    printArray(A, N);
    elapsed_time = (end - start) / (double) CLOCKS_PER_SEC;
    for(int i = 0; i < N; i++){
        if(A[i] != i){
            printf("Assertion fail at %d != %d\n", i, A[i]);
        }
    }
    printf("elapsed time = %lf\n", elapsed_time);
}

int main() {
    printf("Benchmarking threaded impl:\n");
    benchmark(1);

    printf("\nBenchmarking non-threaded impl:\n");
    benchmark(0);

    return 0;
}