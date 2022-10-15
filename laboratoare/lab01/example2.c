#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define cores 2

void *f(void *arg) {
  	long id = *(long*)arg;
    printf("Hello World din thread-ul %ld!\n", id);
  	pthread_exit(NULL);
}

void *g(void *arg) {
  	long id = *(long*)arg;
    printf("Hello World modificat din thread-ul %ld!\n", id);
  	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t threads[cores];
  	int r;
  	long id;
  	void *status;
	long ids[cores];
		
  	for (id = 0; id < cores; id++) {
		ids[id] = id; 
        if(id % 2 == 0)
		    r = pthread_create(&threads[id], NULL, f, &ids[id]);
        else 
            r = pthread_create(&threads[id], NULL, g, &ids[id]);

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
