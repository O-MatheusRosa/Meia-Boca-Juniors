//###########################################################//
//Bibliotecas
//###########################################################//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figurinha.h"
#include "utilitarios.h"
#include "raylib.h"

#ifndef _WIN32
    #include <strings.h>
    #define stricmp strcasecmp
#endif


//###########################################################//
//Funcao que vai meio q limpar o nome, pq ele tem os espaços, o \n, tem tudo, entao limpamos ele
//###########################################################//
void Sanitiza_Nome(Album *album){
    for (int i = 0; i < album->quantidade_atual; i++){
        char *nome = album->figurinhas[i].nome_Jogador;

        nome[strcspn(nome,"\r\n")] = '\0';

        int len = strlen(nome);

        while (len > 0 && nome[len - 1] == ' '){
            nome[len - 1] = '\0';
            len--;
        }//while q repete enquanto há caractere espaço sobrando  

        while(nome[0] == ' '){
           int j = 0;

           while (nome[j + 1] != 0){
                nome[j] = nome[j + 1];
                j++;
           }//while

           nome[j] = '\0';
        }//final do if     
    }//final do for 
}//funcao


//###########################################################//
//Funcao que vai printar o nome dos jogadores
//###########################################################//
void Mostra_Album(Album *album){

    if (album->quantidade_atual == 0){
        printf(">> O album ta vazio! Vai abrir pacotinho!\n");
        return;
    }//caso n tenha figurinha

    printf("\n=== LISTA DE FIGURINHAS ===\n");
    
    for (int i = 0; i < album->quantidade_atual; i++){
        if (album->figurinhas[i].colada == 1){
            printf("\n[ X ] Nome: %-20s || Codigo: %7s || Raridade: %10s || Repetidas: %d\n",album->figurinhas[i].nome_Jogador,album->figurinhas[i].codigo,album->figurinhas[i].raridade,album->figurinhas[i].quantidade_repetidas);
        }else{
            printf("\n[   ] Nome: %-20s || Codigo: %7s || Raridade: %10s || Repetidas: %d\n",album->figurinhas[i].nome_Jogador,album->figurinhas[i].codigo,album->figurinhas[i].raridade,album->figurinhas[i].quantidade_repetidas);
        }//else caso n tenha colado        
    }//for
    printf("===========================\n");
}//primeira funcao

//###########################################################//
//Função que vai ordenar a lista em binario
//###########################################################//
int Ordena_lista_Bin(const void *a, const void *b){

   Dados_Figurinha *figurinhaA = (Dados_Figurinha*) a;
   Dados_Figurinha *figurinhaB = (Dados_Figurinha*) b;

   return stricmp(figurinhaA->nome_Jogador, figurinhaB->nome_Jogador);
}//segunda função


//###########################################################//
//Funcao que vai buscar o nome do jogador
//###########################################################//
int Procura_Jogador(Album *album,char *nome_buscado){

    nome_buscado[strcspn(nome_buscado,"\r\n")] = '\0';

    for (int i = 0; i < album->quantidade_atual; i++){
        if (stricmp(nome_buscado,album->figurinhas[i].nome_Jogador) == 0){// o i ali no str i cpm é pra ignorar o case sensivite
            return i;
        }//if q compara o nome digitado e ve se tem o nome do jogador de vdd
    }//final do for q repete ate ter fig

    return -1;
}//enesima fnc

