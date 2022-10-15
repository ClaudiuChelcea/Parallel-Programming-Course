#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysinfoapi.h>

#define DEBUG 0
#define EX6 1
#define REPEAT_TIMES_TO_CHECK_SCALABILITY 1

// Starting array    
int* arr;
int array_size;

// Getting the nr of cores
long cores;

// Add the value "100" to each element of the received array
void *add_fct(void *arg) {
    // The indexes where to extract the array from for each thread
    long ID = *(long *)arg;
    int start = ID  * (double) array_size / cores;
    int end = min((ID + 1) * (double) array_size / cores, array_size);

    // Modify array
    #if DEBUG
    printf("Thread-ul %d, modific array de la %d la %d!\n", ID, start, end);
    #endif

    #if EX6
    for (int repeat = 0; repeat < REPEAT_TIMES_TO_CHECK_SCALABILITY; ++repeat) {
        for (long i = start; i < end; i++)
        {
            arr[i] += 100;
        }
    }
    #endif

  	pthread_exit(NULL);
}

// Print the array
void print_array(int arr[], int array_size)
{
    for (int i = 0; i < array_size; ++i) {
        printf("%d ", arr[i]);
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    // Get nr of cores
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    cores = sysinfo.dwNumberOfProcessors; // Windows adaptation of the function, since sysconf is only for Linux
    
    #if DEBUG
    printf("Cores %d\n", cores);
    #endif

    // Read and allocate array
    array_size = atoi(argv[1]);
    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

    // Print the array
    print_array(arr, array_size);

    // Create threads
    pthread_t threads[cores];
    int arguments[cores];
    for (int id = 0; id < cores; id++) {

        // Create the thread function for that array
        arguments[id] = id;
        int r = pthread_create(&threads[id], NULL, add_fct, &arguments[id]);
        
        #if DEBUG
        // printf("Thread id created: %d\n", thread_id);
        #endif

        if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
    }

    // Join the threads again
    for (int id = 0; id < cores; ++id) {
		int r = pthread_join(threads[id], NULL);

        // Check if thread was waited for correctly
		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

    // Print the array
    print_array(arr, array_size);

  	pthread_exit(NULL);
}
