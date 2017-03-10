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

default: $(GIT_HOOKS) computepi.o $(EXECUTABLE)

time_test_%: computepi.o %_test.o
	$(CC) $(CFLAGS) $^ -o $@ -lm

%_test.o: time_test.c
	$(CC) $(CFLAGS) -c $< -D$(subst _test.o,,$@) -o $@

benchmark_clock_gettime: computepi.o benchmark_clock_gettime.c
	$(CC) $(CFLAGS) $< $@.c -o $@ -lm

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
