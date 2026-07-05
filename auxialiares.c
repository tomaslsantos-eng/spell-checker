#include "projeto.h"

/*****************************************************************************
 * comparar_palavras()
 *
 * Argumentos:
 *      const void *a - Ponteiro genérico para o primeiro elemento (string).
 * 
 *      const void *b - Ponteiro genérico para o segundo elemento (string).
 *
 * Retorna:
 *      int - Resultado da comparação alfabética entre as duas palavras,
 *            ignorando diferenças entre maiúsculas e minúsculas.
 *            Retorna valor negativo se 'a' < 'b', 0 se forem iguais, positivo se 'a' > 'b'.
 *
 * Descrição:
 *      Função utilizada com qsort() para ordenar alfabeticamente um array de strings,
 *      ignorando capitalização (case-insensitive).
 *
 ****************************************************************************/

int comparar_palavras(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}


/*****************************************************************************
 * comparar_sugestoes()
 *
 * Argumentos:
 *      const void *a - Ponteiro para o primeiro elemento (Sugestao *).
 * 
 *      const void *b - Ponteiro para o segundo elemento (Sugestao *).
 * 
 * Retorna:
 *      int - Valor negativo se 'a' deve vir antes de 'b', positivo se 'b' antes de 'a',
 *            ou 0 se forem equivalentes.
 *
 * Descrição:
 *      Função utilizada com qsort() para ordenar sugestões de correção ortográfica.
 *      As sugestões são ordenadas pela menor diferença (número de edições),
 *      e, em caso de empate, pelo índice de ocorrência (ordem no dicionário).
 *
 ****************************************************************************/

int comparar_sugestoes(const void *a, const void *b)
{
    const Sugestao *sa = *(const Sugestao **)a;
    const Sugestao *sb = *(const Sugestao **)b;

    // Se a diferença for diferente, ordena pela menor diferença
    if (sa->diferenca != sb->diferenca)
    {
        return sa->diferenca - sb->diferenca;
    }

    // Se a diferença for igual, ordena pelo índice mais baixo (apareceu primeiro no texto)
    return sa->indice - sb->indice;
}

/*****************************************************************************
 * isPortugueseLetter()
 *
 * Argumentos:
 *      char c - caráter a verificar.
 *
 * Retorna:
 *      int - 1 (verdadeiro) se for uma letra válida em português, 0 caso contrário.
 *
 * Descrição:
 *      Verifica se um determinado caráter é uma letra do alfabeto português,
 *      incluindo letras acentuadas e o cedilha. Suporta tanto letras maiúsculas
 *      como minúsculas.
 *
 ****************************************************************************/

int isPortugueseLetter(char c) {
    const char *validChars = "àáéíóúãõâêôçÀÁÉÍÓÚÃÕÂÊÔÇ";
    return (isalpha((unsigned char)c) || strchr(validChars, c) != NULL);
}

/*****************************************************************************
 * extrair_palavras()
 *
 * Argumentos:
 *      const char *linha - String de entrada (linha de texto).
 * 
 *      char **palavras   - Array de strings onde as palavras extraídas serão guardadas.
 * 
 *      int *n_palavras   - Ponteiro para o número de palavras extraídas.
 *
 * Retorna:
 *      void
 *
 * Descrição:
 *      Esta função percorre uma linha de texto e extrai todas as palavras válidas,
 *      com base em letras do alfabeto português. Ignora pontuação e espaços.
 *      As palavras são armazenadas dinamicamente no array fornecido.
 *      Palavras com apóstrofos no meio (ex: d'água) são aceites.
 *
 ****************************************************************************/

void extrair_palavras(const char *linha, char **palavras, int *n_palavras) {
    int i = 0;
    int len = strlen(linha);
    *n_palavras = 0;

    while (i < len) {

        // Ignora carateres que não fazem parte de palavras (pontuação, espaços, etc.)
        while (i < len && !isPortugueseLetter(linha[i])) {
            i++;
        }

        if (i >= len) break;

        int inicio = i;      // Marca o início da palavra
        int eh_valida = 1;   // Placeholder, poderá ser útil se quiser validar com mais critérios

        // Avança enquanto for letra válida ou apóstrofo no meio da palavra (ex: d'água)
        while (i < len && (isPortugueseLetter(linha[i]) || 
              (linha[i] == '\'' && i > inicio && i < len-1 && isPortugueseLetter(linha[i+1])))) {
            i++;
        }

        // Se foi encontrada uma palavra válida, guarda-a
        if (eh_valida && i > inicio) {
            palavras[*n_palavras] = (char*)malloc(i - inicio + 1);
            strncpy(palavras[*n_palavras], linha + inicio, i - inicio);
            palavras[*n_palavras][i - inicio] = '\0';  // Adiciona o terminador de string
            (*n_palavras)++; // Incrementa o contador de palavras
        }
    }
}

/*****************************************************************************
 * procurarIndice()
 *
 * Argumentos:
 *      const char *parte - Palavra a procurar no dicionário.
 * 
 *      Dicionario *dic   - Estrutura contendo o dicionário a pesquisar.
 *
 * Retorna:
 *      int - Índice da palavra encontrada no dicionário, ou -1 se não existir.
 *
 * Descrição:
 *      Procura uma palavra no dicionário. Tenta encontrar uma correspondência
 *      ignorando capitalização.
 *
 ****************************************************************************/

int procurarIndice(const char *parte, Dicionario *dic) {
    // Procura ignorando capitalização (ex: "Casa" == "casa")
    for (int i = 0; i < dic->n_palavras; i++) {
        if (strcasecmp(parte, dic->tabela[i]) == 0) {
            return i;
        }
    }

    // Se não encontrou, retorna -1
    return -1;
}
