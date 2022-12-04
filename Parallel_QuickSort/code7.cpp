/*
sudo apt install mpich
mpic++ file.c++ -o file
mpirun -np 5 ./file
*/

#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;
using namespace std::chrono;

int partition(int array[], int low, int high, int pivot)
{
    int i = low;
    int j = high;
    while (i < j)
    {
        while (array[i] <= pivot && i < high)
            i++;
        while (array[j] > pivot && j > low)
            j--;
        if (i < j)
            swap(array[i], array[j]);
    }
    if (j != low)
        swap(array[low], array[j]);
    return j;
}

void quickSort(int array[], int low, int high)
{
    if (low < high)
    {
        int part = partition(array, low, high, array[low]);
        quickSort(array, low, part - 1);
        quickSort(array, part + 1, high);
    }
}

int main(int argc, char *argv[])
{
    int nproc, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // int n = 16, i;
    // int input[] = {8, 5, 7, 4, 6, 3, 9, 1, 2, 54, 6, 9, 7, 5, 3, 4};

    int n = 10000, i;

    int input[n];

    srand(367);
    for (int i = 0; i < n; i++)
    {

        input[i] = rand() % 10000;
    }

    int start = rank * (n / nproc);
    int end = (rank * (n / nproc)) + (n / nproc);

    int size = (n / nproc);
    if (rank == (nproc - 1))
    {
        end = (rank * (n / nproc)) + (n / nproc) + (n % nproc);
        size = (n / nproc) + (n % nproc);
    }

    auto startT = high_resolution_clock::now(); // start time

    int array[n];
    for (i = 0; i < size; i++)
    {

        array[i] = input[start + i];
    }

    // for (int i = 0; i < size; i++)
    // {
    //     cout << array[i] << " ";
    // }
    cout << endl;

    int pivot = 700;
    int part = partition(array, 0, (size - 1), pivot);
    // for (i=0;i<size;i++)
    //     printf("%d ",array[i]);
    // printf("=>part: %d, rank: %d \n",part,rank);

    int recv_rank = rank + nproc / 2;
    int send_rank = rank - nproc / 2;
    int sr_count = 0;

    if (rank >= (nproc / 2))
    {
        // send to 0 and 1
        MPI_Send(&part, 1, MPI_INT, send_rank, 2, MPI_COMM_WORLD);
        MPI_Send(&array, (part + 1), MPI_INT, send_rank, 1, MPI_COMM_WORLD);

        // recv from 0 and 1
        MPI_Recv(&sr_count, 1, MPI_INT, send_rank, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&array[size], sr_count, MPI_INT, send_rank, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        size = size + sr_count;
        for (i = 0; i < (size - (part + 1)); i++)
        {
            array[i] = array[i + part + 1];
        }
        size = size - part - 1;
    }
    else
    {
        // send to 2 and 3
        int temp1 = size - part - 1;
        MPI_Send(&temp1, 1, MPI_INT, recv_rank, 3, MPI_COMM_WORLD);
        MPI_Send(&array[part + 1], temp1, MPI_INT, recv_rank, 4, MPI_COMM_WORLD);

        // recv from 2 and 3
        MPI_Recv(&sr_count, 1, MPI_INT, recv_rank, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sr_count++;
        MPI_Recv(&array[size], sr_count, MPI_INT, recv_rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        size = size + sr_count;
        for (i = part + 1; i < (part + 1 + sr_count); i++)
            array[i] = array[i + (size - sr_count - part - 1)];
        size = size - (size - sr_count - part - 1);
    }

    int n_nproc = (nproc / 2);

    if (rank < (nproc / 2))
    {
        pivot = 300;
        part = partition(array, 0, (size - 1), pivot);
        /*for (i=0;i<size;i++)
            printf("%d ",array[i]);
        printf("=>part: %d, rank: %d \n",part,rank);*/

        recv_rank = rank + n_nproc / 2;
        send_rank = rank - n_nproc / 2;
        sr_count = 0;

        if (rank >= (n_nproc / 2))
        {
            // send to 0 and 1
            MPI_Send(&part, 1, MPI_INT, send_rank, 2, MPI_COMM_WORLD);
            MPI_Send(&array, (part + 1), MPI_INT, send_rank, 1, MPI_COMM_WORLD);

            // recv from 0 and 1
            MPI_Recv(&sr_count, 1, MPI_INT, send_rank, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&array[size], sr_count, MPI_INT, send_rank, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            size = size + sr_count;
            for (i = 0; i < (size - (part + 1)); i++)
                array[i] = array[i + part + 1];
            size = size - part - 1;
        }
        else
        {
            // send to 2 and 3
            int temp1 = size - part - 1;
            MPI_Send(&temp1, 1, MPI_INT, recv_rank, 3, MPI_COMM_WORLD);
            MPI_Send(&array[part + 1], temp1, MPI_INT, recv_rank, 4, MPI_COMM_WORLD);

            // recv from 2 and 3
            MPI_Recv(&sr_count, 1, MPI_INT, recv_rank, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sr_count++;
            MPI_Recv(&array[size], sr_count, MPI_INT, recv_rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            size = size + sr_count;
            for (i = part + 1; i < (part + 1 + sr_count); i++)
                array[i] = array[i + (size - sr_count - part - 1)];
            size = size - (size - sr_count - part - 1);
        }
    }
    else
    {
        pivot = 900;
        part = partition(array, 0, (size - 1), pivot);
        /*for (i=0;i<size;i++)
            printf("%d ",array[i]);
        printf("=>part: %d, rank: %d \n",part,rank);*/

        recv_rank = rank + n_nproc / 2;
        send_rank = rank - n_nproc / 2;
        sr_count = 0;

        if (rank >= ((n_nproc / 2) + n_nproc))
        {
            // send to 0 and 1
            MPI_Send(&part, 1, MPI_INT, send_rank, 2, MPI_COMM_WORLD);
            MPI_Send(&array, (part + 1), MPI_INT, send_rank, 1, MPI_COMM_WORLD);

            // recv from 0 and 1
            MPI_Recv(&sr_count, 1, MPI_INT, send_rank, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&array[size], sr_count, MPI_INT, send_rank, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            size = size + sr_count;
            for (i = 0; i < (size - (part + 1)); i++)
                array[i] = array[i + part + 1];
            size = size - part - 1;
        }
        else
        {
            // send to 2 and 3
            int temp1 = size - part - 1;
            MPI_Send(&temp1, 1, MPI_INT, recv_rank, 3, MPI_COMM_WORLD);
            MPI_Send(&array[part + 1], temp1, MPI_INT, recv_rank, 4, MPI_COMM_WORLD);

            // recv from 2 and 3
            MPI_Recv(&sr_count, 1, MPI_INT, recv_rank, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sr_count++;
            MPI_Recv(&array[size], sr_count, MPI_INT, recv_rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            size = size + sr_count;
            for (i = part + 1; i < (part + 1 + sr_count); i++)
                array[i] = array[i + (size - sr_count - part - 1)];
            size = size - (size - sr_count - part - 1);
        }
    }

    for (i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("=>part: %d, rank: %d \n", part, rank);

    quickSort(array, 0, size - 1);

    for (i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("=> (sorted)  part: %d, rank: %d \n", part, rank);

    int *size_array, *displ, *output;
    if (rank == 0)
    {
        size_array = new int[nproc];
        output = new int[n];
        displ = new int[nproc];
    }
    MPI_Gather(&size, 1, MPI_INT, size_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        displ[0] = 0;
        for (i = 1; i < nproc; i++)
            displ[i] = displ[i - 1] + size_array[i - 1];
    }

    MPI_Gatherv(&array, size, MPI_INT, output, size_array, displ, MPI_INT, 0, MPI_COMM_WORLD);

    auto stopT = high_resolution_clock::now();

    if (rank == 0)
    {
        printf("The sorted array: ");
        for (i = 0; i < n; i++)
            printf("%d ", output[i]);
        printf("\n");
    }

    auto duration = duration_cast<microseconds>(stopT - startT);
    cout << endl;
    cout << "The elasped time is: " << duration.count() << endl;

    MPI_Finalize();

    return 0;
}
