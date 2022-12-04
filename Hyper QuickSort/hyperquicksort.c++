/*
sudo apt install mpich
mpic++ file.c++ -o file
mpirun -np 4 ./file
*/


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>

using namespace std;

// convert decimal x to binary array bin
int* binary(int x, int dim){
    
    int* bin = (int*)malloc(dim*sizeof(int));
    for(int i=0; i<dim; i++)
        bin[i]=0;
    
    int i = dim-1;                                  // start with least signficant position
    while (x > 0){
        bin[i] = x % 2;
        x /= 2;
        i--;                                        // go to next most significant position
    }
    
    return bin;
}

// check which processes generate pivot
bool check_id(int* bin_process_id, int step, int dim){
    for (int i=dim-1; i>=step; i--)
        if (bin_process_id[i] == 1)
            return false;
    return true;
}

// partition array around the pivot
int hyper_partition(vector<int> &array, int low, int high, int pivot){
    
    int i = low - 1;	
    for(int j=low; j<high; j++){
		if(array[j]<=pivot){
			i++;
			int temp = array[i];
			array[i] = array[j];
			array[j] = temp;
		}
	}

	int temp = array[i + 1];
	array[i+1] = array[high];
	array[high] = temp;
	
    return (array[i+1] > pivot) ? i : (i+1);
}

// partition array for serial quicksort
int partition (vector<int> &array, int low, int high){
	
    int pivot = array[high];
	int i = (low-1);
	for (int j=low; j<=high-1; j++){
		if (array[j] <= pivot){
			i++;
			int temp = array[i];
			array[i] = array[j];
			array[j] = temp;
		}
	}

    int temp = array[i+1];
    array[i+1] = array[high];
    array[high] = temp;

	return (i + 1);
}

// serial quicksort
void quicksort(vector<int> &array, int low, int high){
	if (low<high){
		int pi = partition(array, low, high);
		quicksort(array, low, pi-1);
		quicksort(array, pi+1, high);
	}
}

