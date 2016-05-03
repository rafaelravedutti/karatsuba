/*
 * Karatsuba - "Divide and Conquer" Big Number Multiplication Solver
 *
 * Copyright (C) 2016  Felix Yowtang Liu
 *                     Rafael Ravedutti Lucio Machado
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tamanho a partir do qual será executada a multiplicação "ingênua" */
#define CUTOFF                8
/* Tamanho da granularidade (elementos) dos Big Numbers */
#define BIGNUM_GRANULE_SIZE   sizeof(char)

/* Big Number */
typedef char *big_number_t;

/* Funções de multiplicação */
void naive_multiplication(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
void big_number_summation(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
void big_number_subtraction(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
void karatsuba(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
void _karatsuba(big_number_t x, big_number_t y, big_number_t dest, big_number_t dump, unsigned int n);

/* Funções auxiliares */
void fprint_big_number(FILE *out, big_number_t x, unsigned int n);
void fix_carry(big_number_t x, unsigned int n);
unsigned int min_power_of_2(unsigned int n1, unsigned int n2);

/* Multiplicação "ingênua" */
void naive_multiplication(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  unsigned int i, j;
  char res;
  int carry;

  for(i = 0; i < n * 2; ++i) {
    dest[i] = 0;
  }

  for(i = 0; i < n; ++i) {
    carry = 0;

    for(j = 0; j < n; ++j) {
      res = carry + dest[i + j] + x[i] * y[j];
      dest[i + j] = res % 10;
      carry = res / 10;
    }

    dest[i + n] = carry;
  }
}

/* Soma de Big Numbers */
void big_number_summation(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  char carry, res;
  unsigned int i;

  for(i = 0, carry = 0; i < n; ++i) {
    res = x[i] + y[i] + carry;
    dest[i] = res % 10;
    carry = res / 10;
  }

  dest[n] = carry;
}

/* Subtração de Big Numbers */
void big_number_subtraction(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  char carry, res;
  unsigned int i;

  fprintf(stdout, "\nSUB: ");
  fprint_big_number(stdout, x, n);
  fprintf(stdout, " - ");
  fprint_big_number(stdout, y, n);

  for(i = 0, carry = 1; i < n; ++i) {
    res = 9 - y[i] + x[i] + carry;
    dest[i] = res % 10;
    carry = res / 10;
  }

  fprintf(stdout, " = ");
  fprint_big_number(stdout, dest, n);
  fprintf(stdout, "\n");
}

/* Karatsuba */
void karatsuba(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  big_number_t dump;

  dump = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * (n + 1) * 4);

  if(dump != NULL) {
    _karatsuba(x, y, dest, dump, n);
    free(dump);
  }
}

void _karatsuba(big_number_t x, big_number_t y, big_number_t dest, big_number_t dump, unsigned int n) {
  unsigned int m;
  big_number_t z0, z1, z2, temp;
  big_number_t z1_factor[2];

  m = n / 2;

  z0 = dest;
  z2 = dest + n;

  z1 = dump;
  z1_factor[0] = dump + n;
  z1_factor[1] = dump + n + m + 1;

  temp = dump + n;

  if(n <= CUTOFF) {
    naive_multiplication(x, y, dest, n);
  } else {
    _karatsuba(x, y, z0, dump + ((n + 1) * 2), m);
    _karatsuba(x + m, y + m, z2, dump + ((n + 1) * 2), m);

    big_number_summation(x, x + m, z1_factor[0], m);
    big_number_summation(y, y + m, z1_factor[1], m);
    
    _karatsuba(z1_factor[0], z1_factor[1], z1, dump + ((n + 1) * 2), m);

    big_number_summation(z0, z2, temp, n);
    big_number_subtraction(z1, temp, z1, n);
    big_number_summation(dest + m, z1, dest + m, n);
  }
}

/* Imprime um Big Number na saída especificada */
void fprint_big_number(FILE *out, big_number_t x, unsigned int n) {
  int i;

  for(i = n - 1; i > 0 && x[i] == 0; --i);

  while(i >= 0) {
    fprintf(out, "%u", x[i--]);
  }
}

/* Obtêm a menor potência de 2 maior que n1 e n2 */
unsigned int min_power_of_2(unsigned int n1, unsigned int n2) {
  unsigned int result = 2;

  while(result < n1 || result < n2) {
    result *= 2;
  }

  return result;
}

/* Função principal */
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
