#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "computepi.h"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0

double scope[2];

void Deviation(double *time, int loop)
{
    double mean, sum = 0;

    for (int i = 0; i < loop; i++) {
        sum = sum + time[i];
    }
    mean = sum / loop;
    sum = 0;
    for (int i = 0; i < loop; i++) {
        sum = sum + pow(mean - time[i], 2);
    }

    scope[0] = mean - sqrt(sum / loop);
    scope[1] = mean + sqrt(sum / loop);
}



int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};
    FILE *fp;
    fp = fopen("error.csv", "a+w");

    if (argc < 2) return -1;

    int N = atoi(argv[1]);
    int i, loop = 30;
    double time[loop];
    double error, result;

    fprintf(fp, "%d ,", N);
    // Baseline
    for (i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_baseline(N);
        clock_gettime(CLOCK_ID, &end);
        time[i] = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / ONE_SEC;
    }
    Deviation(time, loop);

    while(1) {
        clock_gettime(CLOCK_ID, &start);
        result = compute_pi_baseline(N);
        clock_gettime(CLOCK_ID, &end);
        time[0] = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / ONE_SEC;
        if (scope[0] <= time[0] && time[0] <= scope[1])
            break;
    }
    error = (result- M_PI) / M_PI;
    fprintf(fp, "%e, ", error);
    printf("%lf, ", (double) time[0]);

    // OpenMP with 2 threads
    for (i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 2);
        clock_gettime(CLOCK_ID, &end);
        time[i] = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / ONE_SEC;
    }
    Deviation(time, loop);

    while(1) {
        clock_gettime(CLOCK_ID, &start);
        result = compute_pi_openmp(N, 2);
        clock_gettime(CLOCK_ID, &end);
        time[0] = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / ONE_SEC;
        if (scope[0] <= time[0] && time[0] <= scope[1])
            break;
    }
    error = (result - M_PI) / M_PI;
    fprintf(fp, "%e, ", error);
    printf("%lf, ", (double) time[0]);

    // OpenMP with 4 threads
    for (i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 4);
        clock_gettime(CLOCK_ID, &end);
        time[i] = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / ONE_SEC;
    }
    Deviation(time, loop);

    while(1) {
        clock_gettime(CLOCK_ID, &start);
        result = compute_pi_openmp(N, 4);
        clock_gettime(CLOCK_ID, &end);
        time[0] = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / ONE_SEC;
        if (scope[0] <= time[0] && time[0] <= scope[1])
            break;
    }
    error = (result - M_PI) / M_PI;
    fprintf(fp, "%e, ", error);
    printf("%lf, ", (double) time[0]);

    // AVX SIMD
    for (i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_avx(N);
        clock_gettime(CLOCK_ID, &end);
        time[i] = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / ONE_SEC;
    }
    Deviation(time, loop);

    while(1) {
        clock_gettime(CLOCK_ID, &start);
        result = compute_pi_avx(N);
        clock_gettime(CLOCK_ID, &end);
        time[0] = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / ONE_SEC;
        if (scope[0] <= time[0] && time[0] <= scope[1])
            break;
    }
    error = (result - M_PI) / M_PI;
    fprintf(fp, "%e, ", error);
    printf("%lf, ", (double) time[0]);

    // AVX SIMD + Loop unrolling
    for (i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_avx_unroll(N);
        clock_gettime(CLOCK_ID, &end);
        time[i] = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / ONE_SEC;
    }
    Deviation(time, loop);

    while(1) {
        clock_gettime(CLOCK_ID, &start);
        result = compute_pi_avx_unroll(N);
        clock_gettime(CLOCK_ID, &end);
        time[0] = (end.tv_sec - start.tv_sec) +
                  (end.tv_nsec - start.tv_nsec) / ONE_SEC;
        if (scope[0] <= time[0] && time[0] <= scope[1])
            break;
    }
    error = (result - M_PI) / M_PI;
    fprintf(fp, "%e\n", error);
    printf("%lf\n", (double) time[0]);

    fclose(fp);
    return 0;
}
