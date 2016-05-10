#include <cassert>
#include <iostream>
#include <cstring>
#include <mpi.h>
#include "config.h"

#define TAG_SEND 1
#define TAG_FINISHED 2

void bubbleSort(int *array, const int size) {
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (array[j] > array[j + 1])
                std::swap(array[j], array[j + 1]);
}

int isSorted(const int *array, const int size) {
    for (int i = 0; i < size - 1; i++) {
        if (array[i] > array[i + 1]) {
            return false;
        }
    }
    return true;
}

void printArray(const int *array, const int from, const int to) {
    for (int i = from; i < to; i++){
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int *merge(int *arr, const int size, const int item_size){
    const int groups = (const int) (size / item_size);
    int *out = new int[size];
    int ptr[groups];
    int index = 0;
    std::memset(ptr, 0, sizeof(int) * groups);

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

    assert(index == size);

    return out;
}

int main(int argc, const char *argv[]) {
    double t1, t2;
    MPI::Init(argc, (char **&) argv);

    const int rank = MPI::COMM_WORLD.Get_rank();
    const int size = MPI::COMM_WORLD.Get_size();

    if((size & (size-1)) != 0){
        std::cout << "Process number must be power of 2!" << std::endl;
        exit(1);
    }

    const int item_size = N / size;

    if(rank == 0){
        t1 = MPI::Wtime();
        int32_t A[N];
        for (int i = N - 1; i >= 0; i--) {
            A[N - 1 - i] = i;
        }

        if (isSorted(A, N)) {
            std::cout << "Array is sorted" << std::endl;
        } else {
            std::cout << "Array is NOT sorted" << std::endl;
        }

        std::cout << "Sending to " << size << " processes, " << item_size << " each." << std::endl;

        for(int i = 1; i < size; i++) {
            MPI::COMM_WORLD.Send(&A[i*item_size], item_size, MPI_INT32_T, i, TAG_SEND);
        }

        std::cout << "Sent done, doing sort myself" << std::endl;
        bubbleSort(A, item_size);

        for(int i = 1; i < size; i++) {
            std::cout << "Waiting for " << i << " to finish..." << std::endl;
            MPI::COMM_WORLD.Recv(&A[i*item_size], item_size, MPI_INT32_T, i, TAG_FINISHED);
        }
        int32_t *out = merge(A, N, item_size);

        printArray(out, N - 10, 10);
        if (isSorted(out, N)) {
            std::cout << "Array is sorted" << std::endl;
        } else {
            std::cout << "Array is NOT sorted" << std::endl;
        }

        t2 = MPI::Wtime();
        std::cout << "Elapsed time MPI_Wtime is " << (t2 - t1) << std::endl;
        delete[] out;
    }else{
        int32_t A[item_size];
        t1 = MPI::Wtime();

        MPI::COMM_WORLD.Recv(A, item_size, MPI_INT32_T, 0, TAG_SEND);
        std::cout << rank << " received" << std::endl;
        bubbleSort(A, item_size);
        t2 = MPI::Wtime();

        std::cout << rank << " done in " << (t2-t1) << std::endl;
        MPI::COMM_WORLD.Send(A, item_size, MPI_INT32_T, 0, TAG_FINISHED);
    }
    MPI::Finalize();
    return 0;
}
