#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N;
int P;
int *v;
int *vQSort;

void compare_vectors(int *a, int *b) {
	int i;

	for (i = 0; i < N; i++) {
		if (a[i] != b[i]) {
			printf("Sortare incorecta\n");
			return;
		}
	}

	printf("Sortare corecta\n");
}

void display_vector(int *v) {
	int i;
	int display_width = 2 + log10(N);

	for (i = 0; i < N; i++) {
		printf("%*i", display_width, v[i]);
	}

	printf("\n");
}

int cmp(const void *a, const void *b) {
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

void get_args(int argc, char **argv)
{
	if(argc < 3) {
		printf("Numar insuficient de parametri: ./oets N P\n");
		exit(1);
	}

	N = atoi(argv[1]);
	P = atoi(argv[2]);
}

void init()
{
	int i;
	v = (int*) malloc(sizeof(int) * N);
	vQSort = (int*) malloc(sizeof(int) * N);

	if (v == NULL || vQSort == NULL) {
		printf("Eroare la malloc!");
		exit(1);
	}

	srand(42);

	for (i = 0; i < N; i++)
		v[i] = rand() % N;
}

void print()
{
	printf("v:\n");
	display_vector(v);
	printf("vQSort:\n");
	display_vector(vQSort);
	compare_vectors(v, vQSort);
}

void swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int min(int a, int b)
{
	return a > b? a:b;
}

void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;
	int start = thread_id * N / 2;
	int end = min((thread_id + 1) * N / 2, N);
	int i = 0;

	if(start % 2 == 1) {
		for (int k = start + 1; k < N; k++) {
			for (i = 0; i < end - 1; i += 2) {
				if (v[i] > v[i + 1]) {
					swap(&v[i], &v[i + 1]);
				}
			}
			for (i = start; i < end - 1; i += 2) {
				if (v[i] > v[i + 1]) {
					swap(&v[i], &v[i + 1]);
				}
			}
		}
	} else {
		for (int k = start; k < N; k++) {
			for (i = 0; i < end - 1; i += 2) {
				if (v[i] > v[i + 1]) {
					swap(&v[i], &v[i + 1]);
				}
			}
			for (i = start + 1; i < end - 1; i += 2) {
				if (v[i] > v[i + 1]) {
					swap(&v[i], &v[i + 1]);
				}
			}
		}
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	get_args(argc, argv);
	init();

	int i, aux;
	pthread_t tid[P];
	int thread_id[P];

	// se sorteaza vectorul etalon
	for (i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	// se creeaza thread-urile
	for (i = 0; i < P; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	// se asteapta thread-urile
	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}


	// bubble sort clasic - trebuie transformat in OETS si paralelizat
	// int sorted = 0;
	// while (!sorted) {
	// 	sorted = 1;

	// 	for (i = 0; i < N-1; i++) {
	// 		if(v[i] > v[i + 1]) {
	// 			aux = v[i];
	// 			v[i] = v[i + 1];
	// 			v[i + 1] = aux;
	// 			sorted = 0;
	// 		}
	// 	}
	// }

	// se afiseaza vectorul etalon
	// se afiseaza vectorul curent
	// se compara cele doua
	print();

	free(v);
	free(vQSort);

	return 0;
}
