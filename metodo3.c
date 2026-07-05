#include "utils.h"


/*****************************************************************************
 * is_word_char_contextual ()
 *
 * Argumentos:
 *      const char *linha - Ponteiro para a linha de texto atual.
 *      size_t len        - Comprimento total da linha.
 *      size_t i          - Índice do carácter a ser analisado.
 *
 * Retorna:
 *      int - Retorna 1 (verdadeiro) se o carácter em questão fizer parte de uma
 *            palavra válida no contexto, ou 0 (falso) caso contrário.
 *
 * Descrição:
 *      Esta função verifica se um carácter na posição `i` de uma linha de texto
 *      pode ser considerado como parte de uma palavra. Considera letras (A-Z, a-z)
 *      como válidas por si só, e também permite certos caracteres especiais como
 *      apóstrofos (') desde que estejam rodeados por letras.
 *
 *      Esta abordagem permite que o programa identifique corretamente palavras que
 *      incluem apóstrofos no meio, mas rejeita casos inválidos como apóstrofos isolados
 *      ou mal posicionados.
 *
 ****************************************************************************/

 static int is_word_char_contextual(const char *linha, size_t len, size_t i) {
     char c = linha[i];
     if (isalpha(c)) return 1;
     if (c == '\'' && i > 0 && i + 1 < len) {
         return isalpha(linha[i - 1]) && isalpha(linha[i + 1]);
     }
     return 0;
 }

