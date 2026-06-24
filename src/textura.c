#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "../include/textura.h"


typedef struct {
    float tempoAcumulado;
    float duracaoTurno;
    bool  deDia;
    int   horaGame;
} Relogio;

void Tela_Jogo(Texture2D fundo_dia, Texture2D fundo_noite, Music musica, Album *meu_album, Album *catalogo_geral) {
    // usar meu_album e catalogo_geral aqui{

    // --- Areas do mapa ---
    Vector2 utfpr[] = {
        { 780, 430 }, { 1280, 430 }, { 1280, 720 },
        { 710, 720 }, { 710, 500  }, { 780, 430  }
    };
    int quantidade_pontos = 6;

    Vector2 diltu[] = {
        { 0, 430 }, { 500, 430 }, { 570, 500 },
        { 570, 720}, { 0, 720  }, { 0, 430   }
    };
    int pontos_diltu = 6;


    // -------------------------------------------------------
    // AREA DA PADARIA no mapa (ajuste os pontos conforme
    // a posicao real da padaria no seu mapinha.png)
    // -------------------------------------------------------
    Rectangle areaPadaria = { 831, 56, 306, 221 };

    // --- Estado do jogador ---
    float saldo_jogador    = 20.00f; // boni arruma aq dps fznd favor!!!!!!!
    int   tem_album        = 0;
    int   pacotes_bolso    = 0; // quantidade de pacotes que o jogador possui
    int   figurinhas_bolso = 0;

    // --- Padaria: inicializada aqui para o timer persistir entre visitas ---
    BancaPadaria padaria;
    padaria.qntAlbum               = 5;
    padaria.precoAlbum             = 35.00f;
    padaria.precoPacote            = 5.00f;
    padaria.precoComprarFigurinhas = 0.50f;
    padaria.ultimaAtualizacao      = 0; // forcara atualizacao na primeira entrada

    // --- Relogio ---
    Relogio tempoJogo;
    tempoJogo.tempoAcumulado = 0.0f;
    tempoJogo.duracaoTurno   = 360.0f;
    tempoJogo.deDia          = true;
    tempoJogo.horaGame       = 6;

    while (!WindowShouldClose()) {
        UpdateMusicStream(musica);

        // Atualiza relogio
        tempoJogo.tempoAcumulado += GetFrameTime();
        int horasPassadas = (int)(tempoJogo.tempoAcumulado / (tempoJogo.duracaoTurno / 12.0f));
        if (tempoJogo.deDia) {
            tempoJogo.horaGame = 6 + horasPassadas;
        } else {
            tempoJogo.horaGame = 18 + horasPassadas;
            if (tempoJogo.horaGame >= 24) tempoJogo.horaGame -= 24;
        }
        if (tempoJogo.tempoAcumulado >= tempoJogo.duracaoTurno) {
            tempoJogo.tempoAcumulado = 0.0f;
            tempoJogo.deDia = !tempoJogo.deDia;
        }

        // --- Cliques no mapa ---
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointPoly(mouse, utfpr, quantidade_pontos)) {
                ExecutarModuloMonitoria(&saldo_jogador, tempoJogo.deDia);
            }

            if (CheckCollisionPointPoly(mouse, diltu, pontos_diltu)) {
                Tela_Diltu(musica, &saldo_jogador, tempoJogo.deDia);
            }

            // <<< NOVO: clicar na padaria chama Entra_Padaria >>>
            if (CheckCollisionPointRec(mouse, areaPadaria)) {
                Entra_Padaria(&padaria, &saldo_jogador, &tem_album, &pacotes_bolso, &figurinhas_bolso);
            }
        }

        // --- Desenho do mapa ---
        BeginDrawing();
        ClearBackground(BLACK);

        if (tempoJogo.deDia) {
            DrawTexture(fundo_dia,   0, 0, WHITE);
        } else {
            DrawTexture(fundo_noite, 0, 0, WHITE);
        }

        // HUD: hora
        DrawRectangle(10, 10, 140, 40, Fade(BLACK, 0.7f));
        DrawText(TextFormat("%02d:00", tempoJogo.horaGame), 20, 20, 20, YELLOW);
        DrawText(tempoJogo.deDia ? "DIA" : "NOITE", 90, 20, 20,
                 tempoJogo.deDia ? SKYBLUE : PURPLE);

        // HUD: dinheiro, pacotes e figurinhas no mapa
        char hud[64];
        sprintf(hud, "R$ %.2f", saldo_jogador);
        DrawText(hud, 12, 52, 20, BLACK);
        DrawText(hud, 10, 50, 20, YELLOW);



        EndDrawing();

    } // while
}

// ============================================================
// TELA HOME
// ============================================================
int Tela_Home(Texture2D fundo, Music musica_menu) {
    int  piscarJogar  = 0;
    bool mostrarJogar = true;
    Font fontePixel   = LoadFontEx("assets/minecraft.ttf", 60, 0, 250);

    while (!WindowShouldClose()) {
        UpdateMusicStream(musica_menu);
        piscarJogar++;
        if (piscarJogar > 30) {
            mostrarJogar = !mostrarJogar;
            piscarJogar  = 0;
        }

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            UnloadFont(fontePixel);
            return 1;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(fundo, 0, 0, WHITE);
        if (mostrarJogar) {
            DrawTextEx(fontePixel, " JOGAR\n[ENTER]", (Vector2){580, 650}, 30, 2, YELLOW);
        }
        EndDrawing();
    }

    UnloadFont(fontePixel);
    return 0;
}
