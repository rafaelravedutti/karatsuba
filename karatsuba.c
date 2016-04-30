#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUTOFF                4
#define BIGNUM_GRANULE_SIZE   sizeof(char)

typedef char *big_number_t;

void naive_multiplication(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
void karatsuba(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
void _karatsuba(big_number_t x, big_number_t y, big_number_t dest, big_number_t dump, unsigned int n);
void fprint_big_number(FILE *out, big_number_t x, unsigned int n);

void naive_multiplication(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  unsigned int i, j;

  for(i = 0; i < n * 2; ++i) {
    dest[i] = 0;
  }

  for(i = 0; i < n; ++i) {
    for(j = 0; j < n; ++j) {
      dest[i + j] += x[i] * y[j];
    }
  }
}

void karatsuba(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  big_number_t dump;

  dump = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n * 4);

  if(dump != NULL) {
    _karatsuba(x, y, dest, dump, n);
    free(dump);
  }
}

void _karatsuba(big_number_t x, big_number_t y, big_number_t dest, big_number_t dump, unsigned int n) {
  unsigned int m, i;
  big_number_t z0, z1, z2;
  big_number_t z1_factor[2];

  m = n / 2;

  z0 = dest;
  z2 = dest + n;

  z1 = dump;
  z1_factor[0] = dump + n;
  z1_factor[1] = dump + n + m;

  if(n <= CUTOFF) {
    naive_multiplication(x, y, dest, n);
  } else {
    _karatsuba(x, y, z0, dump + (n * 2), m);
    _karatsuba(x + m, y + m, z2, dump + (n * 2), m);

    for(i = 0; i < m; ++i) {
      z1_factor[0][i] = x[i] + x[m + i];
      z1_factor[1][i] = y[i] + y[m + i];
    }

    _karatsuba(z1_factor[0], z1_factor[1], z1, dump + (n * 2), m);

    for(i = 0; i < n; ++i) {
      z1[i] = z1[i] - z2[i] - z0[i];
    }

    for(i = 0; i < n; ++i) {
      dest[i + m] += z1[i];
    }
  }
}

void fix_carry(big_number_t x, unsigned int n) {
  unsigned int i;
  int carry = 0;

  for(i = 0; i < n; ++i) {
    x[i] += carry;

    if(x[i] < 0) {
      carry = (-(x[i] + 1) / 10) + 1; 
    } else {
      carry = x[i] / 10;
    }

    x[i] -= carry * 10;
  }

  if(carry > 0) {
    fprintf(stderr, "Overflow occured at 0x%p !!!\n", x);
  }
}

void fprint_big_number(FILE *out, big_number_t x, unsigned int n) {
  int i;

  for(i = n - 1; i > 0 && x[i] == 0; --i);

  while(i >= 0) {
    fprintf(out, "%u", x[i--]);
  }
}

unsigned int min_power_of_2(unsigned int n1, unsigned int n2) {
  unsigned int result = 2;

  while(result < n1 || result < n2) {
    result *= 2;
  }

  return result;
}

int main(int argc, const char *argv[]) {
  big_number_t x, y, d;
  unsigned int n, len1, len2, i;
  int p1, p2;

  if(argc < 3) {
    fprintf(stdout, "Uso: %s <factor 1> <factor 2>\n", argv[0]);
    return 0;
  }

  len1 = strlen(argv[1]);
  len2 = strlen(argv[2]);
  n = min_power_of_2(len1, len2);

  x = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n);
  y = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n);
  d = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n * 2);

  for(i = 0; i < n; ++i) {
    x[i] = 0;
    y[i] = 0;
    d[i] = d[n + i] = 0;
  }

  p1 = len1 - 1;
  p2 = len2 - 1;

  for(i = 0; i < n; ++i) {
    if(p1 >= 0) {
      x[i] = argv[1][p1--] - '0';
    }

    if(p2 >= 0) {
      y[i] = argv[2][p2--] - '0';
    }
  }

  karatsuba(x, y, d, n);
  fix_carry(d, n * 2);

  fprint_big_number(stdout, x, n);
  fprintf(stdout, " x ");
  fprint_big_number(stdout, y, n);
  fprintf(stdout, " = ");
  fprint_big_number(stdout, d, n * 2);
  fprintf(stdout, "\n");

  free(x);
  free(y);
  free(d);
  return 0;
}
