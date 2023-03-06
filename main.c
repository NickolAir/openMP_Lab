#include <stdio.h>
#include <omp.h>

int main() {
    printf("Hello, World!\n");
#pragma omp parallel num_threads (2000)
    { int i,n;
        i = omp_get_thread_num();
        n = omp_get_num_threads();
        printf("I’m thread %d of %d\n",i,n);
    }
    return 0;
}