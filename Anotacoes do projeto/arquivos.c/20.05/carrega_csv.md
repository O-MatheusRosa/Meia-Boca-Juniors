# Função: Carregar_CSV (Leitura de Dados Iniciais)

### Rosa

**Arquivo de Origem:** `arquivos.c`
**Responsável:** Popular o sistema na primeira execução do programa.

## O que foi feito
O sistema precisa de uma base de dados inicial fornecida por um arquivo de texto. Esta função foi desenvolvida para extrair os dados brutos de um `.CSV`, quebrar as informações separadas por vírgula e injetar tudo dentro da nossa struct dinamicamente na memória RAM.

## Como funciona a engrenagem
1. O ponteiro `FILE *` abre o arquivo `.CSV` em modo de leitura (`"r"`).
2. Um laço `while` utiliza `fgets` para ler o arquivo linha por linha.
3. A função `strtok` (String Token) atua como um "facão", fatiando a linha inteira toda vez que encontra uma vírgula (`,`).
4. Cada fatia é copiada (com `strcpy`) para o respectivo campo da    `struct` no vetor do álbum (Código, Título, Seção, Tipo, etc.).
5. O contador `quantidade_atual` é incrementado. Ao final, o arquivo é fechado com `fclose`.