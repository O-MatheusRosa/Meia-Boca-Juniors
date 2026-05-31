## Commit do dia 31/05, domingão

#### Matheus

# Função: Abre_Pacotinho e Animacao_Loading

**Arquivos de Origem:** album.c e utilitarios.c
**Responsável:** Simular a abertura de pacotes, verificar repetidas e interagir com o usuário em tempo real.

## O que foi feito
A função principal do jogo foi totalmente refatorada para cumprir a regra de negócio exigida: agora o pacote é interativo, pergunta se o usuário quer colar a carta na hora e possui uma animação de carregamento (loading) antes de revelar as figurinhas.

## Como funciona a engrenagem
1. Animação Inicial: Chama a Animacao_Loading, que usa o Carriage Return (\r) e delays para criar uma barra de progresso no terminal.
2. O Sorteio e a Busca: Roda um for de 7 posições. Sorteia um índice do catálogo geral com rand() e usa o Procura_Jogador para checar se a carta já existe no fichário do usuário.
3. Bifurcação (Repetida vs Inédita): 
   - Se for repetida, apenas incrementa a variável quantidade_repetidas.
   - Se for inédita, aciona o escudo de memória (realloc) para dobrar a capacidade do vetor caso esteja cheio, copia os dados da carta e zera as flags.
4. Decisão em Tempo Real: Pausa o pacote e pergunta: "Deseja colar no álbum? (1-sim/0-nao)". Lê a resposta com scanf e já crava a variável colada = 1 se o usuário quiser, oficializando o crescimento do álbum com quantidade_atual++ logo em seguida.