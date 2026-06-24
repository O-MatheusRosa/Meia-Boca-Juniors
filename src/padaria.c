#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "textura.h"


#define TEMPO_TROCA 600 // atualiza o balcao a cada 10 minutos

// ============================================================
// FUNCOES INTERNAS DO BALCAO
// ============================================================

static void atualizarFigurinhasBalcao(BancaPadaria *b) {
    for (int i = 0; i < 5; i++) {
        b->FigurinhasNoBalcao[i] = (rand() % 981) + 1;
        b->slotUsado[i] = 0;
    }
    b->ultimaAtualizacao = time(NULL);
    printf("[PADARIA] Balcao atualizado com novas figurinhas!\n");
}

static void verificaTempoDoBalcao(BancaPadaria *b) {
    double segundosPassados = difftime(time(NULL), b->ultimaAtualizacao);
    if (segundosPassados >= TEMPO_TROCA) {
        atualizarFigurinhasBalcao(b);
    }
}

// ============================================================
// FUNCAO PRINCIPAL DA PADARIA
// Chamada de dentro do Tela_Jogo() quando o jogador clica
// na padaria no mapa.
// ============================================================
void Entra_Padaria(BancaPadaria *padaria, float *saldo_jogador, int *tem_album, int *pacotes_bolso, int *figurinhas_bolso) {

    // --- Carrega texturas ---
    Texture2D fundo              = LoadTexture("assets/padaria_principal.jpeg");
    Texture2D fundoBalcao        = LoadTexture("assets/padaria_segunda.png");
    Texture2D fundoComprar       = LoadTexture("assets/padaria_comprarpacotes.png");
    Texture2D fundoVender        = LoadTexture("assets/padaria_venderfigurinhas.png");
    Texture2D fundoTrocar        = LoadTexture("assets/padaria_trocarfigurinhas.png");

    if (fundo.id == 0)        printf("[ERRO] padaria_principal.png nao encontrada!\n");
    if (fundoBalcao.id == 0)  printf("[ERRO] padaria_segunda.png nao encontrada!\n");
    if (fundoComprar.id == 0) printf("[ERRO] padaria_comprarpacotes.png nao encontrada!\n");
    if (fundoVender.id == 0)  printf("[ERRO] padaria_venderfigurinhas.png nao encontrada!\n");
    if (fundoTrocar.id == 0)  printf("[ERRO] padaria_trocarfigurinhas.png nao encontrada!\n");

    // --- Estado das telas ---
    // 0=entrada  1=menu balcao  2=comprar pacotes  3=vender figurinhas  4=trocar figurinhas
    int telaAtual = 0;
    int opcao     = -1;

    // --- Input de quantidade ---
    char inputTexto[16] = "";
    int  inputLen       = 0;

    // --- Feedback visual ---
    char  msgFeedback[64] = "";
    Color corFeedback     = GREEN;
    float timerFeedback   = 0.0f;

    // --- Areas clikaveis ---
    Rectangle areaBalcao = {230, 200, 600, 350};

    Rectangle btn1 = {160, 483, 1100, 40}; // Comprar Album
    Rectangle btn2 = {160, 525, 1100, 40}; // Comprar Pacotes
    Rectangle btn3 = {160, 567, 1100, 40}; // Balcao de Trocas
    Rectangle btn4 = {160, 609, 1100, 40}; // Vender Figurinhas
    Rectangle btn5 = {160, 651, 1100, 40}; // Sair da Padaria

    Rectangle btnOk = {885, 610, 100, 55};

    Rectangle btnTrocar[5] = {
        {108, 545, 170, 50},
        {358, 545, 170, 50},
        {593, 545, 170, 50},
        {800, 545, 170, 50},
        {1028, 545, 170, 50},
    };

    // ============================================================
    // LOOP DA PADARIA
    // ============================================================
    while (!WindowShouldClose()) {

        verificaTempoDoBalcao(padaria);

        // ---- ENTRADA DE TEXTO (telas 2 e 3) ----
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

        // ---- ESC ----
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (telaAtual == 0) {
                // Sai da padaria e volta ao mapa
                UnloadTexture(fundo);
                UnloadTexture(fundoBalcao);
                UnloadTexture(fundoComprar);
                UnloadTexture(fundoVender);
                UnloadTexture(fundoTrocar);
                return;
            } else if (telaAtual == 1) {
                telaAtual = 0;
            } else if (telaAtual == 2 || telaAtual == 3) {
                telaAtual  = 1;
                inputTexto[0] = '\0';
                inputLen   = 0;
            } else if (telaAtual == 4) {
                telaAtual = 1;
            }
        }

        // ---- CLIQUES DO MOUSE ----
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            // Tela 0: clicar no balcao para entrar
            if (telaAtual == 0 && CheckCollisionPointRec(mouse, areaBalcao)) {
                telaAtual = 1;
            }

            // Tela 1: menu do balcao
            if (telaAtual == 1) {
                if (CheckCollisionPointRec(mouse, btn1)) opcao = 1;
                if (CheckCollisionPointRec(mouse, btn2)) opcao = 2;
                if (CheckCollisionPointRec(mouse, btn3)) opcao = 3;
                if (CheckCollisionPointRec(mouse, btn4)) opcao = 4;
                if (CheckCollisionPointRec(mouse, btn5)) opcao = 0;
            }

            // Tela 2: confirmar compra de pacotes
            if (telaAtual == 2 && CheckCollisionPointRec(mouse, btnOk)) {
                int qtd       = atoi(inputTexto);
                float custo   = qtd * padaria->precoPacote;
                if (qtd <= 0) {
                    strcpy(msgFeedback, "Digite uma quantidade valida!");
                    corFeedback = RED;
                } else if (*saldo_jogador < custo) {
                    strcpy(msgFeedback, "Dinheiro insuficiente!");
                    corFeedback = RED;
                } else {
                    *saldo_jogador  -= custo;
                    *pacotes_bolso  += qtd; // adiciona pacotes, nao figurinhas
                    strcpy(msgFeedback, "Compra realizada com sucesso!");
                    corFeedback = GREEN;
                }
                timerFeedback    = 3.0f;
                telaAtual        = 1;
                inputTexto[0]    = '\0';
                inputLen         = 0;
            }

            // Tela 3: confirmar venda de figurinhas
            if (telaAtual == 3 && CheckCollisionPointRec(mouse, btnOk)) {
                int qtd = atoi(inputTexto);
                if (qtd <= 0) {
                    strcpy(msgFeedback, "Digite uma quantidade valida!");
                    corFeedback = RED;
                } else if (qtd > *figurinhas_bolso) {
                    strcpy(msgFeedback, "Figurinhas insuficientes!");
                    corFeedback = RED;
                } else {
                    *saldo_jogador    += qtd * padaria->precoComprarFigurinhas;
                    *figurinhas_bolso -= qtd;
                    strcpy(msgFeedback, "Venda realizada com sucesso!");
                    corFeedback = GREEN;
                }
                timerFeedback    = 3.0f;
                telaAtual        = 1;
                inputTexto[0]    = '\0';
                inputLen         = 0;
            }

            // Tela 4: trocar figurinhas no balcao
            if (telaAtual == 4) {
                for (int i = 0; i < 5; i++) {
                    if (CheckCollisionPointRec(mouse, btnTrocar[i])) {
                        if (padaria->slotUsado[i]) {
                            strcpy(msgFeedback, "Essa figurinha ja foi trocada!");
                            corFeedback = RED;
                        } else if (*figurinhas_bolso <= 0) {
                            strcpy(msgFeedback, "Sem figurinhas para trocar!");
                            corFeedback = RED;
                        } else {
                            // Gasta 1 figurinha avulsa e ganha 1 do balcao
                            // (a figurinha ganha deve ser registrada no Album pelo Tela_Jogo)
                            printf("[PADARIA] Trocou pela figurinha #%d!\n", padaria->FigurinhasNoBalcao[i]);
                            padaria->slotUsado[i] = 1;
                            // saldo nao muda: e uma troca 1x1
                            strcpy(msgFeedback, "Troca realizada com sucesso!");
                            corFeedback = GREEN;
                        }
                        timerFeedback = 3.0f;
                    }
                }
            }
        } // fim cliques

        // ---- SWITCH DO MENU ----
        switch (opcao) {
            case 1: // Comprar album
                if (*tem_album) {
                    strcpy(msgFeedback, "Voce ja possui o album!");
                    corFeedback = RED;
                } else if (padaria->qntAlbum > 0 && *saldo_jogador >= padaria->precoAlbum) {
                    *saldo_jogador  -= padaria->precoAlbum;
                    *tem_album       = 1;
                    padaria->qntAlbum--;
                    strcpy(msgFeedback, "Album comprado com sucesso!");
                    corFeedback = GREEN;
                } else {
                    strcpy(msgFeedback, "Sem dinheiro suficiente!");
                    corFeedback = RED;
                }
                timerFeedback = 3.0f;
                opcao = -1;
                break;

            case 2: // Ir para tela de comprar pacotes
                telaAtual     = 2;
                inputTexto[0] = '\0';
                inputLen      = 0;
                opcao         = -1;
                break;

            case 3: // Ir para tela de trocar figurinhas
                telaAtual = 4;
                opcao     = -1;
                break;

            case 4: // Ir para tela de vender figurinhas
                telaAtual     = 3;
                inputTexto[0] = '\0';
                inputLen      = 0;
                opcao         = -1;
                break;

            case 0: // Sair da padaria
                UnloadTexture(fundo);
                UnloadTexture(fundoBalcao);
                UnloadTexture(fundoComprar);
                UnloadTexture(fundoVender);
                UnloadTexture(fundoTrocar);
                return;

            default:
                break;
        }

        // Atualiza timer do feedback
        if (timerFeedback > 0.0f) timerFeedback -= GetFrameTime();

        // ============================================================
        // DESENHO
        // ============================================================
        BeginDrawing();
        ClearBackground(BLACK);

        if (telaAtual == 0) {
            DrawTexturePro(fundo,
                (Rectangle){0, 0, (float)fundo.width, (float)fundo.height},
                (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                (Vector2){0, 0}, 0.0f, WHITE);
            DrawText("ESC para voltar ao mapa", 20, 680, 20, RED);

        } else if (telaAtual == 1) {
            DrawTexturePro(fundoBalcao,
                (Rectangle){0, 0, (float)fundoBalcao.width, (float)fundoBalcao.height},
                (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                (Vector2){0, 0}, 0.0f, WHITE);
            DrawText("ESC para voltar",      20,  680, 20, RED);
            DrawText("Comprar Album",        180, 488, 22, DARKBROWN);
            DrawText("Comprar Pacotes",      180, 530, 22, DARKBROWN);
            DrawText("Balcao de Trocas",     180, 572, 22, DARKBROWN);
            DrawText("Vender Figurinhas",    180, 614, 22, DARKBROWN);
            DrawText("Sair da Padaria",      180, 656, 22, RED);

            // Feedback na tela do menu
            if (timerFeedback > 0.0f) {
                int larg = MeasureText(msgFeedback, 24);
                int px   = (GetScreenWidth() - larg) / 2;
                DrawRectangle(px - 20, 460, larg + 40, 45, Fade(BLACK, 0.75f));
                DrawText(msgFeedback, px, 470, 24, corFeedback);
            }

        } else if (telaAtual == 2) {
            DrawTexturePro(fundoComprar,
                (Rectangle){0, 0, (float)fundoComprar.width, (float)fundoComprar.height},
                (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                (Vector2){0, 0}, 0.0f, WHITE);

            // Cursor piscando
            char texCursor[18];
            sprintf(texCursor, (int)(GetTime() * 2) % 2 == 0 ? "%s|" : "%s", inputTexto);
            DrawText(texCursor, 342, 600, 26, DARKBROWN);

            if (inputLen > 0) {
                char total[64];
                sprintf(total, "Total: R$ %.2f", atoi(inputTexto) * padaria->precoPacote);
                DrawText(total, 345, 558, 22, DARKBROWN);
            }
            DrawText("ESC para voltar", 20, 680, 20, RED);

        } else if (telaAtual == 3) {
            DrawTexturePro(fundoVender,
                (Rectangle){0, 0, (float)fundoVender.width, (float)fundoVender.height},
                (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                (Vector2){0, 0}, 0.0f, WHITE);

            char texCursor[18];
            sprintf(texCursor, (int)(GetTime() * 2) % 2 == 0 ? "%s|" : "%s", inputTexto);
            DrawText(texCursor, 342, 600, 26, DARKBROWN);

            if (inputLen > 0) {
                char ganho[64];
                sprintf(ganho, "Ganho: R$ %.2f", atoi(inputTexto) * padaria->precoComprarFigurinhas);
                DrawText(ganho, 345, 558, 22, DARKBROWN);
            }
            DrawText("ESC para voltar", 20, 680, 20, RED);

        } else if (telaAtual == 4) {
            DrawTexturePro(fundoTrocar,
                (Rectangle){0, 0, (float)fundoTrocar.width, (float)fundoTrocar.height},
                (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                (Vector2){0, 0}, 0.0f, WHITE);

            // Timer do balcao
            int faltaSeg = TEMPO_TROCA - (int)difftime(time(NULL), padaria->ultimaAtualizacao);
            if (faltaSeg < 0) faltaSeg = 0;
            char timerTxt[64];
            sprintf(timerTxt, "Atualiza em: %02d:%02d", faltaSeg / 60, faltaSeg % 60);
            int lw  = MeasureText(timerTxt, 24);
            int lpx = (GetScreenWidth() - lw) / 2;
            DrawText(timerTxt, lpx + 2, 42, 24, BLACK);
            DrawText(timerTxt, lpx,     40, 24, (faltaSeg < 60) ? RED : WHITE);

            // Estado de cada slot
            int centrosX[5]       = {175, 410, 645, 875, 1105};
            int centrosTrocada[5] = {165, 400, 635, 865, 1095};
            for (int i = 0; i < 5; i++) {
                if (padaria->slotUsado[i]) {
                    int larg = MeasureText("TROCADA", 20);
                    DrawText("TROCADA", centrosTrocada[i] - larg / 2, 135, 20, GREEN);
                } else {
                    char numFig[8];
                    sprintf(numFig, "#%d", padaria->FigurinhasNoBalcao[i]);
                    int larg  = MeasureText(numFig, 20);
                    int texX  = centrosX[i] - larg / 2 - 15;
                    int texY  = (int)btnTrocar[i].y + (int)btnTrocar[i].height + 5;
                    DrawText(numFig, texX + 1, texY + 1, 20, BLACK);
                    DrawText(numFig, texX,     texY,     20, WHITE);
                }
            }

            // Feedback de troca
            if (timerFeedback > 0.0f) {
                int larg = MeasureText(msgFeedback, 24);
                int px   = (GetScreenWidth() - larg) / 2;
                DrawRectangle(px - 20, 620, larg + 40, 38, Fade(BLACK, 0.75f));
                DrawText(msgFeedback, px, 627, 24, corFeedback);
            }
            DrawText("ESC para voltar", 20, 680, 20, RED);
        }

        // ---- STATUS DO JOGADOR (visivel em todas as telas) ----
        char hud[64];
        sprintf(hud, "Dinheiro: R$ %.2f", *saldo_jogador);
        DrawText(hud, 22, 22, 26, BLACK);
        DrawText(hud, 20, 20, 26, YELLOW);

        sprintf(hud, "Album: %s", *tem_album ? "SIM" : "NAO");
        DrawText(hud, 22, 52, 26, BLACK);
        DrawText(hud, 20, 50, 26, WHITE);

        sprintf(hud, "Pacotes: %d", *pacotes_bolso);
        DrawText(hud, 22, 142, 26, BLACK);
        DrawText(hud, 20, 140, 26, WHITE);

        sprintf(hud, "Figurinhas: %d", *figurinhas_bolso);
        DrawText(hud, 22, 172, 26, BLACK);
        DrawText(hud, 20, 170, 26, WHITE);

        EndDrawing();

    } // while

    // Libera texturas ao fechar a janela
    UnloadTexture(fundo);
    UnloadTexture(fundoBalcao);
    UnloadTexture(fundoComprar);
    UnloadTexture(fundoVender);
    UnloadTexture(fundoTrocar);
}