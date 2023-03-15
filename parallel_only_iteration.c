#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>

#define E 0.0001
#define t 0.0001
#define DEFAULT 3

void print_vector (double *vect, int N){
    for (int i = 0; i < N; ++i) {
        printf("%f ", vect[i]);
    }
    printf("\n");
}

double *create_vectorB (int N){
    double *vect = (double*) malloc(N * sizeof(double));
    for (int i = 0; i < N; ++i) {
        vect[i] = N + 1;
    }
    return vect;
}

double *create_x0 (int N){
    double *vect = (double*) malloc(N * sizeof(double));
    for (int i = 0; i < N; ++i) {
        vect[i] = 0;
    }
    return vect;
}

double *create_matrix (int N){
    int line = 0;
    double *A = (double*) malloc(N * N * sizeof(double));
    for (int i = 0; i < N * N; ++i) {
        if (i == line * N + line){
            A[i] = 2.0;
            line++;
        }else{
            A[i] = 1.0;
        }
    }
    return A;
}

double norm (const double *vector, int N){
    double res = 0;
    for (int i = 0; i < N; ++i) {
        res += vector[i] * vector[i];
    }
    return sqrt(res);
}

//критерий окончания итераций через эпсилон
int criterion (double *Ax, double normB, int N){
    double result = norm(Ax, N) / normB;
    if (result < E){
        return 1;
    } else {
        return 0;
    }
}
//res := Ax-b
void ParallelCalculate(double* Matrix, double* x0, double* res, double* vectorB, int size){
    #pragma omp for
    for (int i = 0; i < size; i++) {
        res[i] = 0 - vectorB[i];
        for (int j = 0; j < size; j++) {
            res[i] += Matrix[i * size + j] * x0[j];
        }
    }
}

void ParallelSubtract(double* res, double* x0, int size){
    #pragma omp for
    for (int i = 0; i < size; i++) {
        x0[i] -= res[i] * t;
    }
}

void FreeProcess(double* Matrix, double* vectorB, double* x0, double* res) {
    free(Matrix);
    free(vectorB);
    free(x0);
    free(res);
}

int main(int argc, char *argv[]) {
    double* Matrix;
    double* x0;
    double* vectorB;
    double* res;
    double start_time = 0.0, end_time;
    int size = DEFAULT;
    if (argc >= 2){
        size = atoi(argv[1]);
    }
    x0 = create_x0(size);
    res = create_x0(size);
    vectorB = create_vectorB(size);
    double normB;
    Matrix = create_matrix(size);
    start_time = omp_get_wtime();
    normB = norm(vectorB, size);
    int crit = 0;
    #pragma omp parallel
    {
        do {
            ParallelCalculate(Matrix, x0, res, vectorB, size);
            #pragma omp single
            {
                crit = criterion(res, normB, size);
            }
            ParallelSubtract(res, x0, size);
        } while(crit == 0);
    }
    end_time = omp_get_wtime();
    print_vector(x0, size);
    printf("TIME TAKEN: %lf\n", end_time - start_time);
    FreeProcess(Matrix, vectorB, x0, res);
}