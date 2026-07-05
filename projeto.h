#ifndef PROJETO_H
#define PROJETO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <limits.h>


typedef struct {
    char **tabela;         
    int n_palavras;        
} Dicionario;

typedef struct Erro {
    char *palavra;
    struct Erro *proximo;  
} Erro;

typedef struct Sugestao {
    char *palavra;
    int diferenca;
    int indice; // Índice no dicionário ordenado
    struct Sugestao *proximo;
} Sugestao;

typedef struct Linha {
    char *conteudo;
    struct Linha *proximo;     
} Linha;


int comparar_palavras(const void *a, const void *b);
int comparar_sugestoes(const void *a, const void *b);



void metodo_1(const char *dicionario, const char *filename_input, const char *filename_output);
void metodo_2(const char *dicionario, const char *filename_input, const char *filename_output, 
              int max_alternativas, int max_diferencas);
void metodo_3(const char *dicionario, const char *input_file, const char *output_file, int max_dif);


void ler_dicionario(Dicionario *dic, const char *arquivo);
void limpar_dicionario(Dicionario *dic);
void limpar_erros(Erro *lista);
void limpar_linhas(Linha *inicio);


int isPortugueseLetter(char c);
void extrair_palavras(const char *linha, char **palavras, int *n_palavras);
int procurarIndice(const char *parte, Dicionario *dic);

#endif