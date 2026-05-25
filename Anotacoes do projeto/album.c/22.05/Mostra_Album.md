# Função: Mostra_Album

**Arquivo de Origem:** `album.c`
**Responsável:** Interface de exibição formatada da lista de figurinhas.

## O que foi feito
Criação da interface visual para a Opção 1 do menu principal. O objetivo é apresentar o banco de dados carregado na memória RAM em um formato de tabela limpa, alinhada e de fácil leitura para o usuário.

## Como funciona a engrenagem
1. **Trava de Segurança:** A função faz uma verificação inicial (`album->quantidade_atual == 0`). Se o álbum estiver vazio, a execução é interrompida via `return` e uma mensagem alerta o usuário para abrir pacotes primeiro.
2. **Laço de Exibição:** Itera sobre as figurinhas disponíveis.
3. **Formatação de Tabela:** Utiliza parâmetros avançados na função `printf` (como `%-20s`, `%7s` e `%10s`). Esses números forçam o console a reservar uma quantidade exata de caracteres para cada coluna, preenchendo o vazio com espaços em branco. Isso garante que as divisórias verticais (`||`) da tabela fiquem sempre perfeitamente alinhadas, independentemente do tamanho do nome do jogador.