/*
 * Naive Big Number Multiplication
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
/* Tamanho da granularidade (elementos) dos Big Numbers */
#define BIGNUM_GRANULE_SIZE   sizeof(char)

/* Big Number */
typedef char *big_number_t;
/* Big Number com dígitos grandes */
typedef int *big_number_long;

/* Funções de operação com Big Numbers */
void naive_multiplication(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
void naive_multiplication_sum(big_number_long orig, big_number_t dest, unsigned int n);
void naive_single_multiplication(big_number_t y, char x, big_number_long dest, unsigned int n);

/* Funções auxiliares */
void fprint_big_number(FILE *out, big_number_t x, unsigned int n);

/* Etapa de somas da multiplicação ingênua */
void naive_multiplication_sum(big_number_long orig, big_number_t dest, unsigned int n){
  int i, j, soma, carry;

  carry = 0;
  for(j = 0; j < 2*n; ++j){
    soma = carry;
    for(i = 0; i < n; ++i)
      soma += orig[i*(2*n+1)+j];
    dest[j] = soma % 10;
    carry = soma / 10;
  }
}

/* Etapa de multiplicação por um dígico da multiplicação ingênua */
void naive_single_multiplication(big_number_t y, char x, big_number_long dest, unsigned int n){
  int i, carry, prod;
  carry = 0;
  for(i=0; i<n; i++){
    prod = x*y[i] + carry;
    dest[i] = prod % 10;
    carry = prod / 10;
  }
  dest[n] = carry;
}

/* Multiplicação "ingênua" */
void naive_multiplication(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  unsigned int i, j;
  big_number_long rt;

  rt = (big_number_long) malloc(n*(2*n+1)*sizeof(int));
  if(!rt){
    perror("Não consegui alocar memória temporária para multiplicação ingênua");
    exit(-1);
  }

  for(i = 0; i < n; ++i)
    for(j = 0; j <= 2*n; ++j)
      rt[i*(2*n+1)+j] = 0;

  /* Zera os valores do resultado */
  for(i = 0; i < n * 2; ++i)
    dest[i] = 0;

#ifdef DEBUG
  printf("Multiplicando...\n");
#endif
  /* Percorre todas os elementos em x */
  for(i = 0; i < n; ++i)
    /* Faz rt[i] <- x[i]*y */
    naive_single_multiplication(y, x[i], &rt[i*(2*n+1)+i], n);

#ifdef DEBUG
  printf("Somando...\n");
#endif
  /* Faz a soma das coisas */
  naive_multiplication_sum(rt, dest, n);

  free(rt);

/*
#ifdef DEBUG
  fprintf(stdout, "NAIVE(%u, %u): ", n, n);
  fprint_big_number(stdout, x, n);
  fprintf(stdout, " x ");
  fprint_big_number(stdout, y, n);
  fprintf(stdout, " = ");
  fprint_big_number(stdout, dest, n * 2);
  fprintf(stdout, "\n");
#endif
// */
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

/* Imprime um Big Number longo na saída especificada */
void fprint_big_number_long(FILE *out, big_number_long x, unsigned int n) {
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
  naive_multiplication(x, y, d, n);

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
