# Compiler
CC=gcc

# Likwid
LIKWID_PATH=/home/soft/likwid
LIKWID_OPTIONS=-DUSE_LIKWID -I $(LIKWID_PATH)/include -L $(LIKWID_PATH)/lib -DLIKWID_PERFMON -llikwid
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

clean:
	rm karatsuba karatsuba_seq naive naive_par
