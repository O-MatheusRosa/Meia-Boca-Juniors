# Função: Ordena_lista_Bin

**Arquivo de Origem:** `album.c`
**Responsável:** Fornecer o critério de comparação para o algoritmo `qsort`.

## ?? O que foi feito
Para que a listagem de figurinhas seja apresentada em ordem alfabética e prepare o terreno para futuras otimizações de pesquisa, foi implementada esta função auxiliar. Ela age como o "Juiz" que dita as regras de comparação para a função nativa de ordenação do C.

## ?? Como funciona a engrenagem
1. **Casting de Ponteiros:** A função recebe dois ponteiros genéricos e imutáveis (`const void *`). Internamente, converte esses endereços genéricos para o tipo específico da estrutura do projeto (`Dados_Figurinha *`).
2. **Comparação Alfabética:** Com as estruturas mapeadas, a função utiliza `stricmp()` para cruzar o campo `nome_Jogador` da figurinha A com o da figurinha B, ignorando a diferença entre letras maiúsculas e minúsculas (*case-insensitive*).
3. O retorno numérico dessa comparação é lido pelo `qsort` para reposicionar as *structs* na memória de forma ordenada.