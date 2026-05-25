## Commit do dia 19/05, parte da manha e da tarde

#### Matheus

# Função: main (Menu Principal)

**Arquivo de Origem:** `main.c`
**Responsável:** Ponto de ignição do programa e interface de navegação do usuário.

## O que foi feito
A estrutura base do sistema foi construída para atuar como o cérebro do projeto. Precisávamos de um loop infinito controlado que mantivesse o programa rodando até o usuário decidir sair, além de gerenciar a inicialização dos dados antes de qualquer interação.

## Como funciona a engrenagem
1. **Ponto de Ignição (Boot):** Antes de exibir qualquer coisa na tela, a `main` tenta carregar o arquivo binário (`album.bin`). Se ele não existir (primeira execução), ela aciona a função de leitura do `.CSV` original para popular a memória RAM.
2. **Limpeza e Ordenação:** Em seguida, aciona as funções `Sanitiza_Nome` e `qsort` para limpar sujeiras de memória e organizar as figurinhas alfabeticamente.
3. **Loop de Navegação:** Utiliza a estrutura `do-while` envolvendo um `switch(opcao)`. O menu apresenta as opções (Listar, Abrir Pacote, Pesquisar, etc.) e capta a escolha do usuário.
4. **Encerramento Seguro:** Quando o usuário digita `0`, o laço é quebrado e o sistema obrigatoriamente chama a função que salva o estado atual da memória RAM de volta no disco rígido (`album.bin`), garantindo a persistência dos dados.