void Abre_Pacotinho(Album *catalogo, Album *meu_album){

   Animacao_Loading();

   if (catalogo->quantidade_atual == 0) {
        printf(VERMELHO "\n[ERRO] O catalogo esta vazio (CSV nao carregou)! Nao tem como sortear.\n" RESET);
        return; 
    }//caso n abra

    for (int i = 0; i < 7; i++){
        int sorteado = rand() % catalogo->quantidade_atual;

        char *nome_sorteado = catalogo->figurinhas[sorteado].nome_Jogador;
        int posicao = Procura_Jogador(meu_album,nome_sorteado);

        if (posicao != -1){
            meu_album->figurinhas[posicao].quantidade_repetidas++;
            printf(">> Repetida: %s, ja tenho %d dela.....\n",meu_album->figurinhas[posicao].nome_Jogador,meu_album->figurinhas[posicao].quantidade_repetidas);
        }else{

        if (meu_album->quantidade_atual >= meu_album->quantidade_maxima){
            
            if (meu_album->quantidade_maxima == 0) {
                meu_album->quantidade_maxima = 10;
            } else {
                meu_album->quantidade_maxima *= 2;
            }//else

            Dados_Figurinha *temp = realloc(meu_album->figurinhas, meu_album->quantidade_maxima * sizeof(Dados_Figurinha));
            
            if (temp == NULL){
                printf(VERMELHO "\n[ERRO] Memoria cheia! Nao deu pra expandir o album.\n" RESET);
                return; // Sai em seguranca
            }//if d eerro de realloc de memoria temporaria
            meu_album->figurinhas = temp;
        }//if de verificação se encheu ou nao a memoria 

                meu_album->figurinhas[meu_album->quantidade_atual] = catalogo->figurinhas[sorteado];
                
                meu_album->figurinhas[meu_album->quantidade_atual].colada = 0;
                meu_album->figurinhas[meu_album->quantidade_atual].quantidade_repetidas = 0;
                
                printf(AZUL "\n>> Nova figurinha: " RESET);
                printf(CIANO " %s\n" RESET, meu_album->figurinhas[meu_album->quantidade_atual].nome_Jogador);
                Som_Sucesso();
        
                        setbuf(stdin,NULL);
                        int escolha;
                        printf(VERMELHO ">> Colar no album? (1-sim/0-nao): " RESET);
                        scanf("%d",&escolha);

                        printf("\n");

                        if (escolha == 1){
                             meu_album->figurinhas[meu_album->quantidade_atual].colada = 1;
                        }else{
                            printf(AMARELO "OK, figurinha indo pro bolo....\n\n" RESET);
                        }//else
                meu_album->quantidade_atual++;   
        }//else 
    }//for   
}//funcao

void Alterar_Figurinha(Album *meu_album){

    char nome[51];

    setbuf(stdin,NULL);
    printf("\n>> Digite o nome do jogador: \n");
    fgets(nome,50,stdin);
    nome[strcspn(nome,"\n")] = '\0';

    int posicao = Procura_Jogador(meu_album, nome);

    if (posicao == -1){
        printf("\nvoce ainda nao achou esse jogador!\n");
    }else{
        printf("\n>> ENCONTRADA: %s <<\n",meu_album->figurinhas[posicao].nome_Jogador);
        printf("\n>> QUANTIDADE: %d <<\n",meu_album->figurinhas[posicao].quantidade_repetidas);

        if (meu_album->figurinhas[posicao].colada == 1) {
            printf("Status: [ X ] JA COLADA!\n");
            printf(">> Voce nao pode colar uma figurinha que ja esta colada...\n");
        } else {
            printf("Situacao: [   ] NAO COLADA.\n");
            
            int escolha;
            printf("\nDeseja colar essa figurinha no album? (1-Sim / 0-Nao): ");
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                meu_album->figurinhas[posicao].colada = 1;
                printf("\n>> FOI! A cartinha do %s foi colada no album!\n", meu_album->figurinhas[posicao].nome_Jogador);
            } else {
                printf("\n>> Operacao cancelada. A cartinha continua solta por ai.\n");
            }//else de colada ou nao
        }//quer colar?
    }//else de procura de figurinhas
}//funcao

void Exclui_Figurinha(Album *meu_album){
    char nome[51];

        setbuf(stdin,NULL);
        printf("\n>> Digite o nome do jogador: \n");
        fgets(nome,50,stdin);
        nome[strcspn(nome,"\n")] = '\0';

    int posicao = Procura_Jogador(meu_album, nome);

    if (posicao == -1){
        printf("\nvoce ainda nao achou esse jogador!\n");
    }else{
        for (int i = posicao; i < meu_album->quantidade_atual - 1; i++){
            meu_album->figurinhas[i] = meu_album->figurinhas[i+1];
        }//for
        
        meu_album->quantidade_atual--;

        printf("\nFIGURINHA EXCLUIDA!!!\n");
    }//else
}//funcao

