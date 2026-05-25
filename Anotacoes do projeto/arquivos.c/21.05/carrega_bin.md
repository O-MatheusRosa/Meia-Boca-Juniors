# Função: Carregar_Album_Binario

**Arquivo de Origem:** `arquivos.c`
**Responsável:** Restauração do estado salvo do sistema.

## O que foi feito
Permite que o programa pule a leitura do `.CSV` original nas execuções subsequentes, carregando diretamente o estado exato em que o usuário parou na última sessão.

## Como funciona a engrenagem
1. Tenta abrir o `album.bin` em modo de leitura binária (`"rb"`).
2. Se o arquivo retornar `NULL` (não existe), a função avisa a `main` para usar o `.CSV`.
3. Se existir, utiliza a função `fread` para sugar o bloco de dados binários do disco rígido e realocar tudo diretamente nas variáveis e structs da memória RAM.
4. Restaura a variável `quantidade_atual` e fecha o arquivo.