# Compiler
CC=gcc

# Likwid
LIKWID_PATH=/home/soft/likwid
#LIKWID_OPTIONS=-DLIKWID_PERFMON -DUSE_LIKWID -I $(LIKWID_PATH)/include -L $(LIKWID_PATH)/lib -llikwid
LIKWID_OPTIONS=

# Compiler options
CC_OPTIONS=-fopenmp $(LIKWID_OPTIONS) -Wall -O0 

all: karatsuba karatsuba_par

karatsuba: karatsuba.c
	$(CC) $(CC_OPTIONS) -o $@ $^

karatsuba_par: karatsuba_par.c
	$(CC) $(CC_OPTIONS) -o $@ $^

clean:
	rm karatsuba karatsuba_par