void Adiciona_Figurinha(Album *catalogo){
    printf(CIANO "\n>> [ MODO ADMINISTRADOR ] CADASTRAR NOVA FIGURINHA GLOBAL <<\n" RESET);

    if (catalogo->quantidade_atual == catalogo->quantidade_maxima){
        catalogo->quantidade_maxima *=2;
        catalogo->figurinhas = realloc(catalogo->figurinhas, catalogo->quantidade_maxima * sizeof(Dados_Figurinha));

        if (catalogo->figurinhas == NULL){
            printf(VERMELHO "\n[ERRO FATAL] Falha ao expandir a memoria do catalogo!\n" RESET);
            return;
        }//caso n de o realloc
    }//final do if

    int pos = catalogo->quantidade_atual;
    setbuf(stdin, NULL);

    printf("Digite o Codigo (Ex: BRA-01): ");
    fgets(catalogo->figurinhas[pos].codigo, 10, stdin);
    catalogo->figurinhas[pos].codigo[strcspn(catalogo->figurinhas[pos].codigo, "\n")] = '\0';

    setbuf(stdin, NULL);
    printf("Digite o Nome do Jogador / Titulo: ");
    fgets(catalogo->figurinhas[pos].nome_Jogador, 50, stdin);
    catalogo->figurinhas[pos].nome_Jogador[strcspn(catalogo->figurinhas[pos].nome_Jogador, "\n")] = '\0';

    setbuf(stdin, NULL);
    printf("Digite a Secao (Ex: Brasil, Lendas): ");
    fgets(catalogo->figurinhas[pos].secao, 30, stdin);
    catalogo->figurinhas[pos].secao[strcspn(catalogo->figurinhas[pos].secao, "\n")] = '\0';

    setbuf(stdin, NULL);
    printf("Digite o Tipo/Raridade (Ex: Base, Ouro, Bordo): ");
    fgets(catalogo->figurinhas[pos].raridade, 20, stdin);
    catalogo->figurinhas[pos].raridade[strcspn(catalogo->figurinhas[pos].raridade, "\n")] = '\0';

    catalogo->figurinhas[pos].colada = 0;
    catalogo->figurinhas[pos].quantidade_repetidas = 0;

    catalogo->quantidade_atual++;

    FILE *arquivo = fopen("figurinhas2026.csv", "a"); // Ajuste o caminho
    
    if (arquivo != NULL) {
        fprintf(arquivo, "\n%s;%s;%s;%s", catalogo->figurinhas[pos].codigo, catalogo->figurinhas[pos].nome_Jogador, catalogo->figurinhas[pos].secao, catalogo->figurinhas[pos].raridade);
        fclose(arquivo);
    } else {
        printf(VERMELHO "\n[AVISO] A carta foi salva na RAM, mas houve erro ao abrir o arquivo .csv!\n" RESET);
    }//fim do else

    printf(VERDE "\n>> SUCESSO! %s foi adicionado(a) oficialmente ao universo do jogo!\n" RESET, catalogo->figurinhas[pos].nome_Jogador);
    Som_Sucesso(); // Beep da vitoria
}//final da fnc


//###########################################################//
//Pasta onde ficam as imagens das paginas do album (extraidas do PDF oficial)
//Cada imagem tem o MESMO nome da "secao" do csv (espaco trocado por _)
//Ex: secao "Costa do Marfim" -> assets/album/paginas/Costa_do_Marfim.jpg
//###########################################################//
#define PASTA_PAGINAS_ALBUM "assets/album/paginas/"
#define MAX_SECOES_ALBUM 64

