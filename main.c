#define _POSIX_C_SOURCE 200809L
#include "projeto.h"

int main(int argc, char *argv[]) {
    int opcao;
    char *dicionario = "words";  // Valor padrão para o dicionário
    int max_alternativas = 10;   // Valor padrão para o número máximo de alternativas
    int max_diferencas = 2;      // Valor padrão para o número máximo de diferenças
    int modo = 1;                // Valor padrão para o modo de funcionamento
    char filename_input[256] = "";  // Nome do ficheiro de entrada
    char filename_output[256] = ""; // Nome do ficheiro de saída

    // Processamento das opções da linha de comando
    while ((opcao = getopt(argc, argv, "a:d:hi:m:n:o:")) != -1) {
        switch (opcao) {
            case 'a':
                max_alternativas = atoi(optarg);
                if (max_alternativas <= 0) {
                   
                    exit(EXIT_FAILURE);
                }
                break;
            case 'd':
                dicionario = optarg;
                break;
            case 'h':
                printf(
                    "O programa deve ser utilizado da seguinte forma:\n"
                    "Uso: ./ortografia [OPTIONS]\n"
                    "Onde as opções disponíveis são: \n"
                    "  -h        Mostra a ajuda e termina\n"
                    "  -i FILE   Define o ficheiro de entrada (padrão: stdin)\n"
                    "  -o FILE   Define o ficheiro de saída (padrão: stdout)\n"
                    "  -d FILE   Define o ficheiro de dicionário a usar\n"
                    "  -a NN     Define o nº máx. de alternativas por erro ortográfico\n"
                    "  -n NN     Define o nº máx. de diferenças a considerar nos erros\n"
                    "  -m NN     Define o modo de funcionamento do programa\n"
                    "É importante não se esquecer do traço '-'\n"
                );
                return 0;
            case 'i':
                strcpy(filename_input, optarg);
                break;
            case 'm':
                modo = atoi(optarg);
                if (modo != 1 && modo != 2 && modo !=3) {
                    exit(EXIT_FAILURE);
                }
                break;
            case 'n':
                max_diferencas = atoi(optarg);
                if (max_diferencas <= 0) {
                   
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                strcpy(filename_output, optarg);
                break;
            default:
                
                exit(EXIT_FAILURE);
        }
    }


    // Chama o método conforme o modo selecionado:
    if (modo == 1) {
        metodo_1(dicionario, filename_input, filename_output);
    } else if (modo == 2) {
        metodo_2(dicionario, filename_input, filename_output, max_alternativas, max_diferencas);
    } else if (modo == 3) {
        metodo_3(dicionario, filename_input, filename_output, max_diferencas);
    } else {
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}