# Função: Excluir_Figurinha

**Arquivo de Origem:** album.c
**Responsável:** Remover uma figurinha da memória sem deixar "buracos" no vetor dinâmico.

## O que foi feito
Construímos a técnica de compressão de array (o famoso "chega pra lá") para deletar um registro com segurança.

## Como funciona a engrenagem
1. A Busca: Encontra o índice (posição) da carta que precisa ser apagada.
2. O Esmagamento: Um laço for começa na posição encontrada e vai até a penúltima carta. A lógica figurinhas[i] = figurinhas[i + 1] faz com que toda a fila ande um passo para trás, esmagando a carta excluída.
3. O Encolhimento: Dá um quantidade_atual-- para oficializar que o vetor diminuiu e a última gaveta (agora duplicada) fique inacessível.