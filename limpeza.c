#include "projeto.h"

/*****************************************************************************
 * limpar_dicionario()
 *
 * Argumentos:
 *      Dicionario *dic - Ponteiro para a estrutura de dicionário a ser limpa.
 *
 * Retorna:
 *      void
 *
 * Descrição:
 *      Liberta toda a memória alocada dinamicamente para armazenar as palavras
 *      do dicionário. Após a execução, o ponteiro da tabela é definido como NULL
 *      e o contador de palavras passa a zero.
 *
 ****************************************************************************/

void limpar_dicionario(Dicionario *dic) {
    if (!dic) return;
    for (int i = 0; i < dic->n_palavras; i++) {
        free(dic->tabela[i]);
    }
    free(dic->tabela);
    dic->tabela = NULL;
    dic->n_palavras = 0;
}
/*****************************************************************************
 * limpar_linhas()
 *
 * Argumentos:
 *      Linha *inicio - Ponteiro para o início da lista ligada de linhas.
 *
 * Retorna:
 *      void
 *
 * Descrição:
 *      Percorre a lista ligada de linhas, libertando a memória associada a cada
 *      nó (estrutura Linha) e ao conteúdo de cada linha de texto.
 *
 ****************************************************************************/

void limpar_linhas(Linha *inicio) {
    Linha *atual = inicio;
    while (atual != NULL) {
        Linha *proximo = atual->proximo;
        free(atual->conteudo);  
        free(atual);
        atual = proximo;
    }
}

/*****************************************************************************
 * limpar_erros()
 *
 * Argumentos:
 *      Erro *lista - Ponteiro para o início da lista ligada de erros.
 *
 * Retorna:
 *      void
 *
 * Descrição:
 *      Liberta toda a memória associada à lista de erros, incluindo a memória
 *      de cada palavra com erro e os próprios nós da lista.
 *
 ****************************************************************************/

void limpar_erros(Erro *lista) {
    while (lista != NULL) {
        Erro *temp = lista;
        lista = lista->proximo;
        free(temp->palavra);  
        free(temp);           
    }
}
