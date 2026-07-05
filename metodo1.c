#include "projeto.h"

/*****************************************************************************
 * metodo_1 ()
 *
 * Argumentos:
 *      const char *dicionario      - Caminho para o ficheiro que contém o dicionário
 *                                    de palavras válidas (uma por linha).
 * 
 *      const char *filename_input  - Caminho para o ficheiro de entrada a verificar.
 *                                    Se for uma string vazia, lê-se do stdin.
 * 
 *      const char *filename_output - Caminho para o ficheiro de saída onde serão
 *                                    escritos os erros encontrados. Se for uma
 *                                    string vazia, escreve-se no stdout.
 *
 * Retorna:
 *      void - A função não retorna valor diretamente. A saída é feita por
 *             ficheiro/output fornecido.
 *
 * Descrição:
 *      Esta função verifica ortograficamente o conteúdo de um ficheiro linha a linha.
 *      Para cada palavra em cada linha, é feita uma pesquisa binária no dicionário
 *      previamente carregado para verificar a sua validade.
 *
 *      Caso sejam encontradas palavras não existentes no dicionário, essas são
 *      registadas como erros. A linha original e as palavras incorretas são
 *      então escritas no ficheiro de saída.
 *
 *      O processo é realizado em várias etapas:
 *          1. Carregamento do dicionário para memória.
 * 
 *          2. Abertura dos ficheiros de entrada e saída (ou uso de stdin/stdout).
 * 
 *          3. Leitura do ficheiro de entrada e armazenamento das linhas numa
 *             lista ligada.
 *          4. Processamento de cada linha:
 *              - Extração das palavras.
 *              - Verificação no dicionário.
 *              - Registo de erros encontrados.
 * 
 *          5. Escrita dos erros no ficheiro de saída.
 * 
 *          6. Libertação de toda a memória alocada e fecho dos ficheiros.
 *
 ****************************************************************************/

 void metodo_1(const char *dicionario, const char *filename_input, const char *filename_output) {
    Dicionario dic;
    // Carrega o dicionário na memória
    ler_dicionario(&dic, dicionario);
    
    // Abre o arquivo de entrada, ou usa a entrada padrão se não especificado
    FILE *input_file = stdin;
    if (filename_input[0] != '\0') {
        input_file = fopen(filename_input, "r");
        if (!input_file) {
            limpar_dicionario(&dic);
            exit(EXIT_FAILURE);
        }
    }
    
    // Abre o arquivo de saída, ou usa a saída padrão se não especificado
    FILE *output_file = stdout;
    if (filename_output[0] != '\0') {
        output_file = fopen(filename_output, "w");
        if (!output_file) {
            if (input_file != stdin) fclose(input_file);
            limpar_dicionario(&dic);
            exit(EXIT_FAILURE);
        }
    }
    
    // Lê todas as linhas do arquivo de entrada e armazena numa lista ligada
    Linha *linhas = NULL, *ultima = NULL;
    char *linha = NULL;
    size_t len = 0;
    while (getline(&linha, &len, input_file) != -1) {
        linha[strcspn(linha, "\n")] = '\0';  // Remove o '\n' do final da linha

        Linha *nova = malloc(sizeof(Linha));
        nova->conteudo = strdup(linha); // Copia o conteúdo da linha
        nova->proximo = NULL;

        if (ultima) ultima->proximo = nova; // Adiciona à lista
        else linhas = nova;
        ultima = nova;
    }
    free(linha); // Liberta o buffer da getline

    int linha_atual = 1;

    // Processa cada linha armazenada
    for (Linha *l = linhas; l != NULL; l = l->proximo, linha_atual++) {
        Erro *erros = NULL, *ultimo_erro = NULL;
        int encontrou_erro = 0;

        // Extrai palavras da linha atual
        char *palavras[1000]; 
        int n_palavras = 0;
        extrair_palavras(l->conteudo, palavras, &n_palavras);

        // Verifica cada palavra na linha
        for (int i = 0; i < n_palavras; i++) {
            // Busca a palavra no dicionário usando busca binária
            char **found = (char **)bsearch(&palavras[i], dic.tabela, dic.n_palavras, sizeof(char *), comparar_palavras);
            
            if (found == NULL) {
                // Palavra não encontrada: adiciona à lista de erros
                Erro *novo_erro = malloc(sizeof(Erro));
                novo_erro->palavra = strdup(palavras[i]);
                novo_erro->proximo = NULL;

                if (ultimo_erro) ultimo_erro->proximo = novo_erro;
                else erros = novo_erro;
                ultimo_erro = novo_erro;

                encontrou_erro = 1;
            }

            free(palavras[i]); // Liberta a memória da palavra extraída
        }

        // Se houver erros na linha, imprime a linha e os erros
        if (encontrou_erro) {
            fprintf(output_file, "%d: %s\n", linha_atual, l->conteudo);
            for (Erro *e = erros; e != NULL; e = e->proximo) {
                fprintf(output_file, "Erro na palavra \"%s\"\n", e->palavra);
            }
        }

        limpar_erros(erros); // Liberta memória dos erros encontrados
    }

    // Liberta recursos utilizados
    limpar_dicionario(&dic);
    limpar_linhas(linhas);
    if (input_file != stdin) fclose(input_file);
    if (output_file != stdout) fclose(output_file);
}