//###########################################################//
//Monta a lista de secoes (paises/categorias) que existem no album do
//jogador, sem repetir, na ordem em que elas aparecem nas figurinhas
//###########################################################//
static int Monta_Lista_Secoes(Album *meu_album, char lista[][60]){

    int total = 0;

    for (int i = 0; i < meu_album->quantidade_atual; i++){

        char *secao = meu_album->figurinhas[i].secao;
        int ja_existe = 0;

        for (int j = 0; j < total; j++){
            if (strcmp(lista[j], secao) == 0){
                ja_existe = 1;
                break;
            }//if achou repetida
        }//for j

        if (!ja_existe && total < MAX_SECOES_ALBUM){
            strcpy(lista[total], secao);
            total++;
        }//if e uma secao nova
    }//for i

    return total;
}//funcao

//###########################################################//
//Troca os espacos por "_" pra montar o nome do arquivo de imagem
//Ex: "Costa do Marfim" -> "Costa_do_Marfim"
//###########################################################//
static void Monta_Nome_Arquivo(const char *secao, char *saida){

    int i = 0;

    while (secao[i] != '\0'){
        saida[i] = (secao[i] == ' ') ? '_' : secao[i];
        i++;
    }//while

    saida[i] = '\0';
}//funcao

//###########################################################//
//Funcao que desenha o album de forma grafica (chamada com a tecla A no mapa)
//Usa as paginas reais do album (imagens extraidas do PDF oficial da Panini)
//como fundo, e ao lado mostra quais figurinhas daquela pagina ja foram coladas
//
//Recebe a pagina atual por ponteiro, pra lembrar em que pagina o jogador estava
//###########################################################//
void Desenha_Album(Album *meu_album, int *pagina){

    //listas e textura ficam "static" pra n precisar remontar/recarregar
    //tudo de novo a cada frame, so quando o album realmente mudar de pagina
    static char secoes[MAX_SECOES_ALBUM][60];
    static int total_secoes = -1;
    static int qtd_conhecida = -1; //guarda quantas figurinhas tinha na ultima vez que montamos a lista

    static Texture2D textura_pagina;
    static int pagina_carregada = -1;
    static bool textura_valida = false;

    //(re)monta a lista de secoes se for a 1a vez OU se o album mudou de tamanho
    //(pode mudar pq o jogador abriu um pacotinho novo, por exemplo)
    if (total_secoes == -1 || qtd_conhecida != meu_album->quantidade_atual){
        total_secoes = Monta_Lista_Secoes(meu_album, secoes);
        qtd_conhecida = meu_album->quantidade_atual;
        pagina_carregada = -1; //forca recarregar a textura, pode ter mudado a ordem
    }//if precisa remontar

    //fundo escurecido por cima do mapa, pra destacar o album
    DrawRectangle(0, 0, 1280, 720, Fade(BLACK, 0.8f));

    if (total_secoes <= 0){
        DrawRectangle(140, 260, 1000, 200, Fade(DARKBLUE, 0.92f));
        DrawRectangleLines(140, 260, 1000, 200, YELLOW);
        DrawText("Seu album esta vazio! Va abrir uns pacotinhos...", 220, 350, 22, WHITE);
        DrawText("[A] Fechar", 220, 400, 18, WHITE);
        return;
    }//if album vazio

    //trava de seguranca, caso o album tenha mudado de tamanho (excluiu fig, etc)
    if (*pagina >= total_secoes) *pagina = total_secoes - 1;
    if (*pagina < 0) *pagina = 0;

    //navegacao entre paginas (paises) com as setas do teclado
    if (IsKeyPressed(KEY_RIGHT) && *pagina < total_secoes - 1){
        (*pagina)++;
    }//if pag seguinte

    if (IsKeyPressed(KEY_LEFT) && *pagina > 0){
        (*pagina)--;
    }//if pag anterior

    //carrega a imagem da pagina atual, so quando o jogador troca de pagina
    //(carregar textura toda hora, todo frame, pesaria muito)
    if (pagina_carregada != *pagina){

        if (textura_valida){
            UnloadTexture(textura_pagina);
            textura_valida = false;
        }//descarrega a pagina anterior da memoria de video

        char nome_arquivo[60];
        Monta_Nome_Arquivo(secoes[*pagina], nome_arquivo);

        char caminho[120];
        sprintf(caminho, "%s%s.jpg", PASTA_PAGINAS_ALBUM, nome_arquivo);

        if (FileExists(caminho)){
            textura_pagina = LoadTexture(caminho);
            textura_valida = true;
        }//if a imagem dessa secao existe

        pagina_carregada = *pagina;
    }//if trocou de pagina

    //painel principal do album
    int painel_x = 40, painel_y = 40, painel_w = 1200, painel_h = 640;

    DrawRectangle(painel_x, painel_y, painel_w, painel_h, Fade(BLACK, 0.92f));
    DrawRectangleLines(painel_x, painel_y, painel_w, painel_h, YELLOW);

    DrawText("MEU ALBUM - COPA 2026", painel_x + 20, painel_y + 12, 24, YELLOW);
    DrawText(TextFormat("Pagina %d/%d", *pagina + 1, total_secoes), painel_x + painel_w - 150, painel_y + 16, 18, WHITE);

    //area da imagem real da pagina (esquerda do painel)
    int img_x = painel_x + 15;
    int img_y = painel_y + 50;
    int img_w = 760;
    int img_h = 575;

    if (textura_valida){
        Rectangle origem  = { 0, 0, (float)textura_pagina.width, (float)textura_pagina.height };
        Rectangle destino = { (float)img_x, (float)img_y, (float)img_w, (float)img_h };

        DrawTexturePro(textura_pagina, origem, destino, (Vector2){ 0, 0 }, 0.0f, WHITE);
    }else{
        DrawRectangle(img_x, img_y, img_w, img_h, Fade(GRAY, 0.3f));
        DrawText("Pagina visual nao encontrada", img_x + 60, img_y + 270, 20, LIGHTGRAY);
        DrawText(secoes[*pagina], img_x + 60, img_y + 300, 20, LIGHTGRAY);
    }//else sem imagem pra essa secao

    //painel lateral direito, com a lista de figurinhas daquela secao/pais
    int lista_x = img_x + img_w + 20;
    int lista_y = painel_y + 50;
    int lista_w = painel_x + painel_w - lista_x - 15;

    DrawText(secoes[*pagina], lista_x, lista_y, 22, YELLOW);

    int y = lista_y + 35;
    int coladas = 0;
    int total_da_secao = 0;

    for (int i = 0; i < meu_album->quantidade_atual; i++){

        if (strcmp(meu_album->figurinhas[i].secao, secoes[*pagina]) != 0){
            continue; //essa figurinha n e dessa pagina, ignora
        }//if

        Dados_Figurinha *fig = &meu_album->figurinhas[i];
        total_da_secao++;

        Color cor = (fig->colada == 1) ? GREEN : LIGHTGRAY;
        const char *status = (fig->colada == 1) ? "[X]" : "[ ]";

        //corta o nome se for muito grande, pra n estourar o painel lateral
        char nome_curto[26];
        strncpy(nome_curto, fig->nome_Jogador, 25);
        nome_curto[25] = '\0';

        DrawText(TextFormat("%s %-6s", status, fig->codigo), lista_x, y, 14, cor);
        DrawText(nome_curto, lista_x, y + 16, 13, cor);

        if (fig->quantidade_repetidas > 0){
            DrawText(TextFormat("x%d repetida", fig->quantidade_repetidas), lista_x + lista_w - 90, y + 8, 12, ORANGE);
        }//if tem repetida

        if (fig->colada == 1) coladas++;

        y += 38;

        //se a listinha ficou maior que o espaco do painel, para de desenhar
        //(pra n desenhar por cima do rodape)
        if (y > painel_y + painel_h - 70) break;
    }//for figurinhas dessa secao

    DrawRectangle(lista_x, painel_y + painel_h - 55, lista_w, 35, Fade(DARKGREEN, 0.5f));
    DrawText(TextFormat("Coladas: %d/%d", coladas, total_da_secao), lista_x + 10, painel_y + painel_h - 47, 16, WHITE);

    DrawText("[A] Fechar album      [SETAS] Trocar de pais/pagina", painel_x + 15, painel_y + painel_h - 30, 16, WHITE);
}//funcao Desenha_Album