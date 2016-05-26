#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "config.h"

#define TAG_SEND 1
#define TAG_FINISHED 2

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

int isSorted(const int *a, int size) {
    for (int i = 0; i < size - 1; i++) {
        if (a[i] > a[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Function to print an array
void printArray(const int arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int* merge(const int arr[], int count, int item_size){
    int groups = count/item_size;
    int* out = malloc(sizeof(int) * count);
    int ptr[groups];
    int index = 0;
    memset(ptr, 0, sizeof(int) * groups);

    while(1){
        int target_group = -1;
        int target_min = -1;
        for(int i = 0; i < groups; i++){
            if(ptr[i] >= item_size){
                continue;
            }
            int cur_item_id = (i * item_size) + ptr[i];
            if(target_group == -1 || arr[cur_item_id] < target_min){
                target_group = i;
                target_min = arr[cur_item_id];
            }
        }
        if(target_group == -1){
            break;
        }
        ptr[target_group]++;
        out[index++] = target_min;
    }

    assert(index == count);

    return out;
}

int main(int argc, const char *argv[]) {
    double t1, t2;
    MPI_Init(&argc, (char ***) &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if((size & (size-1)) != 0){
        printf("Process number must be power of 2!");
        exit(1);
    }

    int item_size = N / size;

    if(rank == 0){
        t1 = MPI_Wtime();
        int32_t A[N];
        for (int i = N - 1; i >= 0; i--) {
            A[N - 1 - i] = i;
        }

        if (isSorted(A, N)) {
            printf("Array is sorted\n");
        } else {
            printf("Array is NOT sorted\n");
        }

        printf("Sending to %d processes, %d each.\n", size, item_size);

        for(int i = 1; i < size; i++) {
            MPI_Send(&A[i*item_size], item_size, MPI_INT32_T, i, TAG_SEND, MPI_COMM_WORLD);
        }

        printf("Sent done, doing sort myself\n");
        bubbleSort(A, item_size);

        for(int i = 1; i < size; i++) {
            printf("Waiting for %d to finish...\n", i);
            MPI_Recv(&A[i*item_size], item_size, MPI_INT32_T, i, TAG_FINISHED, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        int* A2 = merge(A, N, item_size);

        printArray(&A2[N - 10], 10);
        if (isSorted(A2, N)) {
            printf("Array is sorted\n");
        }else {
            printf("Array is NOT sorted\n");
        }

        free(A2);

        t2 = MPI_Wtime();
        printf("Elapsed time MPI_Wtime is %f\n", t2 - t1);
    }else{
        int32_t A[item_size];
        t1 = MPI_Wtime();

        MPI_Recv(A, item_size, MPI_INT32_T, 0, TAG_SEND, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("%d received\n", rank);
        bubbleSort(A, item_size);
        t2 = MPI_Wtime();

        printf("%d done in %f\n", rank, t2 - t1);
        MPI_Send(&A, item_size, MPI_INT32_T, 0, TAG_FINISHED, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
