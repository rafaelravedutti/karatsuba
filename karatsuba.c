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

/* Debug */
//#define DEBUG
/* Tamanho a partir do qual será executada a multiplicação "ingênua" */
#define CUTOFF                42
/* Tamanho da granularidade (elementos) dos Big Numbers */
#define BIGNUM_GRANULE_SIZE   sizeof(char)

/* Big Number */
typedef char *big_number_t;

/* Funções de operação com Big Numbers */
void naive_multiplication(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
void big_number_summation(big_number_t x, big_number_t y, big_number_t dest, unsigned int nx, unsigned int ny);
void big_number_subtraction(big_number_t x, big_number_t y, big_number_t dest, unsigned int nx, unsigned int ny);
void karatsuba(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
void _karatsuba(big_number_t x, big_number_t y, big_number_t dest, big_number_t dump, unsigned int n);

/* Funções auxiliares */
void fprint_big_number(FILE *out, big_number_t x, unsigned int n);
void fix_carry(big_number_t x, unsigned int n);

/* Multiplicação "ingênua" */
void naive_multiplication(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  unsigned int i, j;
  char res;
  int carry;

#ifdef DEBUG
  fprintf(stdout, "NAIVE(%u, %u): ", n, n);
  fprint_big_number(stdout, x, n);
  fprintf(stdout, " x ");
  fprint_big_number(stdout, y, n);
#endif

  /* Zera os valores do resultado */
  for(i = 0; i < n * 2; ++i) {
    dest[i] = 0;
  }

  /* Percorre todas os elementos em x */
  for(i = 0; i < n; ++i) {
    carry = 0;

    /* Realiza a multiplicação com os termos de y e soma no destino, verificando problemas de carry */
    for(j = 0; j < n; ++j) {
      res = carry + dest[i + j] + x[i] * y[j];
      dest[i + j] = res % 10;
      carry = res / 10;
    }

    /* Armazena o carry da operação no valor mais significativo */
    dest[i + n] = carry;
  }

#ifdef DEBUG
  fprintf(stdout, " = ");
  fprint_big_number(stdout, dest, n * 2);
  fprintf(stdout, "\n");
#endif

}

/* Soma de Big Numbers */
void big_number_summation(big_number_t x, big_number_t y, big_number_t dest, unsigned int nx, unsigned int ny) {
  big_number_t bigger;
  char carry, res;
  unsigned int i, min, max;

#ifdef DEBUG
  fprintf(stdout, "SUM(%u, %u): ", nx, ny);
  fprint_big_number(stdout, x, nx);
  fprintf(stdout, " + ");
  fprint_big_number(stdout, y, ny);
#endif

  min = (nx < ny) ? nx : ny;
  max = (nx > ny) ? nx : ny;
  bigger = (nx > ny) ? x : y;

  /* Percorre os elementos, realizando a soma e verificando os problemas de carry */
  for(i = 0, carry = 0; i < min; ++i) {
    res = x[i] + y[i] + carry;
    dest[i] = res % 10;
    carry = res / 10;
  }

  /* Se os números tem diferentes tamanho, termina a soma apenas com os valores do maior */
  for(i = min; i < max; ++i) {
    res = bigger[i] + carry;
    dest[i] = res % 10;
    carry = res / 10;
  }

  dest[max] += carry;

#ifdef DEBUG
  fprintf(stdout, " = ");
  fprint_big_number(stdout, dest, max + 1);
  fprintf(stdout, " (CARRY = %d)\n", carry);
#endif

}

/* Subtração de Big Numbers */
void big_number_subtraction(big_number_t x, big_number_t y, big_number_t dest, unsigned int nx, unsigned int ny) {
  char carry, res;
  unsigned int i;

#ifdef DEBUG
  fprintf(stdout, "SUB(%u, %u): ", nx, ny);
  fprint_big_number(stdout, x, nx);
  fprintf(stdout, " - ");
  fprint_big_number(stdout, y, ny);
#endif

  /* Realiza a subtração em cada elemento, verificando problemas de carry */
  for(i = 0, carry = 1; i < ny; ++i) {
    res = 9 - y[i] + x[i] + carry;
    dest[i] = res % 10;
    carry = res / 10;
  }

  /* Se ainda há valores em x, copia-os para o resultado */
  for(i = ny; i < nx; ++i) {
    res = 9 + x[i] + carry;
    dest[i] = res % 10;
    carry = res / 10;
  }

#ifdef DEBUG
  fprintf(stdout, " = ");
  fprint_big_number(stdout, dest, nx);
  fprintf(stdout, "\n");
#endif

}

/* Karatsuba (base) */
void karatsuba(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  big_number_t dump;

  /* Aloca região de memória usada para armazenar resultados temporários */
  dump = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * ((n + 4) * 4 + n));

  /* Se não ocorreu erro ao alocar */
  if(dump != NULL) {
    /* Chama a função recursiva de Karatsuba, junto com o dump */
    _karatsuba(x, y, dest, dump, n);
    /* Libera a região de memória temporária */
    free(dump);
  }
}

/* Karatsuba (recursões) */
void _karatsuba(big_number_t x, big_number_t y, big_number_t dest, big_number_t dump, unsigned int n) {
  unsigned int m, nz0, nz1, nz2, nz1f, i;
  big_number_t z0, z1, z2, z1f1, z1f2, temp_sum;

  /* Define valor de m */
  m = n / 2;

  /* z0 aponta para o resultado em [0..n-1] e z2 aponta para o resultado em [n..2n-1],
     conforme especificação do algoritmo */
  z0 = dest;
  z2 = dest + m + m;

  /* Usa a região de dump para calcular z1 e seus fatores (x0+xm e y0+ym) */
  z1 = dump;
  z1f1 = dump + n + 4;
  z1f2 = dump + n + 4 + m + 2;

  /* Região temporária para armazenar z0 + z2 */
  temp_sum = dump + n + 4;

  /* Tamanhos */
  nz0 = m * 2;
  nz2 = (n - m) * 2;
  nz1f = m + (n % 2) + 1;
  nz1 = nz1f * 2;

  /* Se o tamanho é menor que o CUTOFF, resolve utilizando a multiplicação "ingênua",
     este procedimento é feito para evitar diversas chamadas de funções e operações com pilha */
  if(n <= CUTOFF) {
    naive_multiplication(x, y, dest, n);
  /* Caso contrário, resolve recursões */
  } else {
    /* Resolve z0 recursivamente */
    _karatsuba(x, y, z0, dump + (n + 4) * 2, m);
    /* Resolve z2 recursivamente */
    _karatsuba(x + m, y + m, z2, dump + (n + 4) * 2, n - m);

    /* Zera fatores de z1 */
    for(i = 0; i < m + 2; ++i) {
      z1f1[i] = z1f2[i] = 0;
    }

    /* Calcula os fatores de z1 */
    big_number_summation(x, x + m, z1f1, m, m + (n % 2));
    big_number_summation(y, y + m, z1f2, m, m + (n % 2));

    /* Resolve z1 recursivamente */
    _karatsuba(z1f1, z1f2, z1, dump + (n + 4) * 2, nz1f);

    /* Zera temp_sum */
    for(i = 0; i < n + 4; ++i) {
      temp_sum[i] = 0;
    }

    /* Subtrai em z1 os valores de z2 e z0 conforme especificação */
    big_number_summation(z0, z2, temp_sum, nz0, nz2);
    big_number_subtraction(z1, temp_sum, z1, nz1, nz2 + 1);

    /* Adiciona z1 no resultado em [n/2...2n/3] */
    big_number_summation(dest + m, z1, dest + m, nz1, nz1);
  }
}

/* Imprime um Big Number na saída especificada */
void fprint_big_number(FILE *out, big_number_t x, unsigned int n) {
  int i;

  /* Não imprime zeros mais significativos */
  for(i = n - 1; i > 0 && x[i] == 0; --i);

  /* Imprime caracteres de 0 a 9 */
  while(i >= 0) {
    if(x[i] > 10) {
      fprintf(out, "(%u)", x[i]);
    } else {
      fprintf(out, "%u", x[i]);
    }

    --i;
  }
}

int get_file_length(const char *filename) {
  FILE *fp;
  int ret;

  if((fp = fopen(filename, "r")) == NULL) {
    fprintf(stdout, "Falha ao abrir o arquivo \"%s\".\n", filename);
    exit(-1);
  }

  fseek(fp, 0, SEEK_END);
  ret = ftell(fp);
  fclose(fp);
  return ret;  
}

void fget_big_number(const char *filename, big_number_t x, unsigned int n) {
  FILE *fp;
  int i;

  if((fp = fopen(filename, "r")) == NULL) {
    fprintf(stdout, "Falha ao abrir o arquivo \"%s\".\n", filename);
    exit(-1);
  }

  for(i = n - 1; i >= 0; --i) {
    x[i] = fgetc(fp) - '0';
  }

  fclose(fp);
}

/* Função principal */
int main(int argc, const char *argv[]) {
  big_number_t x, y, d;
  unsigned int n, len1, len2, i;
  int p1, p2;

  /* Verifica argumentos */
  if(argc < 2 || (argv[1][0] == '-' && argv[1][1] == '\0' && argc < 4)) {
    fprintf(stdout, "Uso: %s <filename 1> <filename 2>\n"
                    "     %s - <factor 1> <factor 2>\n", argv[0], argv[0]);
    return 0;
  }

  /* Tamanho das entradas */
  if(argv[1][0] == '-' && argv[1][1] == '\0') {
    len1 = strlen(argv[2]);
    len2 = strlen(argv[3]);
  } else {
    len1 = get_file_length(argv[1]) - 1;
    len2 = get_file_length(argv[2]) - 1;
  }

  /* n será o tamanho da maior entrada */
  n = (len1 > len2) ? len1 : len2;

  /* Aloca memória para armazenar os Big Numbers e o resultado */
  x = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n);
  y = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n);
  d = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n * 2);

  /* Inicializa Big Numbers */
  for(i = 0; i < n; ++i) {
    x[i] = 0;
    y[i] = 0;
    d[i] = d[i + n] = 0;
  }

  /* Se o primeiro parâmetro é "-" */
  if(argv[1][0] == '-' && argv[1][1] == '\0') {
    /* Armazena os valores lido na ordem inversa */
    p1 = len1 - 1;
    p2 = len2 - 1;

    for(i = 0; i < n; ++i) {
      if(p1 >= 0) {
        x[i] = argv[2][p1--] - '0';
      }

      if(p2 >= 0) {
        y[i] = argv[3][p2--] - '0';
      }
    }
  } else {
    fget_big_number(argv[1], x, len1);
    fget_big_number(argv[2], y, len2);
  }

  /* Realiza a multiplicação de Karatsuba */
  karatsuba(x, y, d, n);

#ifdef PRINTABLE
  /* Imprime resultado */
  fprint_big_number(stdout, x, n);
  fprintf(stdout, " x ");
  fprint_big_number(stdout, y, n);
  fprintf(stdout, " = ");
  fprint_big_number(stdout, d, n * 2);
  fprintf(stdout, "\n");
#endif

  /* Libera memória ocupada pelos Big Numbers */
  free(x);
  free(y);
  free(d);
  return 0;
}
