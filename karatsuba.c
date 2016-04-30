/*
 * Karatsuba - "Divide and Conquer" Big Number Multiplication Solver
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
void karatsuba(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
void _karatsuba(big_number_t x, big_number_t y, big_number_t dest, big_number_t dump, unsigned int n);

/* Funções auxiliares */
void fprint_big_number(FILE *out, big_number_t x, unsigned int n);
void fix_carry(big_number_t x, unsigned int n);
unsigned int min_power_of_2(unsigned int n1, unsigned int n2);

/* Multiplicação "ingênua" */
void naive_multiplication(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  unsigned int i, j;

  /* Zera os valores do resultado */
  for(i = 0; i < n * 2; ++i) {
    dest[i] = 0;
  }

  /* Resolve a multiplicação como se faz na primeira série */
  for(i = 0; i < n; ++i) {
    for(j = 0; j < n; ++j) {
      dest[i + j] += x[i] * y[j];
    }
  }
}

/* Karatsuba (base) */
void karatsuba(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  big_number_t dump;

  /* Aloca região de memória usada para armazenar resultados temporários */
  dump = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n * 4);

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
  unsigned int m, i;
  big_number_t z0, z1, z2;
  big_number_t z1_factor[2];

  /* Define m, como n é sempre potência de 2, então n / 2 será inteiro em
     todas as recursões, por questões de facilidade */
  m = n / 2;

  /* z0 aponta para o resultado em [0..n-1] e z2 aponta para o resultado em [n..2n-1],
     conforme especificação do algoritmo */
  z0 = dest;
  z2 = dest + n;

  /* Usa a região de dump para calcular z1 e seus fatores (x0+xm e y0+ym) */
  z1 = dump;
  z1_factor[0] = dump + n;
  z1_factor[1] = dump + n + m;

  /* Se o tamanho é menor que o CUTOFF, resolve utilizando a multiplicação "ingênua",
     este procedimento é feito para evitar diversas chamadas de funções e operações com pilha */
  if(n <= CUTOFF) {
    naive_multiplication(x, y, dest, n);
  /* Caso contrário, resolve recursões */
  } else {
    /* Resolve z0 recursivamente */
    _karatsuba(x, y, z0, dump + (n * 2), m);
    /* Resolve z2 recursivamente */
    _karatsuba(x + m, y + m, z2, dump + (n * 2), m);

    /* Calcula os fatores de z1 */
    for(i = 0; i < m; ++i) {
      z1_factor[0][i] = x[i] + x[m + i];
      z1_factor[1][i] = y[i] + y[m + i];
    }

    /* Resolve z1 recursivamente */
    _karatsuba(z1_factor[0], z1_factor[1], z1, dump + (n * 2), m);

    /* Subtrai em z1 os valores de z2 e z0 conforme especificação */
    for(i = 0; i < n; ++i) {
      z1[i] = z1[i] - z2[i] - z0[i];
    }

    /* Adiciona z1 no resultado em [n/2...2n/3] */
    for(i = 0; i < n; ++i) {
      dest[i + m] += z1[i];
    }
  }
}

/* Função para corrigir os carries, quando algum elemento no Big Number é
   maior do que 10 */
void fix_carry(big_number_t x, unsigned int n) {
  unsigned int i;
  int carry = 0;

  /* Percorre do elemento menos significativo ao mais */
  for(i = 0; i < n; ++i) {
    /* Adiciona o carry obtido anteriormente no elemento atual */
    x[i] += carry;

    /* Se o elemento for negativo */
    if(x[i] < 0) {
      carry = (-(x[i] + 1) / 10) + 1;
    /* Caso contrário, armazena no carry os digitos a partir da segunda casa (base 10) */ 
    } else {
      carry = x[i] / 10;
    }

    /* Subtrai do elemento atual os digitos a partir da segunda casa (carry) */
    x[i] -= carry * 10;
  }

  /* Se há carry porém não há mais elementos, então ocorreu overflow */
  if(carry > 0) {
    fprintf(stderr, "Overflow occured at 0x%p !!!\n", x);
  }
}

/* Imprime um Big Number na saída especificada */
void fprint_big_number(FILE *out, big_number_t x, unsigned int n) {
  int i;

  /* Não imprime zeros mais significativos */
  for(i = n - 1; i > 0 && x[i] == 0; --i);

  /* Imprime caracteres de 0 a 9 */
  while(i >= 0) {
    fprintf(out, "%u", x[i--]);
  }
}

/* Obtêm a menor potência de 2 maior que n1 e n2 */
unsigned int min_power_of_2(unsigned int n1, unsigned int n2) {
  unsigned int result = 2;

  /* Enquanto o resultado for menor que n1 e n2, multiplica-o por 2 */
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

  /* Verifica argumentos */
  if(argc < 3) {
    fprintf(stdout, "Uso: %s <factor 1> <factor 2>\n", argv[0]);
    return 0;
  }

  /* Tamanho das entradas */
  len1 = strlen(argv[1]);
  len2 = strlen(argv[2]);

  /* Para facilitar, n será a menor potência de 2 maior que o tamanho das entradas */
  n = min_power_of_2(len1, len2);

  /* Aloca memória para armazenar os Big Numbers e o resultado */
  x = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n);
  y = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n);
  d = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * n * 2);

  /* Inicializa Big Numbers */
  for(i = 0; i < n; ++i) {
    x[i] = 0;
    y[i] = 0;
    d[i] = d[n + i] = 0;
  }

  /* Armazena os valores lido na ordem inversa */
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

  /* Realiza a multiplicação de Karatsuba */
  karatsuba(x, y, d, n);
  /* Corrige os problemas de carry */
  fix_carry(d, n * 2);

  /* Imprime resultado */
  fprint_big_number(stdout, x, n);
  fprintf(stdout, " x ");
  fprint_big_number(stdout, y, n);
  fprintf(stdout, " = ");
  fprint_big_number(stdout, d, n * 2);
  fprintf(stdout, "\n");

  /* Libera memória ocupada pelos Big Numbers */
  free(x);
  free(y);
  free(d);
  return 0;
}
