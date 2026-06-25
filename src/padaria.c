#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "textura.h"

#define TEMPO_TROCA 600

// ============================================================
// funcoes internas do balcao
// ============================================================

static void atualizarFigurinhasBalcao(BancaPadaria *b, Album *catalogo) {
    for (int i = 0; i < 5; i++) {
        int idx = rand() % catalogo->quantidade_atual;
        strncpy(b->FigurinhasNoBalcao[i], catalogo->figurinhas[idx].codigo, 14);
        b->FigurinhasNoBalcao[i][14] = '\0';

        // remove espacos, \r e \n do final do codigo
        int len = strlen(b->FigurinhasNoBalcao[i]);
        while (len > 0 && (b->FigurinhasNoBalcao[i][len-1] == ' '  ||
                           b->FigurinhasNoBalcao[i][len-1] == '\r' ||
                           b->FigurinhasNoBalcao[i][len-1] == '\n')) {
            b->FigurinhasNoBalcao[i][--len] = '\0';
        }
        b->slotUsado[i] = 0;
    }
    b->ultimaAtualizacao = time(NULL);
    printf("[padaria] balcao atualizado com novas figurinhas!\n");
}

static void verificaTempoDoBalcao(BancaPadaria *b, Album *catalogo) {
    double segundosPassados = difftime(time(NULL), b->ultimaAtualizacao);
    if (segundosPassados >= TEMPO_TROCA) {
        atualizarFigurinhasBalcao(b, catalogo);
    }
}

static void carregarTexFigurinhas(BancaPadaria *b, Texture2D texFigurinhas[5]) {
    for (int i = 0; i < 5; i++) {
        if (texFigurinhas[i].id != 0) UnloadTexture(texFigurinhas[i]);
        char caminho[64];
        sprintf(caminho, "assets/figurinhas/%s.png", b->FigurinhasNoBalcao[i]);
        texFigurinhas[i] = LoadTexture(caminho);
        if (texFigurinhas[i].id == 0)
            printf("[erro] figurinha nao encontrada: %s\n", caminho);
    }
}

// procura no album do jogador uma figurinha com quantidade_repetidas > 0
// retorna o indice no album, ou -1 se nao tiver nenhuma repetida
static int acharFigurinhaRepetida(Album *meu_album) {
    for (int i = 0; i < meu_album->quantidade_atual; i++) {
        if (meu_album->figurinhas[i].quantidade_repetidas > 0) {
            return i;
        }
    }
    return -1;
}

// remove espacos/\r/\n do final de uma string (in-place)
static void limparCodigo(const char *entrada, char *saida, int tamanho) {
    strncpy(saida, entrada, tamanho - 1);
    saida[tamanho - 1] = '\0';
    int len = strlen(saida);
    while (len > 0 && (saida[len-1] == ' ' || saida[len-1] == '\r' || saida[len-1] == '\n'))
        saida[--len] = '\0';
}

// adiciona a figurinha do balcao no album do jogador (colada ou repetida)
static void adicionarFigurinhaNoAlbum(Album *meu_album, Album *catalogo_geral, const char *codigo) {
    char cod_limpo[15];

    // procura no album do jogador
    for (int i = 0; i < meu_album->quantidade_atual; i++) {
        limparCodigo(meu_album->figurinhas[i].codigo, cod_limpo, 15);
        if (strcmp(cod_limpo, codigo) == 0) {
            if (meu_album->figurinhas[i].colada == 0) {
                meu_album->figurinhas[i].colada = 1;
            } else {
                meu_album->figurinhas[i].quantidade_repetidas++;
            }
            return;
        }
    }

    // nao tem no album: procura no catalogo e adiciona
    for (int i = 0; i < catalogo_geral->quantidade_atual; i++) {
        limparCodigo(catalogo_geral->figurinhas[i].codigo, cod_limpo, 15);
        if (strcmp(cod_limpo, codigo) == 0) {
            int qtd = meu_album->quantidade_atual;
            if (qtd == 0) {
                meu_album->figurinhas = malloc(sizeof(Dados_Figurinha));
            } else {
                meu_album->figurinhas = realloc(meu_album->figurinhas, (qtd + 1) * sizeof(Dados_Figurinha));
            }
            meu_album->figurinhas[qtd] = catalogo_geral->figurinhas[i];
            meu_album->figurinhas[qtd].colada = 1;
            meu_album->figurinhas[qtd].quantidade_repetidas = 0;
            meu_album->quantidade_atual++;
            return;
        }
    }
}

