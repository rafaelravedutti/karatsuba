# Compiler
CC=gcc

# Likwid
LIKWID_OPTIONS=-llikwid -DUSE_LIKWID -DLIKWID_PERFMON
#LIKWID_OPTIONS=

# Compiler options
CC_OPTIONS=-fopenmp $(LIKWID_OPTIONS) -Wall -O0

all: karatsuba karatsuba_seq naive naive_par

karatsuba: karatsuba.c
	$(CC) $(CC_OPTIONS) -o $@ $^

karatsuba_seq: karatsuba_seq.c
	$(CC) $(CC_OPTIONS) -o $@ $^

naive: naive.c
	$(CC) $(CC_OPTIONS) -o $@ $^

naive_par: naive_par.c
	$(CC) $(CC_OPTIONS) -o $@ $^
