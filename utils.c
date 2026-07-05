
#include "utils.h"



/*****************************************************************************
 * calcular_diferencas()
 *
 * Argumentos:
 *      const char *palavra_errada - Palavra com erro ortográfico a ser comparada.
 * 
 *      const char *palavra_dic    - Palavra do dicionário para comparar com a palavra errada.
 * 
 *      int max_dif                - Número máximo de diferenças permitidas.
 *
 * Retorna:
 *      int - Número mínimo de diferenças encontradas entre as duas palavras,
 *            se for menor ou igual a max_dif. Caso contrário, devolve INT_MAX.
 *
 * Descrição:
 *      Esta função compara duas palavras (ignorando maiúsculas/minúsculas) e tenta
 *      encontrar a menor quantidade de alterações necessárias para transformar a
 *      palavra_errada na palavra_dic, sem ultrapassar o limite max_dif.
 *      
 *      A função testa vários cenários:
 *          - Substituição de caracteres.
 *          - Inserções e remoções (offsets).
 *          - Diferenças acumuladas no fim da palavra.
 * 
 *      Se alguma dessas abordagens resultar numa diferença igual ao alvo (entre 1 e max_dif),
 *      o valor é imediatamente devolvido.
 *
 ****************************************************************************/


int calcular_diferencas(const char *palavra_errada, const char *palavra_dic, int max_dif)
{
    // Obter o comprimento das duas strings
    int len_errada = strlen(palavra_errada);
    int len_dic = strlen(palavra_dic);

    // Tentamos encontrar exatamente a diferença alvo entre 1 e max_dif
    for (int dif_alvo = 1; dif_alvo <= max_dif; dif_alvo++)
    {
        
        int i = 0; 
        int j = 0; 

        // Posição da primeira diferença entre as palavras
        int primeira_diferenca = -1;

        // Procurar a primeira diferença entre as duas palavras
        while (i < len_errada && j < len_dic)
        {
            // Comparação case-insensitive
            if (tolower(palavra_errada[i]) != tolower(palavra_dic[j]))
            {
                primeira_diferenca = i; // Guardamos a posição da primeira diferença
                break;
            }
            i++;
            j++;
        }

        // Se não encontramos diferenças nos caracteres comparados
        if (primeira_diferenca == -1)
        {
            // Se ambas as strings terminaram ao mesmo tempo, são iguais
            if (i == len_errada && j == len_dic)
            {
                return 0; // Palavras idênticas, diferença = 0
            }
            // Se a palavra_errada terminou mas a palavra_dic não
            else if (i == len_errada)
            {
                // A diferença é o número de caracteres restantes em palavra_dic
                int dif_restante = len_dic - j;

                // Se essa diferença for exatamente o que estamos procurando, retornamos imediatamente
                if (dif_restante == dif_alvo)
                    return dif_alvo;
            }
            // Se a palavra_dic terminou mas a palavra_errada não
            else if (j == len_dic)
            {
                // A diferença é o número de caracteres restantes em palavra_errada
                int dif_restante = len_errada - i;

                // Se essa diferença for exatamente o que estamos procurando, retornamos imediatamente
                if (dif_restante == dif_alvo)
                    return dif_alvo;
            }
        }
        // Se encontramos uma diferença nos caracteres
        else
        {
            // Tentativa 1: Offset no dicionário
            // Tentamos Saltar caracteres na palavra do dicionário para ver se conseguimos alinhar melhor com a palavra errada
            int offset = dif_alvo;
            int temp_i = primeira_diferenca;
            int temp_j = primeira_diferenca + offset; // Saltamos 'offset' caracteres na palavra do dicionário
            int dif_offset_dic = offset;              // Começamos com diferença = offset 

            // Verificamos se ainda estamos dentro dos limites da palavra do dicionário
            if (temp_j <= len_dic)
            {
                // Continuamos a comparar os caracteres após o offset
                while (temp_i < len_errada && temp_j < len_dic)
                {
                    if (tolower(palavra_errada[temp_i]) != tolower(palavra_dic[temp_j]))
                        dif_offset_dic++; // Incrementa a diferença se os caracteres não corresponderem
                    temp_i++;
                    temp_j++;
                }

                // Adiciona qualquer caráter restante em qualquer uma das strings
                if (temp_i < len_errada)
                    dif_offset_dic += len_errada - temp_i;
                if (temp_j < len_dic)
                    dif_offset_dic += len_dic - temp_j;

                // Se a diferença for exatamente o que estamos procurando, retornamos
                if (dif_offset_dic == dif_alvo)
                    return dif_alvo;
            }

            // Tentativa 2: Offset na palavra errada
            // Agora tentamos o oposto: Saltar caracteres na palavra errada
            temp_i = primeira_diferenca + offset; // Saltamos 'offset' caracteres na palavra errada
            temp_j = primeira_diferenca;
            int dif_offset_errada = offset; // Começamos com diferença = offset

            // Verificamos se ainda estamos dentro dos limites da palavra errada
            if (temp_i <= len_errada)
            {
                // Continuamos comparando caracteres após o offset
                while (temp_i < len_errada && temp_j < len_dic)
                {
                    if (tolower(palavra_errada[temp_i]) != tolower(palavra_dic[temp_j]))
                        dif_offset_errada++; // Incrementa a diferença se os caracteres não corresponderem
                    temp_i++;
                    temp_j++;
                }

                // Adiciona qualquer caráter restante em qualquer uma das strings
                if (temp_i < len_errada)
                    dif_offset_errada += len_errada - temp_i;
                if (temp_j < len_dic)
                    dif_offset_errada += len_dic - temp_j;

               
                if (dif_offset_errada == dif_alvo)
                    return dif_alvo;
            }

         
            int dif_sem_offset = 0;
            i = 0;
            j = 0;

            while (i < len_errada && j < len_dic)
            {
                if (tolower(palavra_errada[i]) != tolower(palavra_dic[j]))
                    dif_sem_offset++; 
                i++;
                j++;
            }

           
            if (i < len_errada)
                dif_sem_offset += len_errada - i;
            if (j < len_dic)
                dif_sem_offset += len_dic - j;

            
            if (dif_sem_offset == dif_alvo)
                return dif_alvo;
        }

      
        int dif_fim = 0;
        i = len_errada - 1; 
        j = len_dic - 1;    

      
        while (i >= 0 && j >= 0)
        {
            if (tolower(palavra_errada[i]) != tolower(palavra_dic[j]))
                dif_fim++; 
            i--;
            j--;
        }

       
        if (i >= 0)
            dif_fim += i + 1;
        if (j >= 0)
            dif_fim += j + 1;

  
        if (dif_fim == dif_alvo)
            return dif_alvo;
    }

    
    return INT_MAX;
}

