CC = gcc
CFLAGS = -O0 -std=gnu99 -Wall -fopenmp -mavx
EXECUTABLE = \
	time_test_baseline time_test_openmp_2 time_test_openmp_4 \
	time_test_avx time_test_avxunroll \
	benchmark_clock_gettime

GIT_HOOKS := .git/hooks/pre-commit

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

default: $(GIT_HOOKS) computepi.o
	$(CC) $(CFLAGS) computepi.o time_test.c -DBASELINE -o time_test_baseline -lm
	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_2 -o time_test_openmp_2 -lm
	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_4 -o time_test_openmp_4 -lm
	$(CC) $(CFLAGS) computepi.o time_test.c -DAVX -o time_test_avx -lm
	$(CC) $(CFLAGS) computepi.o time_test.c -DAVXUNROLL -o time_test_avxunroll -lm
	$(CC) $(CFLAGS) computepi.o benchmark_clock_gettime.c -o benchmark_clock_gettime -lm
	$(CC) $(CFLAGS) computepi.o Machin_like.c  -o Machin_like -lm

.PHONY: clean default

%.o: %.c
	$(CC) -c $(CFLAGS)  $< -o $@ 

check: default
	time ./time_test_baseline
	time ./time_test_openmp_2
	time ./time_test_openmp_4
	time ./time_test_avx
	time ./time_test_avxunroll

gencsv: rm_file
	for i in `seq 100 300 100000`; do \
		printf "%d," $$i;\
		./benchmark_clock_gettime $$i; \
	done > result.csv	

Machin_like: rm_file
	for i in `seq 1 1 50`; do \
		printf "%d," $$i;\
		./Machin_like $$i; \
	done > result.csv	

plot: result.csv error.csv
	gnuplot scripts/runtime.gp
	gnuplot scripts/error.gp

rm_file: 
	rm -rf result.csv error.csv

clean:
	rm -f $(EXECUTABLE) *.o *.s result_clock_gettime.csv
