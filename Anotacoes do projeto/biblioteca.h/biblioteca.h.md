# Arquivo de Cabeçalho: figurinha.h

**Responsável:** Definição das Estruturas de Dados (Structs), Contratos de Funções (Protótipos) e Include Guards.

##  O que foi feito
Para que o sistema possua uma arquitetura modular e escalável, foi necessário criar um arquivo de cabeçalho que atua como o "contrato" do projeto. É neste arquivo que definimos o "molde" de uma figurinha e declaramos a existência de todas as funções, permitindo que a `main.c` e os outros arquivos `.c` consigam enxergar e utilizar os códigos uns dos outros sem gerar erros de compilação.

## Como funciona a engrenagem
1. **Include Guards (`#ifndef`, `#define`, `#endif`):** Todo o código do arquivo foi envelopado por essas macros de pré-processador. Isso atua como um escudo que impede o compilador de incluir este mesmo arquivo duas vezes na memória, o que causaria um erro fatal de redefinição de *structs*.
2. **Definição da Base de Dados (Structs):** - Foi construída a estrutura base do jogador, contemplando as exigências do projeto: `Código`, `Título` (Nome), `Seção` (Seleção/País), `Tipo` (Raridade), além dos campos inteiros de controle (`quantidade_para_troca`, `is_colada`).
   - Foi criada a estrutura `Album`, que contém um vetor dinâmico (ou ponteiro) para agrupar as figurinhas e uma variável inteira `quantidade_atual` para rastrear o limite do laço em tempo real.
3. **Assinaturas (Protótipos):** O arquivo lista apenas as "capas" das funções (ex: `void Sanitiza_Nome(Album *album);`). O corpo lógico dessas funções fica escondido dentro dos arquivos `.c`. Quando a `main.c` faz o `#include "album.h"`, ela recebe a planta baixa do sistema inteiro e sabe exatamente como chamar cada ferramenta.