// ============================================================
// funcao principal da padaria
// ============================================================
void Entra_Padaria(BancaPadaria *padaria, Album *catalogo_geral, Album *meu_album, float *saldo_jogador, int *tem_album, int *pacotes_bolso, int *figurinhas_bolso) {

    if (padaria->ultimaAtualizacao == 0) {
        atualizarFigurinhasBalcao(padaria, catalogo_geral);
    }

    Texture2D fundo        = LoadTexture("assets/padaria_principal.jpeg");
    Texture2D fundoBalcao  = LoadTexture("assets/padaria_segunda.png");
    Texture2D fundoComprar = LoadTexture("assets/padaria_comprarpacotes.png");
    Texture2D fundoVender  = LoadTexture("assets/padaria_venderfigurinhas.png");
    Texture2D fundoTrocar  = LoadTexture("assets/padaria_trocarfigurinhas.png");

    if (fundo.id == 0)        printf("[erro] padaria_principal.png nao encontrada!\n");
    if (fundoBalcao.id == 0)  printf("[erro] padaria_segunda.png nao encontrada!\n");
    if (fundoComprar.id == 0) printf("[erro] padaria_comprarpacotes.png nao encontrada!\n");
    if (fundoVender.id == 0)  printf("[erro] padaria_venderfigurinhas.png nao encontrada!\n");
    if (fundoTrocar.id == 0)  printf("[erro] padaria_trocarfigurinhas.png nao encontrada!\n");

    Texture2D texFigurinhas[5] = {0};
    carregarTexFigurinhas(padaria, texFigurinhas);
    time_t ultimaTextura = padaria->ultimaAtualizacao;

    int telaAtual = 0;
    int opcao     = -1;

    char inputTexto[16] = "";
    int  inputLen       = 0;

    char  msgFeedback[64] = "";
    Color corFeedback     = GREEN;
    float timerFeedback   = 0.0f;

    Rectangle areaBalcao = {230, 200, 600, 350};

    Rectangle btn1 = {160, 483, 1100, 40};
    Rectangle btn2 = {160, 525, 1100, 40};
    Rectangle btn3 = {160, 567, 1100, 40};
    Rectangle btn4 = {160, 609, 1100, 40};
    Rectangle btn5 = {160, 651, 1100, 40};

    Rectangle btnOk = {885, 610, 100, 55};

    Rectangle btnTrocar[5] = {
        {108, 510, 170, 50},
        {358, 510, 170, 50},
        {593, 510, 170, 50},
        {800, 510, 170, 50},
        {1028, 510, 170, 50},
    };

    int imgLarg  = 180;
    int imgAltu  = 250;
    int imgTopoY = 200;

    // ============================================================
    // loop da padaria
    // ============================================================
    while (!WindowShouldClose()) {

        verificaTempoDoBalcao(padaria, catalogo_geral);

        if (padaria->ultimaAtualizacao != ultimaTextura) {
            carregarTexFigurinhas(padaria, texFigurinhas);
            ultimaTextura = padaria->ultimaAtualizacao;
        }

        if (telaAtual == 2 || telaAtual == 3) {
            int tecla = GetCharPressed();
            while (tecla > 0) {
                if (tecla >= '0' && tecla <= '9' && inputLen < 5) {
                    inputTexto[inputLen++] = (char)tecla;
                    inputTexto[inputLen]   = '\0';
                }
                tecla = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputLen > 0) {
                inputTexto[--inputLen] = '\0';
            }
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            if (telaAtual == 0) {
                UnloadTexture(fundo); UnloadTexture(fundoBalcao);
                UnloadTexture(fundoComprar); UnloadTexture(fundoVender);
                UnloadTexture(fundoTrocar);
                for (int i = 0; i < 5; i++)
                    if (texFigurinhas[i].id != 0) UnloadTexture(texFigurinhas[i]);
                return;
            } else if (telaAtual == 1) {
                telaAtual = 0;
            } else if (telaAtual == 2 || telaAtual == 3) {
                telaAtual = 1; inputTexto[0] = '\0'; inputLen = 0;
            } else if (telaAtual == 4) {
                telaAtual = 1;
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            if (telaAtual == 0 && CheckCollisionPointRec(mouse, areaBalcao))
                telaAtual = 1;

            if (telaAtual == 1) {
                if (CheckCollisionPointRec(mouse, btn1)) opcao = 1;
                if (CheckCollisionPointRec(mouse, btn2)) opcao = 2;
                if (CheckCollisionPointRec(mouse, btn3)) opcao = 3;
                if (CheckCollisionPointRec(mouse, btn4)) opcao = 4;
                if (CheckCollisionPointRec(mouse, btn5)) opcao = 0;
            }

            if (telaAtual == 2 && CheckCollisionPointRec(mouse, btnOk)) {
                int qtd     = atoi(inputTexto);
                float custo = qtd * padaria->precoPacote;
                if (qtd <= 0) {
                    strcpy(msgFeedback, "Digite uma quantidade valida!");
                    corFeedback = RED;
                } else if (*saldo_jogador < custo) {
                    strcpy(msgFeedback, "Dinheiro insuficiente!");
                    corFeedback = RED;
                } else {
                    *saldo_jogador -= custo;
                    *pacotes_bolso += qtd;
                    strcpy(msgFeedback, "Compra realizada com sucesso!");
                    corFeedback = GREEN;
                }
                timerFeedback = 3.0f; telaAtual = 1;
                inputTexto[0] = '\0'; inputLen = 0;
            }

            // tela 3: venda — desconta figurinhas repetidas do album
            if (telaAtual == 3 && CheckCollisionPointRec(mouse, btnOk)) {
                int qtd = atoi(inputTexto);

                // conta total de figurinhas repetidas no album
                int totalRepetidas = 0;
                for (int j = 0; j < meu_album->quantidade_atual; j++)
                    totalRepetidas += meu_album->figurinhas[j].quantidade_repetidas;

                if (qtd <= 0) {
                    strcpy(msgFeedback, "Digite uma quantidade valida!");
                    corFeedback = RED;
                } else if (qtd > totalRepetidas) {
                    strcpy(msgFeedback, "Figurinhas repetidas insuficientes!");
                    corFeedback = RED;
                } else {
                    // desconta as repetidas do album, uma a uma
                    int restante = qtd;
                    for (int j = 0; j < meu_album->quantidade_atual && restante > 0; j++) {
                        int tirar = meu_album->figurinhas[j].quantidade_repetidas;
                        if (tirar > restante) tirar = restante;
                        meu_album->figurinhas[j].quantidade_repetidas -= tirar;
                        restante -= tirar;
                    }
                    *saldo_jogador += qtd * padaria->precoComprarFigurinhas;
                    strcpy(msgFeedback, "Venda realizada com sucesso!");
                    corFeedback = GREEN;
                }
                timerFeedback = 3.0f; telaAtual = 1;
                inputTexto[0] = '\0'; inputLen = 0;
            }

            // tela 4: troca — gasta 1 figurinha repetida do album
            if (telaAtual == 4) {
                for (int i = 0; i < 5; i++) {
                    if (CheckCollisionPointRec(mouse, btnTrocar[i])) {
                        if (padaria->slotUsado[i]) {
                            strcpy(msgFeedback, "Essa figurinha ja foi trocada!");
                            corFeedback = RED;
                        } else {
                            int idxRepetida = acharFigurinhaRepetida(meu_album);
                            if (idxRepetida == -1) {
                                strcpy(msgFeedback, "Sem figurinhas repetidas para trocar!");
                                corFeedback = RED;
                            } else {
                                // desconta 1 repetida do album
                                meu_album->figurinhas[idxRepetida].quantidade_repetidas--;
                                // adiciona a figurinha do balcao no album
                                adicionarFigurinhaNoAlbum(meu_album, catalogo_geral, padaria->FigurinhasNoBalcao[i]);
                                padaria->slotUsado[i] = 1;
                                printf("[padaria] trocou pela figurinha %s!\n", padaria->FigurinhasNoBalcao[i]);
                                strcpy(msgFeedback, "Troca realizada com sucesso!");
                                corFeedback = GREEN;
                            }
                        }
                        timerFeedback = 3.0f;
                    }
                }
            }
        }

        switch (opcao) {
            case 1:
                if (*tem_album) {
                    strcpy(msgFeedback, "Voce ja possui o album!");
                    corFeedback = RED;
                } else if (padaria->qntAlbum > 0 && *saldo_jogador >= padaria->precoAlbum) {
                    *saldo_jogador -= padaria->precoAlbum;
                    *tem_album      = 1;
                    padaria->qntAlbum--;
                    strcpy(msgFeedback, "Album comprado com sucesso!");
                    corFeedback = GREEN;
                } else {
                    strcpy(msgFeedback, "Sem dinheiro suficiente!");
                    corFeedback = RED;
                }
                timerFeedback = 3.0f; opcao = -1; break;

            case 2:
                telaAtual = 2; inputTexto[0] = '\0'; inputLen = 0; opcao = -1; break;

            case 3:
                telaAtual = 4; opcao = -1; break;

            case 4:
                telaAtual = 3; inputTexto[0] = '\0'; inputLen = 0; opcao = -1; break;

            case 0:
                UnloadTexture(fundo); UnloadTexture(fundoBalcao);
                UnloadTexture(fundoComprar); UnloadTexture(fundoVender);
                UnloadTexture(fundoTrocar);
                for (int i = 0; i < 5; i++)
                    if (texFigurinhas[i].id != 0) UnloadTexture(texFigurinhas[i]);
                return;

            default: break;
        }

        if (timerFeedback > 0.0f) timerFeedback -= GetFrameTime();

        // ============================================================
        // desenho
        // ============================================================
        BeginDrawing();
        ClearBackground(BLACK);

        if (telaAtual == 0) {
            DrawTexturePro(fundo,
                (Rectangle){0,0,(float)fundo.width,(float)fundo.height},
                (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},
                (Vector2){0,0}, 0.0f, WHITE);
            DrawText("ESC para voltar ao mapa", 20, 680, 20, RED);

        } else if (telaAtual == 1) {
            DrawTexturePro(fundoBalcao,
                (Rectangle){0,0,(float)fundoBalcao.width,(float)fundoBalcao.height},
                (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},
                (Vector2){0,0}, 0.0f, WHITE);
            DrawText("ESC para voltar",   20,  680, 20, RED);
            DrawText("Comprar Album",     180, 488, 22, DARKBROWN);
            DrawText("Comprar Pacotes",   180, 530, 22, DARKBROWN);
            DrawText("Balcao de Trocas",  180, 572, 22, DARKBROWN);
            DrawText("Vender Figurinhas", 180, 614, 22, DARKBROWN);
            DrawText("Sair da Padaria",   180, 656, 22, RED);

            if (timerFeedback > 0.0f) {
                int larg = MeasureText(msgFeedback, 24);
                int px   = (GetScreenWidth() - larg) / 2;
                DrawRectangle(px - 20, 460, larg + 40, 45, Fade(BLACK, 0.75f));
                DrawText(msgFeedback, px, 470, 24, corFeedback);
            }

        } else if (telaAtual == 2) {
            DrawTexturePro(fundoComprar,
                (Rectangle){0,0,(float)fundoComprar.width,(float)fundoComprar.height},
                (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},
                (Vector2){0,0}, 0.0f, WHITE);
            char texCursor[18];
            sprintf(texCursor, (int)(GetTime()*2)%2==0 ? "%s|" : "%s", inputTexto);
            DrawText(texCursor, 342, 600, 26, DARKBROWN);
            if (inputLen > 0) {
                char total[64];
                sprintf(total, "Total: R$ %.2f", atoi(inputTexto) * padaria->precoPacote);
                DrawText(total, 345, 558, 22, DARKBROWN);
            }
            DrawText("ESC para voltar", 20, 680, 20, RED);

        } else if (telaAtual == 3) {
            DrawTexturePro(fundoVender,
                (Rectangle){0,0,(float)fundoVender.width,(float)fundoVender.height},
                (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},
                (Vector2){0,0}, 0.0f, WHITE);

            char texCursor[18];
            sprintf(texCursor, (int)(GetTime()*2)%2==0 ? "%s|" : "%s", inputTexto);
            DrawText(texCursor, 342, 600, 26, DARKBROWN);
            if (inputLen > 0) {
                char ganho[64];
                sprintf(ganho, "Ganho: R$ %.2f", atoi(inputTexto) * padaria->precoComprarFigurinhas);
                DrawText(ganho, 345, 558, 22, DARKBROWN);
            }
            DrawText("ESC para voltar", 20, 680, 20, RED);

        } else if (telaAtual == 4) {
            DrawTexturePro(fundoTrocar,
                (Rectangle){0,0,(float)fundoTrocar.width,(float)fundoTrocar.height},
                (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},
                (Vector2){0,0}, 0.0f, WHITE);

            // timer
            int faltaSeg = TEMPO_TROCA - (int)difftime(time(NULL), padaria->ultimaAtualizacao);
            if (faltaSeg < 0) faltaSeg = 0;
            char timerTxt[64];
            sprintf(timerTxt, "Atualiza em: %02d:%02d", faltaSeg/60, faltaSeg%60);
            int lw  = MeasureText(timerTxt, 24);
            int lpx = (GetScreenWidth() - lw) / 2;
            DrawText(timerTxt, lpx+2, 42, 24, BLACK);
            DrawText(timerTxt, lpx,   40, 24, (faltaSeg < 60) ? RED : WHITE);

            // conta repetidas para mostrar ao jogador
            int totalRepetidas = 0;
            for (int j = 0; j < meu_album->quantidade_atual; j++)
                totalRepetidas += meu_album->figurinhas[j].quantidade_repetidas;
            char repTxt[64];
            sprintf(repTxt, "Figurinhas repetidas: %d", totalRepetidas);
            int lrep = MeasureText(repTxt, 20);
            DrawText(repTxt, (GetScreenWidth()-lrep)/2 + 1, 72, 20, BLACK);
            DrawText(repTxt, (GetScreenWidth()-lrep)/2,     70, 20, (totalRepetidas > 0) ? YELLOW : RED);

            // slots
            int centrosTrocada[5] = {165, 400, 635, 865, 1095};
            int offsets[5] = {-40, -40, -40, 0, 5};
            for (int i = 0; i < 5; i++) {
                int centroX = (int)btnTrocar[i].x + (int)btnTrocar[i].width/2 + offsets[i];
                Rectangle destImg = { centroX - imgLarg/2, imgTopoY, imgLarg, imgAltu };

                if (padaria->slotUsado[i]) {
                    int larg = MeasureText("TROCADA", 20);
                    DrawText("TROCADA", centrosTrocada[i] - larg/2, 135, 20, GREEN);
                } else {
                    if (texFigurinhas[i].id != 0) {
                        DrawTexturePro(texFigurinhas[i],
                            (Rectangle){0,0,(float)texFigurinhas[i].width,(float)texFigurinhas[i].height},
                            destImg, (Vector2){0,0}, 0.0f, WHITE);
                    } else {
                        int larg = MeasureText(padaria->FigurinhasNoBalcao[i], 18);
                        DrawText(padaria->FigurinhasNoBalcao[i], centroX-larg/2+1, imgTopoY+imgAltu/2+1, 18, BLACK);
                        DrawText(padaria->FigurinhasNoBalcao[i], centroX-larg/2,   imgTopoY+imgAltu/2,   18, WHITE);
                    }
                }
            }

            if (timerFeedback > 0.0f) {
                int larg = MeasureText(msgFeedback, 24);
                int px   = (GetScreenWidth() - larg) / 2;
                DrawRectangle(px-20, 620, larg+40, 38, Fade(BLACK, 0.75f));
                DrawText(msgFeedback, px, 627, 24, corFeedback);
            }
            DrawText("ESC para voltar", 20, 680, 20, RED);
        }

        // hud
        char hud[64];
        sprintf(hud, "Dinheiro: R$ %.2f", *saldo_jogador);
        DrawText(hud, 22, 22, 26, BLACK); DrawText(hud, 20, 20, 26, YELLOW);
        sprintf(hud, "Album: %s", *tem_album ? "SIM" : "NAO");
        DrawText(hud, 22, 52, 26, BLACK); DrawText(hud, 20, 50, 26, WHITE);
        sprintf(hud, "Pacotes: %d", *pacotes_bolso);
        DrawText(hud, 22, 142, 26, BLACK); DrawText(hud, 20, 140, 26, WHITE);
        int totalRepHud = 0;
        for (int j = 0; j < meu_album->quantidade_atual; j++)
            totalRepHud += meu_album->figurinhas[j].quantidade_repetidas;
        sprintf(hud, "Repetidas: %d", totalRepHud);
        DrawText(hud, 22, 172, 26, BLACK); DrawText(hud, 20, 170, 26, WHITE);

        EndDrawing();
    }

    UnloadTexture(fundo); UnloadTexture(fundoBalcao);
    UnloadTexture(fundoComprar); UnloadTexture(fundoVender);
    UnloadTexture(fundoTrocar);
    for (int i = 0; i < 5; i++)
        if (texFigurinhas[i].id != 0) UnloadTexture(texFigurinhas[i]);
}