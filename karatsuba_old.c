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

#define DEBUG(a) /* a */

/* Tamanho a partir do qual será executada a multiplicação "ingênua" */
#define CUTOFF                8
/* Tamanho da granularidade (elementos) dos Big Numbers */
#define BIGNUM_GRANULE_SIZE   sizeof(char)

/* Big Number */
typedef char *big_number_t;

/* Funções de operação com Big Numbers */
void naive_multiplication(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
int big_number_summation(big_number_t x, big_number_t y, big_number_t dest, unsigned int n);
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
}

/* Soma de Big Numbers */
int big_number_summation(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  char carry, res;
  unsigned int i;

  DEBUG(
  fprintf(stdout, "\nSUM: ");
  fprint_big_number(stdout, x, n);
  fprintf(stdout, " + ");
  fprint_big_number(stdout, y, n);
  )

  /* Percorre os elementos, realizando a soma e verificando os problemas de carry */
  for(i = 0, carry = 0; i < n; ++i) {
    res = x[i] + y[i] + carry;
    dest[i] = res % 10;
    carry = res / 10;
  }

  DEBUG(
  fprintf(stdout, " = ");
  if(carry)
    fprintf(stdout, "1");
  fprint_big_number(stdout, dest, n);
  fprintf(stdout, "\n");
  )

  /* Devolve o carry que sobrou, para ser tratado externamente (resultado tem n elementos) */
  return carry;
}

/* Subtração de Big Numbers */
void big_number_subtraction(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  char carry, res;
  unsigned int i;

  DEBUG(
  fprintf(stdout, "\nSUB: ");
  fprint_big_number(stdout, x, n);
  fprintf(stdout, " - ");
  fprint_big_number(stdout, y, n);
  )

  /* Realiza a subtração em cada elemento, verificando problemas de carry */
  for(i = 0, carry = 1; i < n; ++i) {
    res = 9 - y[i] + x[i] + carry;
    dest[i] = res % 10;
    carry = res / 10;
  }

  DEBUG(
  fprintf(stdout, " = ");
  fprint_big_number(stdout, dest, n);
  fprintf(stdout, "\n");
  )
}

/* Karatsuba (base) */
void karatsuba(big_number_t x, big_number_t y, big_number_t dest, unsigned int n) {
  big_number_t dump;

  /* Aloca região de memória usada para armazenar resultados temporários */
  dump = (big_number_t) malloc(BIGNUM_GRANULE_SIZE * (n + 4) * 4);

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
  unsigned int m;
  big_number_t z0, z1, z2, z1f1, z1f2, temp_sum;

  /* Define m, como n é sempre potência de 2, então n / 2 será inteiro em
     todas as recursões, por questões de facilidade */
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

    /* Calcula os fatores de z1 */
    z1f1[m] = big_number_summation(x, x + m, z1f1, m);
    z1f2[m] = big_number_summation(y, y + m, z1f2, m);

    /* Normaliza fatores para tamanho m + 2 */
    if(n % 2 != 0) {
      char soma = z1f1[m] + x[m + m];
      z1f1[m] = soma % 10;
      z1f1[m + 1] = soma / 10;

      soma = z1f2[m] + y[m + m];
      z1f2[m] = soma % 10;
      z1f2[m + 1] = soma / 10;
    } else
      z1f1[m + 1] = z1f2[m + 1] = 0;

    /* Resolve z1 recursivamente */
    _karatsuba(z1f1, z1f2, z1, dump + (n + 4) * 2, m + 2);

    /* Normaliza z1 para tamanho n + 4 */
    if(n % 2 != 0)
      z1[n + 3] = 0;

    /* Subtrai em z1 os valores de z2 e z0 conforme especificação */
    temp_sum[m+m] = big_number_summation(z0, z2, temp_sum, m+m);

    /* Normaliza soma de z0 e z2 para tamanho n + 4 */
    if(n % 2 != 0) {
      char soma = temp_sum[n - 1] + z2[n - 1];
      temp_sum[n - 1] = soma % 10;
      temp_sum[n] = soma / 10;

      soma = temp_sum[n] + z2[n];
      temp_sum[n] = soma % 10;
      temp_sum[n + 1] = soma / 10;
    } else
      temp_sum[n + 1] = 0;
    temp_sum[n + 2] = temp_sum[n + 3] = 0;

    big_number_subtraction(z1, temp_sum, z1, n + 4);

    /* Adiciona z1 no resultado em [n/2...2n/3] */
    if(big_number_summation(dest + m, z1, dest + m, n + 4)) {
      int carry = 1, p = m + n + 4;
      while(carry) {
        int soma = dest[p] + carry;
        dest[p] = soma % 10;
        carry = soma / 10;

        p++;
      }
    }
  }

  DEBUG(
  fprintf(stdout, "\nMUL: (n %d m %d) ", n, m);
  fprint_big_number(stdout, x, n);
  fprintf(stdout, " x ");
  fprint_big_number(stdout, y, n);
  fprintf(stdout, " = ");
  fprint_big_number(stdout, dest, n * 2);
  fprintf(stdout, "\n");
  )
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
    len1 = get_file_length(argv[1]);
    len2 = get_file_length(argv[2]);
  }

  /* Para facilitar, n será a menor potência de 2 maior que o tamanho das entradas */
  //n = min_power_of_2(len1, len2);
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
