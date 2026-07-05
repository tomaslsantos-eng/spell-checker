#ifndef UTILS_H
#define UTILS_H

#include "projeto.h"

void processar_divisoes(Erro *erro, Dicionario *dic, Sugestao **sugestoes, int *n_sug, int max_sug, int max_dif);
int calcular_diferencas(const char *palavra_errada, const char *palavra_dicionario, int max_diferencas);
char* substituir_palavra(char *str, size_t pos, size_t len, const char *nova_palavra);



#endif