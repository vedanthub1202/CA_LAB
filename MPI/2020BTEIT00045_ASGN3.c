/*
sudo apt install mpich
mpicc file.c -o file
mpirun -np 5 ./file
*/

#include<stdio.h>
#include<mpi.h>

int main(int argc, char** argv)
{
	int my_rank;
	int array[10] = {1,1,1,1};
	int rate[4] = {10,20,30,40};

	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank==0){
		MPI_Recv( &array, 4, MPI_INT, 1, 100, MPI_COMM_WORLD, &status);
		printf("The data is received at headoffice from department 1 is\n");

		for(int i = 0;i<4;i++)
			printf("%d ",array[i]);
		printf("\n\n");

		array[0] = array[0] + (array[0]*rate[0])/100;
         	array[1] = array[1] + (array[1]*rate[1])/100;
         	array[2] = array[2] + (array[2]*rate[2])/100;
         	array[3] = array[3] + (array[3]*rate[3])/100;

         	MPI_Send( &array, 4, MPI_INT, 1, 10, MPI_COMM_WORLD);
		MPI_Recv( &array, 4, MPI_INT, 2, 200, MPI_COMM_WORLD, &status);
		printf("The data is received at headoffice from department 2 is\n");

		for(int i = 0;i<4;i++)
			printf("%d ",array[i]);
		printf("\n\n");

		array[0] = array[0] + (array[0]*rate[0])/100;
         	array[1] = array[1] + (array[1]*rate[1])/100;
         	array[2] = array[2] + (array[2]*rate[2])/100;
         	array[3] = array[3] + (array[3]*rate[3])/100;

         	MPI_Send( &array, 4, MPI_INT, 2, 20, MPI_COMM_WORLD);
		MPI_Recv( &array, 4, MPI_INT, 3, 300, MPI_COMM_WORLD, &status);
		printf("The data is received at headoffice from department 3 is\n");

		for(int i = 0;i<4;i++)
			printf("%d ",array[i]);
		printf("\n\n");

		array[0] = array[0] + (array[0]*rate[0])/100;
         	array[1] = array[1] + (array[1]*rate[1])/100;
         	array[2] = array[2] + (array[2]*rate[2])/100;
         	array[3] = array[3] + (array[3]*rate[3])/100;
         	MPI_Send( &array, 4, MPI_INT, 3, 30, MPI_COMM_WORLD);
		
	}
	else if(my_rank == 1)
	{
		if (!array) // error checking
         	{
          		MPI_Abort (MPI_COMM_WORLD,1);
         	}

         	array[0] = 1;
         	array[1] = 2;
         	array[2] = 3;
         	array[3] = 4;
         	printf("The item cost of department 1\n");

         	for(int i = 0;i<4;i++)
			printf("%d ",array[i]);	
		printf("\n\n");

		MPI_Send( &array, 4, MPI_INT, 0, 100, MPI_COMM_WORLD);
		printf("The data is sent to the head office from the server number %d\n \n",my_rank);
		MPI_Recv( &array, 4, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
		printf("Successfully Received the data at department number 1 after processing\n");

		for(int i = 0;i<4;i++)
			printf("%d ",array[i]);
		printf("\n\n");
		
	}
	else if(my_rank == 2)
	{
		if (!array) // error checking
         	{
          		MPI_Abort (MPI_COMM_WORLD,1);
         	}
         	array[0] = 2;
         	array[1] = 4;
         	array[2] = 6;
         	array[3] = 8;
         	printf("The item cost of department 2\n");

         	for(int i = 0;i<4;i++)
			printf("%d ",array[i]);	
		printf("\n\n");

		MPI_Send( &array, 4, MPI_INT, 0, 200, MPI_COMM_WORLD);
		printf("The data is sent to the head office from the server number %d\n",my_rank);
		MPI_Recv( &array, 4, MPI_INT, 0, 20, MPI_COMM_WORLD, &status);
		printf("Successfully Received the data at department number 2 after processing\n");

		for(int i = 0;i<4;i++)
			printf("%d ",array[i]);
		printf("\n\n");
	}
	else if(my_rank == 3)
	{
		if (!array) // error checking
         	{
          		MPI_Abort (MPI_COMM_WORLD,1);
         	}
         	array[0] = 1;
         	array[1] = 3;
         	array[2] = 5;
         	array[3] = 7;
         	printf("The item cost of department 3\n");

         	for(int i = 0;i<4;i++)
			printf("%d ",array[i]);	
		printf("\n\n");

		MPI_Send( &array, 4, MPI_INT, 0, 300, MPI_COMM_WORLD);
		printf("The data is sent to the head office from the server number %d\n",my_rank);
		MPI_Recv( &array, 4, MPI_INT, 0, 30, MPI_COMM_WORLD, &status);
		printf("Successfully Received the data at department number 3 after processing\n");

		for(int i = 0;i<4;i++)
			printf("%d ",array[i]);
		printf("\n\n");
	}
	MPI_Finalize();
	return 0;
}