/*****************************************************************************
 * processar_divisoes()
 *
 * Argumentos:
 *      Erro *erro          - Estrutura contendo a palavra com erro ortográfico.
 * 
 *      Dicionario *dic     - Ponteiro para o dicionário onde serão feitas as pesquisas.
 * 
 *      Sugestao **sugestoes - Array de ponteiros onde serão guardadas as sugestões válidas.
 * 
 *      int *n_sug          - Ponteiro para o número atual de sugestões (é atualizado).
 * 
 *      int max_sug         - Número máximo de sugestões a armazenar.
 * 
 *      int max_dif         - Diferença máxima permitida entre a palavra com erro e uma sugestão.
 *
 * Retorna:
 *      void
 *
 * Descrição:
 *      Esta função tenta dividir uma palavra com erro em duas partes e verifica se ambas
 *      existem no dicionário. Se sim, junta as duas partes numa sugestão (separadas por espaço)
 *      e calcula a diferença em relação à palavra original.
 *
 *      Se a sugestão estiver dentro dos limites de diferença e ainda não estiver na lista,
 *      é adicionada ao array de sugestões. No final, as sugestões são ordenadas com base
 *      na diferença e na ordem de ocorrência.
 *
 ****************************************************************************/

void processar_divisoes(Erro *erro, Dicionario *dic, Sugestao **sugestoes, int *n_sug, int max_sug, int max_dif)
{
    size_t len = strlen(erro->palavra);

    // Tenta dividir a palavra em duas, em todas as posições possíveis
    for (size_t split_pos = 1; split_pos < len; split_pos++)
    {
        // Aloca espaço para as duas partes
        char *parte1 = malloc(split_pos + 1);
        if (!parte1) continue;

        char *parte2 = malloc(len - split_pos + 1);
        if (!parte2)
        {
            free(parte1);
            continue;
        }

        // Copia os segmentos da palavra original para parte1 e parte2
        strncpy(parte1, erro->palavra, split_pos);
        parte1[split_pos] = '\0';
        strcpy(parte2, erro->palavra + split_pos);

        // Verifica se ambas as partes existem no dicionário
        int idx_p1 = procurarIndice(parte1, dic);
        int idx_p2 = procurarIndice(parte2, dic);

        if (idx_p1 != -1 && idx_p2 != -1)
        {
            // Calcula o tamanho necessário para a string combinada 
            size_t combinacao_len = strlen(dic->tabela[idx_p1]) + 1 + strlen(dic->tabela[idx_p2]) + 1;
            char *combinacao = malloc(combinacao_len);

            if (combinacao)
            {
                // Junta as palavras com um espaço
                snprintf(combinacao, combinacao_len, "%s %s", dic->tabela[idx_p1], dic->tabela[idx_p2]);

                // Calcula a diferença entre a palavra original e a combinação
                int dif = calcular_diferencas(erro->palavra, combinacao, max_dif);

                // Se for aceitável e ainda não existir nas sugestões
                if (dif <= max_dif)
                {
                    int duplicata = 0;
                    for (int i = 0; i < *n_sug; i++)
                    {
                        if (strcasecmp(sugestoes[i]->palavra, combinacao) == 0)
                        {
                            duplicata = 1;
                            break;
                        }
                    }

                    // Adiciona à lista de sugestões
                    if (!duplicata && *n_sug < max_sug)
                    {
                        Sugestao *nova = malloc(sizeof(Sugestao));
                        if (nova)
                        {
                            nova->palavra = strdup(combinacao);
                            nova->diferenca = dif;
                            nova->indice = idx_p1;
                            sugestoes[(*n_sug)++] = nova;
                        }
                    }
                }

                free(combinacao);
            }
        }

        free(parte1);
        free(parte2);
    }

    // Ordena as sugestões pela melhor diferença e ordem de ocorrência
    qsort(sugestoes, *n_sug, sizeof(Sugestao *), comparar_sugestoes);
}


// Função para substituir uma palavra em uma posição específica da string
char* substituir_palavra(char *str, size_t pos, size_t len, const char *nova_palavra) {
    size_t str_len = strlen(str);
    size_t nova_len = strlen(nova_palavra);
    size_t nova_str_len = str_len - len + nova_len;
    
    char *nova_str = malloc(nova_str_len + 1);
    if (!nova_str) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(EXIT_FAILURE);
    }
    
    // Copia a parte anterior à palavra
    strncpy(nova_str, str, pos);
    
    // Copia a nova palavra
    strcpy(nova_str + pos, nova_palavra);
    
    // Copia o restante da string original
    strcpy(nova_str + pos + nova_len, str + pos + len);
    
    return nova_str;
}