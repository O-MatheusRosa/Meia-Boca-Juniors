# Função: Procura_Jogador

**Arquivo de Origem:** `album.c`
**Responsável:** Varredura e localização de figurinhas específicas.

## O que foi feito
Implementação do sistema de pesquisa (Opção 3 do menu). Permite a busca exata de um jogador pelo nome, devolvendo a sua localização no vetor de memória, o que é essencial tanto para visualização quanto para futuras mecânicas de exclusão ou alteração de dados.

## Como funciona a engrenagem
1. **Sanitização do Input:** A função recebe a `string` capturada do usuário. Antes de qualquer verificação, o comando `strcspn` localiza e elimina a quebra de linha (`\r\n`) gerada pela tecla ENTER durante a digitação.
2. **Iteração de Busca:** Um laço `for` percorre as posições ativas do álbum (`quantidade_atual`).
3. **Condição de Parada:** A função `stricmp` cruza o nome limpo digitado com o nome já sanitizado do banco de dados. Se a correspondência for exata (retornando `0`), a função devolve o índice `i` (posição do jogador no vetor).
4. **Retorno de Falha:** Se o laço inteiro terminar sem encontrar correspondência, a função retorna `-1`, indicando que o jogador não existe no fichário.