#include "utils.h"


/*****************************************************************************
 * metodo_2 ()
 *
 * Argumentos:
 *      const char *dicionario     - Caminho para o ficheiro que contém o dicionário
 *                                   de palavras válidas (uma por linha).
 * 
 *      const char *filename_input - Caminho para o ficheiro de entrada com o texto
 *                                   a verificar. Se for uma string vazia, lê-se do stdin.
 * 
 *      const char *output_file    - Caminho para o ficheiro onde serão escritos os
 *                                   resultados. Se for uma string vazia, escreve-se no stdout.
 * 
 *      int max_alt                - Número máximo de sugestões a apresentar por palavra incorreta.
 * 
 *      int max_dif                - Diferença máxima permitida entre uma palavra incorreta
 *                                   e uma palavra do dicionário, para ser considerada sugestão.
 *
 * Retorna:
 *      void - A função não retorna valores diretamente. Os resultados são escritos
 *             no ficheiro de saída fornecido (ou stdout).
 *
 * Descrição:
 *      Esta função realiza uma verificação ortográfica linha a linha e apresenta sugestões para
 *      palavras incorretas.
 *
 *      Para cada palavra não encontrada no dicionário, é feita uma análise para gerar
 *      sugestões com base na diferença de caracteres e através da função `processar_divisoes`, 
 *      que tenta separar palavras compostas não reconhecidas.
 *
 *      As melhores sugestões são ordenadas por relevância e apresentadas até ao limite
 *      especificado por `max_alt`.
 *
 * Etapas principais:
 *      1. Abertura dos ficheiros de entrada e saída (ou uso de stdin/stdout).
 * 
 *      2. Leitura do dicionário para memória.
 * 
 *      3. Leitura das linhas do ficheiro de entrada e armazenamento numa lista ligada.
 * 
 *      4. Para cada linha:
 *          - Extração de palavras.
 *          - Verificação da existência no dicionário.
 *          - Caso uma palavra esteja errada:
 *              - Armazenar o erro.
 *              - Gerar sugestões por divisão e por diferença de caracteres.
 *              - Ordenar sugestões.
 *              - Escrever a linha original, o erro e as sugestões no ficheiro de saída.
 * 
 *      5. Libertação da memória e fecho dos ficheiros utilizados.
 *
 ****************************************************************************/




 void metodo_2(const char *dicionario, const char *filename_input, const char *output_file, int max_alt, int max_dif)
{
     FILE *in = NULL;
     FILE *out = NULL;
 
     // Abre o arquivo de entrada ou usa stdin, se não for especificado
     if (filename_input[0])
     {
         in = fopen(filename_input, "r");
         if (!in)
         {
             exit(EXIT_FAILURE);
         }
     }
     else
     {
         in = stdin;
     }
 
     // Abre o arquivo de saída ou usa stdout, se não for especificado
     if (output_file[0])
     {
         out = fopen(output_file, "w");
         if (!out)
         {
             if (in != stdin)
             {
                 fclose(in);
             }
             exit(EXIT_FAILURE);
         }
     }
     else
     {
         out = stdout;
     }
 
     // Carrega o dicionário na estrutura de dados
     Dicionario dic;
     ler_dicionario(&dic, dicionario);
 
     Linha *linhas = NULL, *ultima = NULL;
     char *buffer = NULL;
     size_t buffer_size = 0;
     size_t read;
 
     // Lê todas as linhas do arquivo de entrada
     while ((read = getline(&buffer, &buffer_size, in)) != -1)
     {
         Linha *nova = malloc(sizeof(Linha));
         if (!nova)
             continue;
 
         nova->conteudo = strdup(buffer);
         nova->proximo = NULL;
 
         // Constrói uma lista encadeada com as linhas
         if (ultima)
         {
             ultima->proximo = nova;
         }
         else
         {
             linhas = nova;
         }
         ultima = nova;
     }
     free(buffer);
 
     int num_linha = 0;
     int max_sug = dic.n_palavras + 10;
 
     // Percorre cada linha da lista
     for (Linha *l = linhas; l != NULL; l = l->proximo)
     {
         num_linha++;
         char *linha = l->conteudo;
         linha[strcspn(linha, "\n")] = '\0'; // Remove o '\n' do final da linha
 
         Erro *erros = NULL, *ultimo_erro = NULL;
 
         char *palavras[1000];
         int n_palavras = 0;
         extrair_palavras(linha, palavras, &n_palavras); // Extrai palavras da linha
 
         // Verifica se cada palavra está no dicionário
         for (int i = 0; i < n_palavras; i++)
         {
             char *palavra = palavras[i];
 
             char **found = (char **)bsearch(&palavra, dic.tabela, dic.n_palavras, sizeof(char *), comparar_palavras);
 
             // Se não encontrada, adiciona à lista de erros
             if (!found)
             {
                 Erro *novo = malloc(sizeof(Erro));
                 if (novo)
                 {
                     novo->palavra = strdup(palavra);
                     novo->proximo = NULL;
 
                     if (ultimo_erro)
                     {
                         ultimo_erro->proximo = novo;
                     }
                     else
                     {
                         erros = novo;
                     }
                     ultimo_erro = novo;
                 }
             }
 
             free(palavras[i]);
         }
 
         if (erros)
         {
             // Imprime a linha com erro
             fprintf(out, "%d: %s\n", num_linha, linha);
 
             for (Erro *e = erros; e != NULL; e = e->proximo)
             {
                 // Imprime a palavra incorreta
                 fprintf(out, "Erro na palavra \"%s\"\n", e->palavra);
 
                 // Cria vetor para armazenar sugestões
                 Sugestao **sugestoes = malloc(max_sug * sizeof(Sugestao *));
                 int n_sug = 0;
 
                 // Gera sugestões com base em divisões possíveis
                 processar_divisoes(e, &dic, sugestoes, &n_sug, max_sug, max_dif);
 
                 // Verifica palavras do dicionário com diferenças aceitáveis
                 for (int i = 0; i < dic.n_palavras; i++)
                 {
                     int dif = calcular_diferencas(e->palavra, dic.tabela[i], max_dif);
                     if (dif != INT_MAX)
                     {
                         int duplicata = 0;
                         // Verifica se a sugestão já está na lista
                         for (int j = 0; j < n_sug; j++)
                         {
                             if (strcasecmp(sugestoes[j]->palavra, dic.tabela[i]) == 0)
                             {
                                 duplicata = 1;
                                 break;
                             }
                         }
 
                         // Adiciona nova sugestão se não for duplicada
                         if (!duplicata && n_sug < max_sug)
                         {
                             Sugestao *nova = malloc(sizeof(Sugestao));
                             if (nova)
                             {
                                 nova->palavra = strdup(dic.tabela[i]);
                                 nova->diferenca = dif;
                                 nova->indice = i;
                                 sugestoes[n_sug++] = nova;
                             }
                         }
                     }
                 }
 
                 // Ordena sugestões por critérios (provavelmente diferença e ordem no dicionário)
                 qsort(sugestoes, n_sug, sizeof(Sugestao *), comparar_sugestoes);
 
                 // Imprime no máximo max_alt sugestões
                 for (int i = 0; i < n_sug; i++)
                 {
                     if (i < max_alt)
                     {
                         fprintf(out, "%s%s", (i == 0) ? "" : ", ", sugestoes[i]->palavra);
                     }
                     free(sugestoes[i]->palavra);
                     free(sugestoes[i]);
                 }
                 fprintf(out, "\n");
                 free(sugestoes);
             }
 
             // Liberta memória da lista de erros
             limpar_erros(erros);
         }
     }
 
     // Liberta estruturas auxiliares
     limpar_linhas(linhas);
     limpar_dicionario(&dic);
 
     // Fecha arquivos se não forem stdin/stdout
     if (in != stdin)
         fclose(in);
     if (out != stdout)
         fclose(out);
 }