# Função: Salvar_Album_Binario

### Rosa

**Arquivo de Origem:** `arquivos.c`
**Responsável:** Persistência definitiva dos dados do usuário.

## O que foi feito
Para que o usuário não perca seu progresso (figurinhas coladas, repetidas e alterações) ao fechar o programa, construímos uma função que tira uma "fotografia" da memória RAM atual e a grava no disco rígido em formato binário.

## Como funciona a engrenagem
1. Abre (ou cria) um arquivo chamado `album.bin` em modo de escrita binária (`"wb"`).
2. O modo `"wb"` destrói o arquivo antigo e reescreve um novo com os dados atualizados.
3. Utiliza a função `fwrite` para despejar a struct inteira (ou o vetor dinâmico de figurinhas) diretamente no arquivo em um único bloco de memória, garantindo alta velocidade de I/O (Input/Output), que não é possivel por fscanf ou fprinf.
4. Fecha o arquivo com `fclose`, blindando os dados contra corrupção.