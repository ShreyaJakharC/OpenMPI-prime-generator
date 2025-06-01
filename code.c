#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to check if a number is prime
int is_prime(int x) {
    if (x < 2)
        return 0;
    for (int i = 2; i * i <= x; i++) {
        if (x % i == 0)
            return 0;
    }
    return 1;
}

// Compare function for qsort, because the number in the .txt file are not
// in proper ascending order.
int cmpfunc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: ./genprimes M N t\n");
        return 1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int t = atoi(argv[3]);

    int* primes = (int*) malloc((N - M + 1) * sizeof(int));
    int count = 0;

    double tstart = 0.0, tend = 0.0, ttaken = 0.0;

    // Following the structure provided.
    tstart = omp_get_wtime();

    #pragma omp parallel num_threads(t) shared(primes, count)
    {
        #pragma omp for schedule(dynamic)
        for (int x = M; x <= N; x++) {
            if (is_prime(x)) {
                #pragma omp critical
                {
                    primes[count] = x;
                    count++;
                }
            }
        }
    }

    ttaken = omp_get_wtime() - tstart;

    printf("The number of prime numbers found between %d and %d is %d.\n", M, N, count);
    printf("Time taken for the main part: %f\n", ttaken);

    // Sort the primes array, to have ascending order.
    qsort(primes, count, sizeof(int), cmpfunc);

    // Making the .txt(output) file.
    char filename[100];
    sprintf(filename, "%d.txt", N);
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        free(primes);
        return 1;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d\n", primes[i]);
    }

    fclose(fp);
    free(primes);

    return 0;
}
