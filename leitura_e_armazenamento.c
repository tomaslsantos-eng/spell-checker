#include "projeto.h"

/*****************************************************************************
 * ler_dicionario()
 *
 * Argumentos:
 *      Dicionario *dic     - Ponteiro para a estrutura onde as palavras serão armazenadas.
 * 
 *      const char *arquivo - Nome do ficheiro de texto que contém as palavras do dicionário,
 *                            uma por linha.
 *
 * Retorna:
 *      void
 *
 * Descrição:
 *      Esta função lê palavras de um ficheiro de texto, preenchendo a estrutura
 *      Dicionario com todas as palavras válidas encontradas. Cada palavra deve
 *      estar numa linha separada. A função realiza duas passagens pelo ficheiro:
 *          1. Para contar quantas palavras existem e determinar o tamanho da maior.
 *          2. Para copiar as palavras para a estrutura.
 *      Após a leitura, as palavras são ordenadas alfabeticamente (ignora capitalização).
 *
 *      Em caso de erro (ficheiro inexistente, erro de leitura ou de alocação),
 *      o programa termina.
 *
 ****************************************************************************/

void ler_dicionario(Dicionario *dic, const char *arquivo) {
   
    FILE *file = fopen(arquivo, "r");
    if (!file) {
        // Encerra o programa com erro se o ficheiro não for encontrado ou não puder ser aberto
        exit(EXIT_FAILURE);
    }

    char *linha = NULL;     // Ponteiro para armazenar cada linha lida
    size_t len = 0;         // Comprimento alocado para getline
    size_t read;           // Número de caracteres lidos por getline
    int maior_size = 0;     // Guarda o tamanho da maior palavra lida

    dic->n_palavras = 0;    // Inicializa o número de palavras no dicionário como zero

    // Primeira leitura: determinar o número de palavras e o tamanho da maior
    while ((read = getline(&linha, &len, file)) != -1) {

        // Remove o caractere de nova linha ('\n') do final da linha, se existir
        if (read > 0 && linha[read-1] == '\n') {
            linha[read-1] = '\0';
            read--;
        }

        // Se a linha não estiver vazia
        if (read > 0) {
            // Atualiza o tamanho da maior palavra, se necessário
            if ((int)read > maior_size) {
                maior_size = (int)read;
            }
            dic->n_palavras++;  // Conta mais uma palavra
        }
    }

    // Se não foi lida nenhuma palavra ou o tamanho máximo é inválido, encerra com erro
    if (maior_size <= 0 || dic->n_palavras <= 0) {
        free(linha);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Reposiciona o cursor do ficheiro para o início, para fazer nova leitura
    fseek(file, 0, SEEK_SET);

    // Aloca memória para a tabela de palavras (array de strings)
    dic->tabela = malloc(sizeof(char *) * dic->n_palavras);
    if (!dic->tabela) {
        free(linha);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int i = 0;  

    // Segunda leitura: preencher a tabela com as palavras
    while ((read = getline(&linha, &len, file)) != -1) {

        // Remove o '\n' do fim da linha, se existir
        if (read > 0 && linha[read-1] == '\n') {
            linha[read-1] = '\0';
            read--;
        }

        // Se a linha contiver conteúdo
        if (read > 0) {
            
            dic->tabela[i] = strdup(linha);
            if (!dic->tabela[i]) {
                // Em caso de falha na alocação, liberta toda a memória já alocada
                for (int j = 0; j < i; j++)
                    free(dic->tabela[j]);
                free(dic->tabela);
                free(linha);
                fclose(file);
                exit(EXIT_FAILURE);
            }
            i++;  
        }
    }

    // Liberta a memória usada pela linha e fecha o ficheiro
    free(linha);
    fclose(file);

    // Ordena a tabela de palavras em ordem alfabética
    qsort(dic->tabela, dic->n_palavras, sizeof(char *), comparar_palavras);
}
