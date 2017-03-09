#include <stdio.h>

double arctan(double x, int N);
double compute_pi_baseline(size_t N);
double compute_pi_openmp(size_t N, int threads);
double compute_pi_avx(size_t N);
double compute_pi_avx_unroll(size_t N);
double Machin_like(size_t N);
