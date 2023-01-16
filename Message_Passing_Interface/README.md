Part I:
MPI calculate value of PI

![image](https://user-images.githubusercontent.com/54830217/212599228-85edff1c-1bb6-4ab4-9fb0-00d19ae4ddbe.png)


This is an MPI parallel implementation of the calculation for PI using the given formula. First step is to initialize the MPI environment by passing the command line args: argc and argv as the parameters to MPI_Init() function. Then we use the communicator functionality to solicit the values of number of processes (MPI_Comm_size()) and process identifier (MPI_Comm_rank()). Now from the root process we broadcast the problem size parameter (NUMSTEPS) to all other processes (MPI_Bcast()). Now we perform the calculation as done in the sequential version of the code. Once a specific group is done computing its value, it relays the message through the communicator and asks it to reduce it based on the computations performed by other groups by utilizing MPI_Reduce(). This value gets stored in our PI variable. We also store timestamps of start and end of computation and display it along with the calculated value of PI. Finally, we compare the computed PI value to the prestored accurate PI value up to 25 digits and find out the error by subtracting the former from the latter. We terminate the program by calling MPI_Finalize().



Part II:
MPI QuickSort numbers

![image](https://user-images.githubusercontent.com/54830217/212599294-6d4ad53b-9ada-43e9-9dc6-40250e567b07.png)


Part II: This is an MPI parallel implementation for sorting a million numbers in a given data file using QuickSort. The first step is again to initialize the MPI environment by passing the command line args: argc and argv as the parameters to MPI_Init() function. Once more we them use the communicator functionality to solicit the values of number of processes (MPI_Comm_size()) and process identifier (MPI_Comm_rank()). After this, we malloc exact amount of space for 1 million integer entries and read the file data.txt into the allocated space in the heap. Now from the root process we broadcast to all other processes like before (MPI_Bcast()), but this time we use MPI_Scatter() as well to distribute the input data array (part of work to be done) from root process to other processes, so that each process receives NUMSTEPS/numprocs number of elements each. Next, we implement our code for QuickSort. QuickSort uses a divide-and-conquer strategy and is one of the fastest sorting algorithms out there in most cases O(NlogN). We divide the input data set into disjoint subsets. We resolve the problems within the disjoint set by sort them. We then combine the solutions into a solution for the entire data set. This is done by making use of MPI_Send() and MPI_Recv() in order to facilitate exchange of data with neighbors. At the termination of the algorithm, we create a new file and write our sorted array into it. We also store timestamps of start and end of computation and display it before we terminate the program by calling MPI_Finalize().
