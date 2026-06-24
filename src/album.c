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
//Funcao que vai meio q limpar o nome, pq ele tem os espa�os, o \n, tem tudo, entao limpamos ele
//###########################################################//
void Sanitiza_Nome(Album *album){
    for (int i = 0; i < album->quantidade_atual; i++){
        char *nome = album->figurinhas[i].nome_Jogador;

        nome[strcspn(nome,"\r\n")] = '\0';

        int len = strlen(nome);

        while (len > 0 && nome[len - 1] == ' '){
            nome[len - 1] = '\0';
            len--;
        }//while q repete enquanto h� caractere espa�o sobrando  

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
//Sanitiza o campo secao de todas as figurinhas:
//remove espacos antes e depois, igual a Sanitiza_Nome mas pro campo secao
//###########################################################//
void Sanitiza_Secao(Album *album){
    for (int i = 0; i < album->quantidade_atual; i++){
        char *secao = album->figurinhas[i].secao;

        secao[strcspn(secao,"\r\n")] = '\0';

        int len = strlen(secao);

        while (len > 0 && secao[len - 1] == ' '){
            secao[len - 1] = '\0';
            len--;
        }//while tira espacos no fim

        while(secao[0] == ' '){
           int j = 0;
           while (secao[j + 1] != 0){
                secao[j] = secao[j + 1];
                j++;
           }//while
           secao[j] = '\0';
        }//tira espacos no inicio
    }//for
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
//Fun��o que vai ordenar a lista em binario
//###########################################################//
int Ordena_lista_Bin(const void *a, const void *b){

   Dados_Figurinha *figurinhaA = (Dados_Figurinha*) a;
   Dados_Figurinha *figurinhaB = (Dados_Figurinha*) b;

   return stricmp(figurinhaA->nome_Jogador, figurinhaB->nome_Jogador);
}//segunda fun��o


//###########################################################//
//Funcao que vai buscar o nome do jogador
//###########################################################//
int Procura_Jogador(Album *album,char *nome_buscado){

    nome_buscado[strcspn(nome_buscado,"\r\n")] = '\0';

    for (int i = 0; i < album->quantidade_atual; i++){
        if (stricmp(nome_buscado,album->figurinhas[i].nome_Jogador) == 0){// o i ali no str i cpm � pra ignorar o case sensivite
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
        }//if de verifica��o se encheu ou nao a memoria 

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
//Ex: secao "Costa do Marfim" -> assets/Costa_do_Marfim.jpg
//###########################################################//
#define PASTA_PAGINAS_ALBUM "assets/"
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
//###########################################################//
void Desenha_Album(Album *meu_album, int *pagina){

    static char secoes[MAX_SECOES_ALBUM][60];
    static int total_secoes = -1;
    static int qtd_conhecida = -1;

    static Texture2D textura_pagina;
    static int pagina_carregada = -1;
    static bool textura_valida = false;

    #define MAX_FIGS_PAGINA 30
    static Texture2D tex_figs[MAX_FIGS_PAGINA];
    static bool tex_figs_valida[MAX_FIGS_PAGINA];
    static int n_figs_cache = 0;
    static int pagina_figs_carregada = -1;

    if (total_secoes == -1 || qtd_conhecida != meu_album->quantidade_atual){
        total_secoes = Monta_Lista_Secoes(meu_album, secoes);
        qtd_conhecida = meu_album->quantidade_atual;
        pagina_carregada = -1;
        pagina_figs_carregada = -1;
    }

    DrawRectangle(0, 0, 1280, 720, BLACK);

    if (total_secoes <= 0){
        DrawText("Album vazio! Abra uns pacotinhos primeiro.", 340, 340, 24, GOLD);
        DrawText("[A] Fechar", 580, 390, 18, WHITE);
        return;
    }

    if (*pagina >= total_secoes) *pagina = total_secoes - 1;
    if (*pagina < 0) *pagina = 0;

    if (IsKeyPressed(KEY_RIGHT) && *pagina < total_secoes - 1) (*pagina)++;
    if (IsKeyPressed(KEY_LEFT)  && *pagina > 0)               (*pagina)--;

    // --- carrega imagem de fundo ---
    if (pagina_carregada != *pagina){
        if (textura_valida){ UnloadTexture(textura_pagina); textura_valida = false; }
        char nome_arquivo[60];
        Monta_Nome_Arquivo(secoes[*pagina], nome_arquivo);
        char caminho[120];
        sprintf(caminho, "%s%s.jpg", PASTA_PAGINAS_ALBUM, nome_arquivo);
        if (FileExists(caminho)){ textura_pagina = LoadTexture(caminho); textura_valida = true; }
        pagina_carregada = *pagina;
    }

    // --- carrega texturas das figurinhas ---
    if (pagina_figs_carregada != *pagina){
        for (int i = 0; i < n_figs_cache; i++)
            if (tex_figs_valida[i]){ UnloadTexture(tex_figs[i]); tex_figs_valida[i] = false; }
        n_figs_cache = 0;

        for (int i = 0; i < meu_album->quantidade_atual && n_figs_cache < MAX_FIGS_PAGINA; i++){
            if (strcmp(meu_album->figurinhas[i].secao, secoes[*pagina]) != 0) continue;
            tex_figs_valida[n_figs_cache] = false;
            if (meu_album->figurinhas[i].colada == 1){
                char cod[15];
                strncpy(cod, meu_album->figurinhas[i].codigo, 14); cod[14] = '\0';
                int cl = strlen(cod);
                while(cl > 0 && (cod[cl-1]==' '||cod[cl-1]=='\r'||cod[cl-1]=='\n')){ cod[cl-1]='\0'; cl--; }
                char caminho_fig[80];
                sprintf(caminho_fig, "assets/figurinhas/%s.png", cod);
                if (FileExists(caminho_fig)){
                    tex_figs[n_figs_cache] = LoadTexture(caminho_fig);
                    tex_figs_valida[n_figs_cache] = true;
                }
            }
            n_figs_cache++;
        }
        pagina_figs_carregada = *pagina;
    }

    //------------------------------------------------------------------
    // LAYOUT
    // Topo: HUD (38px)
    // Meio: imagem do album
    // Base: bandeja de figurinhas (160px)
    //------------------------------------------------------------------
    int HUD_H   = 38;
    int BAND_H  = 165;
    int IMG_Y   = HUD_H;
    int IMG_H   = 720 - HUD_H - BAND_H;   // ~517px
    int BAND_Y  = HUD_H + IMG_H;

    // --- imagem do album ---
    if (textura_valida){
        Rectangle src = {0,0,(float)textura_pagina.width,(float)textura_pagina.height};
        Rectangle dst = {0,(float)IMG_Y,1280,(float)IMG_H};
        DrawTexturePro(textura_pagina, src, dst, (Vector2){0,0}, 0.0f, WHITE);
    } else {
        DrawRectangle(0, IMG_Y, 1280, IMG_H, Fade(DARKGRAY, 0.6f));
        DrawText(secoes[*pagina], 640 - MeasureText(secoes[*pagina],28)/2, IMG_Y + IMG_H/2, 28, WHITE);
    }

    // sombra separando album da bandeja
    DrawRectangleGradientV(0, BAND_Y - 18, 1280, 18, BLANK, Fade(BLACK, 0.8f));

    //------------------------------------------------------------------
    // BANDEJA DE FIGURINHAS
    //------------------------------------------------------------------
    DrawRectangle(0, BAND_Y, 1280, BAND_H, (Color){18, 18, 28, 255});
    DrawLine(0, BAND_Y, 1280, BAND_Y, Fade(GOLD, 0.5f));

    // monta lista de figurinhas desta secao
    int indices[MAX_FIGS_PAGINA];
    int total_secao = 0, coladas = 0;
    for (int i = 0; i < meu_album->quantidade_atual && total_secao < MAX_FIGS_PAGINA; i++){
        if (strcmp(meu_album->figurinhas[i].secao, secoes[*pagina]) != 0) continue;
        indices[total_secao++] = i;
        if (meu_album->figurinhas[i].colada == 1) coladas++;
    }

    // dimensoes de cada card na bandeja
    int FIG_W = 80, FIG_H = 110;
    int GAP   = 8;
    int total_largura = total_secao * (FIG_W + GAP) - GAP;
    int start_x = (1280 - total_largura) / 2; // centralizado
    if (start_x < 10) start_x = 10;
    int fig_y = BAND_Y + (BAND_H - FIG_H) / 2;

    int cache_idx = 0;
    for (int k = 0; k < total_secao; k++){
        Dados_Figurinha *fig = &meu_album->figurinhas[indices[k]];
        int fx = start_x + k * (FIG_W + GAP);

        if (fig->colada == 1){
            if (cache_idx < n_figs_cache && tex_figs_valida[cache_idx]){
                // foto real da figurinha
                Rectangle src = {0,0,(float)tex_figs[cache_idx].width,(float)tex_figs[cache_idx].height};
                Rectangle dst = {(float)fx,(float)fig_y,(float)FIG_W,(float)FIG_H};
                DrawTexturePro(tex_figs[cache_idx], src, dst, (Vector2){0,0}, 0.0f, WHITE);
            } else {
                // colada mas sem imagem
                DrawRectangle(fx, fig_y, FIG_W, FIG_H, Fade(DARKGREEN, 0.8f));
                char cod[12]; strncpy(cod, fig->codigo, 11); cod[11]='\0';
                int cl=strlen(cod); while(cl>0&&cod[cl-1]==' '){cod[cl-1]='\0';cl--;}
                DrawText(cod, fx + FIG_W/2 - MeasureText(cod,11)/2, fig_y + FIG_H/2 - 8, 11, WHITE);
            }
            // borda verde
            DrawRectangleLinesEx((Rectangle){(float)fx,(float)fig_y,(float)FIG_W,(float)FIG_H}, 2, GREEN);
            // badge repetida
            if (fig->quantidade_repetidas > 0){
                const char *rep = TextFormat("+%d", fig->quantidade_repetidas);
                DrawRectangle(fx + FIG_W - 22, fig_y, 22, 16, Fade(ORANGE, 0.95f));
                DrawText(rep, fx + FIG_W - 20, fig_y + 2, 11, BLACK);
            }
            cache_idx++;
        } else {
            // nao colada: slot vazio
            DrawRectangle(fx, fig_y, FIG_W, FIG_H, Fade(BLACK, 0.6f));
            DrawRectangleLinesEx((Rectangle){(float)fx,(float)fig_y,(float)FIG_W,(float)FIG_H}, 1, Fade(GRAY, 0.35f));
            DrawText("?", fx + FIG_W/2 - 7, fig_y + FIG_H/2 - 12, 22, Fade(GRAY, 0.4f));
        }
    }

    //------------------------------------------------------------------
    // HUD TOPO
    //------------------------------------------------------------------
    DrawRectangle(0, 0, 1280, HUD_H, Fade(BLACK, 0.85f));
    DrawLine(0, HUD_H, 1280, HUD_H, Fade(GOLD, 0.4f));

    // nome do pais
    DrawText(secoes[*pagina], 14, 9, 22, GOLD);

    // pagina centralizada
    const char *pag_txt = TextFormat("%d / %d", *pagina + 1, total_secoes);
    DrawText(pag_txt, 640 - MeasureText(pag_txt,18)/2, 10, 18, WHITE);

    // progresso direita
    Color cor_prog = (coladas == total_secao && total_secao > 0) ? GOLD : GREEN;
    const char *prog = TextFormat("%d/%d coladas", coladas, total_secao);
    DrawText(prog, 1280 - MeasureText(prog,18) - 14, 10, 18, cor_prog);

    //------------------------------------------------------------------
    // SETAS DE NAVEGACAO
    //------------------------------------------------------------------
    Vector2 mouse = GetMousePosition();

    if (*pagina > 0){
        Rectangle btn = {8, (float)IMG_Y + IMG_H/2 - 30, 44, 60};
        bool hov = CheckCollisionPointRec(mouse, btn);
        DrawRectangleRounded(btn, 0.3f, 6, hov ? GOLD : Fade(BLACK, 0.75f));
        DrawText("<", 18, IMG_Y + IMG_H/2 - 14, 30, hov ? BLACK : WHITE);
        if (hov && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) (*pagina)--;
    }
    if (*pagina < total_secoes - 1){
        Rectangle btn = {1228, (float)IMG_Y + IMG_H/2 - 30, 44, 60};
        bool hov = CheckCollisionPointRec(mouse, btn);
        DrawRectangleRounded(btn, 0.3f, 6, hov ? GOLD : Fade(BLACK, 0.75f));
        DrawText(">", 1240, IMG_Y + IMG_H/2 - 14, 30, hov ? BLACK : WHITE);
        if (hov && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) (*pagina)++;
    }

    // dica fechar
    DrawText("[A] Fechar", 14, BAND_Y + BAND_H - 20, 13, Fade(WHITE, 0.4f));

}//funcao Desenha_Album