#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysinfoapi.h>

void *f(void *arg) {
  	long id = *(long*)arg;
	for(int i = 0; i <100; ++i) {
  		printf("Hello World din thread-ul %ld!\n", id);
	}
  	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	// Getting the nr of cores
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	long cores = sysinfo.dwNumberOfProcessors; // Windows adaptation of the function, since sysconf is only for Linux
	// long cores = sysconf(_SC_NPROCESSORS_CONF);
	pthread_t threads[cores];
  	int r;
  	long id;
  	void *status;
	long ids[cores];
		
	

  	for (id = 0; id < cores; id++) {
		ids[id] = id; 
		r = pthread_create(&threads[id], NULL, f, &ids[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	for (id = 0; id < cores; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	pthread_exit(NULL);
}
