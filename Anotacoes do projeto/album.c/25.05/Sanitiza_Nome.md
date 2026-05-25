# Função: Sanitiza_Nome

**Arquivo de Origem:** `album.c`
**Responsável:** Limpeza profunda e padronização das strings na memória RAM.

##  O que foi feito
Durante a carga de dados do arquivo `.CSV`, strings frequentemente herdam caracteres de formatação invisíveis ou espaços em branco irregulares. Esta função varre a base de dados em tempo de execução para eliminar essas inconsistências, garantindo que buscas e alinhamentos de tela funcionem perfeitamente.

##  Como funciona a engrenagem
A função recebe o álbum por referência e itera sobre todas as figurinhas cadastradas aplicando três filtros em cada nome:
1. **Remoção de Quebra de Linha:** O comando `nome[strcspn(nome,"\r\n")] = '\0'` localiza e destrói os rastros da tecla ENTER.
2. **Remoção de Espaços Finais:** Mede a string dinamicamente com `strlen` e usa um laço `while` para substituir espaços consecutivos no fim da palavra pelo terminador nulo `\0`.
3. **Remoção de Espaços Iniciais (Arrastão):** Usa um laço `while` para identificar se o índice `[0]` é um espaço. Caso seja, um segundo `while` copia iterativamente cada caractere da frente (`j + 1`) para a posição atual (`j`), puxando a string inteira para a esquerda até eliminar todos os espaços iniciais.