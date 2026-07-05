# Corretor Ortográfico em C

Programa em linguagem C que verifica a ortografia de um texto, sugere palavras alternativas para erros ortográficos e corrige automaticamente um texto com base num dicionário.

Projeto desenvolvido no âmbito da Unidade Curricular de **Programação**, do curso de **LEEC**, no Instituto Superior Técnico (ano letivo 2024/2025).

## Funcionalidades

O programa suporta três modos de funcionamento:

- **Modo 1 — Deteção de erros**: lê o texto de entrada linha a linha e identifica as palavras que não constam no dicionário, assinalando a linha e cada palavra errada encontrada.
- **Modo 2 — Sugestão de alternativas**: além de detetar os erros, sugere até um número máximo de palavras alternativas do dicionário, ordenadas pelo menor número de diferenças em relação à palavra errada.
- **Modo 3 — Correção automática**: corrige automaticamente cada palavra errada, substituindo-a pela primeira alternativa encontrada (segundo o mesmo critério do modo 2), e escreve o texto corrigido no output.

O dicionário é lido de um ficheiro de texto (uma palavra por linha) e ordenado alfabeticamente em memória, para permitir pesquisa binária rápida durante a verificação.

## Compilação

O projeto inclui um `Makefile`. Para compilar, basta correr:

```bash
make
```

Isto gera o executável `ortografia` na pasta do projeto. As flags de compilação usadas são `-Wall -O3 -g`, conforme especificado no enunciado do projeto.

Para limpar os ficheiros compilados:

```bash
make clean
```

## Utilização

```bash
./ortografia [OPÇÕES]
```

### Opções disponíveis

| Opção | Descrição | Valor por omissão |
|---|---|---|
| `-h` | Mostra a mensagem de ajuda e termina | — |
| `-i FILE` | Ficheiro de entrada (em alternativa a `stdin`) | `stdin` |
| `-o FILE` | Ficheiro de saída (em alternativa a `stdout`) | `stdout` |
| `-d FILE` | Ficheiro de dicionário a usar | `words` |
| `-a NN` | Número máximo de alternativas a apresentar por erro | `10` |
| `-n NN` | Número máximo de diferenças a considerar entre palavras | `2` |
| `-m NN` | Modo de funcionamento (`1`, `2` ou `3`) | `1` |

### Exemplos

```bash
# Ajuda
./ortografia -h

# Modo 1: deteta erros lendo do stdin, escrevendo no stdout
./ortografia -m 1 -d words

# Modo 2: sugere até 5 alternativas por erro, com no máximo 2 diferenças
./ortografia -m 2 -a 5 -n 2 -d words -i texto.txt

# Modo 3: corrige automaticamente um ficheiro e grava o resultado noutro
./ortografia -m 3 -d words -i texto.txt -o texto_corrigido.txt
```

O programa termina com código `EXIT_SUCCESS` numa execução bem-sucedida, e `EXIT_FAILURE` caso os parâmetros sejam inválidos.

### Dicionários

O programa foi testado com os dicionários `words` (inglês) e `portuguese`, normalmente disponíveis em `/usr/share/dict/` em sistemas Linux, mas aceita qualquer ficheiro de texto com uma palavra por linha.

## Estrutura do código

| Ficheiro | Responsabilidade |
|---|---|
| `main.c` | Ponto de entrada; interpretação dos argumentos da linha de comando (`getopt`) e despacho para o modo de funcionamento escolhido |
| `projeto.h` | Estruturas de dados principais (`Dicionario`, `Erro`, `Sugestao`, `Linha`) e declarações de funções partilhadas |
| `leitura_e_armazenamento.c` | Leitura do ficheiro de dicionário para memória e ordenação alfabética |
| `limpeza.c` | Libertação de memória alocada dinamicamente (dicionário, listas de erros, listas de linhas) |
| `metodo1.c` | Implementação do modo 1 — deteção de erros ortográficos |
| `metodo2.c` | Implementação do modo 2 — sugestão de palavras alternativas |
| `metodo3.c` | Implementação do modo 3 — correção automática do texto |
| `auxialiares.c` | Funções auxiliares de comparação e extração de palavras de uma linha de texto |
| `utils.c` / `utils.h` | Cálculo do número de diferenças entre duas palavras e funções de apoio à geração de sugestões |

## Detalhes de implementação

- O dicionário é lido para um vetor alocado dinamicamente e ordenado com `qsort()`, usando `strcasecmp()` para ignorar maiúsculas/minúsculas.
- A pesquisa de palavras no dicionário é feita por pesquisa binária, através de `bsearch()`.
- O cálculo de diferenças entre uma palavra errada e uma palavra do dicionário segue um algoritmo de comparação com deslocamento (*offset*), capaz de detetar substituições, inserções, remoções de caracteres, e ainda a divisão de uma palavra em duas palavras válidas do dicionário.
- Toda a memória alocada dinamicamente é libertada antes do término do programa.

## Autor

Projeto desenvolvido para a disciplina de Programação (LEEC, IST) — trabalho de grupo, 2024/2025.