// main function
int main(int argc, char *argv[]){
    
    int size = atoi(argv[1]),                       // size of array
        arr[size],                                  // declare array
        sorted_array[size],                         // declare sorted array
        num_process,                                // number of processes
        process_id,                                 // process id
        sub_num_process,                            // number of processes in sub_hypercubes
        sub_process_id,                             // process id in sub_hypercubes
        color,                                      // color of sub_hypercubes
        pivot,                                      // pivot
        position,                                   // position of pivot
        partner_process,                            // partner process in sub_hypercube
        send_array_size,                            // size of array to be sent
        receive_array_size;                         // size of array to be received
   
    double  start_time,                             // start time
            elapsed_time;                           // elapsed time

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    start_time = MPI_Wtime( );

    int dim = log2(num_process),                    // dimension of hypercube
        chunk_size = size/num_process,              // size of of chuck array of each process
        chunk_array[chunk_size];                    // chunk array of each process
    
    int* bin_process_id = binary(process_id, dim);  // binary process id

    if(process_id==0)                               // process 0
        for(int i=0; i<size; i++)                   // generates random array
            arr[i] = rand()%RAND_MAX;

    MPI_Scatter(arr, chunk_size, MPI_INT, chunk_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    vector<int> new_chunk_array;                    // chunk array (as vector)
    for(int i=0; i<chunk_size; i++)
        new_chunk_array.push_back(chunk_array[i]);
        
    for(int i=0; i<dim; i++){

        color = process_id/(num_process/(i+1));     // define color based on (dim-1)-hypercube partition
        
        MPI_Comm SUB_HYPERCUBE;
        
        MPI_Comm_split(MPI_COMM_WORLD, color, process_id, &SUB_HYPERCUBE);
        MPI_Comm_rank(SUB_HYPERCUBE, &sub_process_id);
        MPI_Comm_size(SUB_HYPERCUBE, &sub_num_process);

        if(check_id(bin_process_id, i, dim))
            pivot = new_chunk_array[chunk_size-1];      // pick last element of array as pivot

        MPI_Bcast(&pivot, 1, MPI_INT, 0, SUB_HYPERCUBE);

        vector<int> send_array;                     // array to be sent
        vector<int> keep_array;                     // array to be kept

        position = hyper_partition(new_chunk_array, 0, chunk_size-1, pivot);    // partition array around pivot and return pivot position

        if(sub_process_id/(sub_num_process/2)==1){          // if process in upper half of sub_hypercube          
            for(int i=0; i<=position; i++)                  // send first elements of array
                send_array.push_back(new_chunk_array[i]);   // up to the pivot position
            for(int i=position+1; i<chunk_size; i++)        // but
                keep_array.push_back(new_chunk_array[i]);   // keep the others
        }
        else{                                               // otherwise 
            for(int i=position+1; i<chunk_size; i++)        // send the remaining elements of array
                send_array.push_back(new_chunk_array[i]);   // including the pivot
            for(int i=0; i<=position; i++)                  // but
                keep_array.push_back(new_chunk_array[i]);   // keep the others
        }

        if(sub_process_id/(sub_num_process/2)==0)
            partner_process = sub_process_id+sub_num_process/2; // determine partner process in the upper half of the sub_hypercube
        else
            partner_process = sub_process_id-sub_num_process/2; // determine partner process in the lower half of the sub_hypercube

        send_array_size = send_array.size();                    // size of array to be sent (in int format)
        
        MPI_Send(&send_array_size, 1, MPI_INT, partner_process, dim, SUB_HYPERCUBE);                        // send size of array to be sent

        MPI_Recv(&receive_array_size, 1, MPI_INT, partner_process, dim, SUB_HYPERCUBE, MPI_STATUS_IGNORE);  // receive size of array to be received

        int receive_array[receive_array_size];                  // array to be received
        int new_send_array[send_array_size];                    // array to be sent (in array format)
        for(int i=0; i<send_array_size; i++)
            new_send_array[i] = send_array[i];

        MPI_Send(&new_send_array, send_array_size, MPI_INT, partner_process, dim, SUB_HYPERCUBE);                           // send array

        MPI_Recv(receive_array, receive_array_size, MPI_INT, partner_process, dim, SUB_HYPERCUBE, MPI_STATUS_IGNORE);       // receive array

        new_chunk_array.clear();                            // clear new_chunk_array array

        for(int i=0; i<keep_array.size(); i++)              // repopulate first positions of new_chunk_array with keep_array
            new_chunk_array.push_back(keep_array[i]);
        for(int i=0; i<receive_array_size; i++)             // repopulate remaining positions with receive_array
            new_chunk_array.push_back(receive_array[i]);
        
        chunk_size = new_chunk_array.size();                // redefine chunk_size

        MPI_Comm_free(&SUB_HYPERCUBE);
    }

    quicksort(new_chunk_array, 0, chunk_size-1);

    int sorted_chunk_array[chunk_size],                     // array for new_chunk_array (in array format)
        count[num_process],                                 // array with size of each chunk_array
        displacement[num_process];                          // array with placement index of each chunk_array
    
    for(int i=0; i<chunk_size; i++)
        sorted_chunk_array[i] = new_chunk_array[i];         // sorted new_chunk_array (in array format)

    if(process_id==0){
        count[0] = chunk_size;
        displacement[0] = 0;

        for(int i=1; i<num_process; i++)
            MPI_Recv(&count[i], 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   // receive the size of each processor's chunk_array on processor 0
        
        int temp = 0;
        for(int i=1; i<num_process; i++){
            temp += count[i-1];
            displacement[i] = temp;                 // increment the placement index of each chunk_array
        }
    }
    else
        MPI_Send(&chunk_size, 1, MPI_INT, 0, process_id, MPI_COMM_WORLD);               // send the size of chunk_array to processor 0

    MPI_Gatherv(sorted_chunk_array, chunk_size, MPI_INT, sorted_array, count, displacement, MPI_INT, 0, MPI_COMM_WORLD);

    //if(process_id==0)
    //    MPI_Gatherv(sorted_chunk_array, chunk_size, MPI_INT, sorted_array, count, displacement, MPI_INT, 0, MPI_COMM_WORLD);
    //else
    //    MPI_Gatherv(sorted_chunk_array, chunk_size, MPI_INT, NULL, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);

    free(bin_process_id);

    elapsed_time = MPI_Wtime ( ) - start_time;      // calculate elapsed time

    MPI_Finalize();

    if(process_id==0){
        printf("\n");
        printf("This is the final sorted array:\n");
        for(int i=0; i<size; i++)
            printf("%d ", sorted_array[i]);
        printf("\n \n");
        printf("Time taken: %fs\n", elapsed_time);
    }

    return 0;
}