/*****************************************************************************
 * metodo_3 ()
 *
 * Argumentos:
 *      const char *dicionario   - Caminho para o arquivo contendo o dicionário.
 * 
 *      const char *filename_input - Caminho para o arquivo de entrada com o texto a ser processado.
 * 
 *      const char *output_file   - Caminho para o arquivo onde o texto corrigido será salvo.
 * 
 *      int max_dif               - Valor máximo de diferença permitida para sugestões de palavras.
 *
 * Descrição:
 *      A função `metodo_3` realiza a leitura de um texto de um arquivo de entrada
 *      (ou stdin), e processa o texto linha por linha.Para cada palavra encontrada, 
 *      ela verifica se a palavra existe no dicionário e, caso não exista, sugere 
 *      correções com base em uma lista de palavras do dicionário.
 *      As sugestões são geradas com base em um cálculo de diferença de caracteres
 *      entre a palavra incorreta e as palavras do dicionário, e a palavra incorreta é substituída
 *      pela sugestão mais adequada, caso existam sugestões válidas.
 *      Após o processamento, o texto corrigido é gravado no arquivo de saída (ou stdout, caso o caminho seja vazio).
 *
 * Etapas principais:
 *      1. Abertura dos arquivos de entrada e saída, utilizando stdin e stdout como standard.
 * 
 *      2. Leitura do dicionário para memória, utilizando a função `ler_dicionario`.
 * 
 *      3. Leitura e processamento de cada linha de texto. Para cada linha:
 *         a. Tokenização e identificação das palavras.
 *         b. Verificação de cada palavra no dicionário, utilizando `bsearch` para busca binária.
 *         c. Caso a palavra não seja encontrada, gera sugestões de palavras utilizando a função `processar_divisoes`.
 *         d. As sugestões são ordenadas pela menor diferença de caracteres e a palavra incorreta é substituída pela melhor sugestão.
 * 
 *      4. A linha corrigida é gravada no arquivo de saída.
 * 
 *      5. Após o processamento de todas as linhas, os arquivos são fechados, e a memória alocada é liberada.
 *
 *****************************************************************************/

 void metodo_3(const char *dicionario, const char *filename_input, const char *output_file, int max_dif) {
    // Abertura dos ficheiros de entrada e saída. Caso sejam strings vazias, usa stdin e stdout
    FILE *in = filename_input[0] ? fopen(filename_input, "r") : stdin;
    FILE *out = output_file[0] ? fopen(output_file, "w") : stdout;

    // Verificação de erros na abertura dos ficheiros
    if (!in || !out) {
        if (in) fclose(in);
        if (out && out != stdout) fclose(out);
        fprintf(stderr, "Erro ao abrir arquivos\n");
        exit(EXIT_FAILURE);
    }

    // Leitura do dicionário para memória
    Dicionario dic;
    ler_dicionario(&dic, dicionario);

    // Variáveis para leitura do texto
    char *buffer = NULL;
    size_t buffer_size = 0;
    size_t read;

    // Leitura do texto linha por linha
    while ((read = getline(&buffer, &buffer_size, in)) != -1) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remover o '\n' no final da linha

        // Duplicar a linha para manipulação
        char *linha_corrigida = strdup(buffer);
        if (!linha_corrigida) {
            fprintf(stderr, "Erro de alocação de memória\n");
            exit(EXIT_FAILURE);
        }

        // Processamento de cada palavra na linha
        size_t linha_len = strlen(linha_corrigida);
        size_t pos = 0;

        // Tokenização e verificação das palavras na linha
        while (pos < linha_len) {
            // Pula caracteres que não fazem parte de palavras
            while (pos < linha_len && !isalpha(linha_corrigida[pos])) {
                pos++;
            }

            // Se atingimos o final da linha, terminamos a iteração
            if (pos >= linha_len) break;

            size_t inicio_palavra = pos;

            // Continua a percorrer até encontrar o fim da palavra
            while (pos < linha_len && is_word_char_contextual(linha_corrigida, linha_len, pos)) {
                pos++;
            }

            size_t fim_palavra = pos;
            size_t tamanho_palavra = fim_palavra - inicio_palavra;

            // Criação de uma nova string para a palavra
            char *palavra = malloc(tamanho_palavra + 1);
            if (!palavra) {
                fprintf(stderr, "Erro de alocação de memória\n");
                exit(EXIT_FAILURE);
            }

            // Copia a palavra para a memória
            strncpy(palavra, linha_corrigida + inicio_palavra, tamanho_palavra);
            palavra[tamanho_palavra] = '\0';

            // Verifica se a palavra está no dicionário
            void *found = bsearch(&palavra, dic.tabela, dic.n_palavras, sizeof(char *), comparar_palavras);

            // Se a palavra não for encontrada no dicionário
            if (!found) {
                // Alocação de memória para sugestões
                Sugestao **sugestoes = malloc((dic.n_palavras + 10) * sizeof(Sugestao *));
                int n_sug = 0;

                if (!sugestoes) {
                    fprintf(stderr, "Erro de alocação de memória\n");
                    exit(EXIT_FAILURE);
                }

                // Geração de sugestões com base nas divisões e diferenças
                processar_divisoes(&(Erro){.palavra = palavra}, &dic, sugestoes, &n_sug, dic.n_palavras + 10, max_dif);

                // Calcula as diferenças entre a palavra incorreta e as palavras do dicionário
                for (int i = 0; i < dic.n_palavras; i++) {
                    int dif = calcular_diferencas(palavra, dic.tabela[i], max_dif);
                    if (dif != INT_MAX) {
                        // Criação de uma nova sugestão com base na diferença
                        Sugestao *nova = malloc(sizeof(Sugestao));
                        if (!nova) {
                            fprintf(stderr, "Erro de alocação de memória\n");
                            exit(EXIT_FAILURE);
                        }
                        nova->palavra = strdup(dic.tabela[i]);
                        nova->diferenca = dif;
                        nova->indice = i;
                        sugestoes[n_sug++] = nova;
                    }
                }

                // Se existirem sugestões válidas
                if (n_sug > 0) {
                    // Ordena as sugestões pela menor diferença
                    qsort(sugestoes, n_sug, sizeof(Sugestao *), comparar_sugestoes);

                    // Substitui a palavra errada pela melhor sugestão
                    char *melhor_palavra = sugestoes[0]->palavra;

                    // Substituição da palavra na linha
                    char *nova_linha = substituir_palavra(linha_corrigida, inicio_palavra, tamanho_palavra, melhor_palavra);

                    // Atualiza o tamanho da linha com a substituição
                    int delta_tamanho = strlen(melhor_palavra) - tamanho_palavra;
                    linha_len += delta_tamanho;
                    pos = inicio_palavra + strlen(melhor_palavra);

                    // Libertação da memória da linha anterior e atualização
                    free(linha_corrigida);
                    linha_corrigida = nova_linha;
                }

                // Libertação das sugestões geradas
                for (int i = 0; i < n_sug; i++) {
                    free(sugestoes[i]->palavra);
                    free(sugestoes[i]);
                }
                free(sugestoes);
            }

            // Liberta a memória da palavra processada
            free(palavra);
        }

        // Escreve a linha corrigida no ficheiro de saída
        fprintf(out, "%s\n", linha_corrigida);
        free(linha_corrigida);
    }

    // Liberta a memória do buffer e do dicionário
    free(buffer);
    limpar_dicionario(&dic);

    // Fecha os ficheiros de entrada e saída, se necessário
    if (in != stdin) fclose(in);
    if (out != stdout) fclose(out);
}


