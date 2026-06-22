#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "../include/textura.h"

typedef struct {
    float tempoAcumulado;
    float duracaoTurno;  
    bool deDia; 
    int horaGame;
} Relogio;

void Tela_Jogo(Texture2D fundo_dia, Texture2D fundo_noite, Music musica, Album *meu_album) {


    // 1. ÁREA DA UTFPR (Canto Inferior Direito)
    Vector2 utfpr[] = {

        { 780, 430 },  // Ponto 1: Quina superior esquerda
        { 1280, 430 }, // Ponto 2: Vai reto acompanhando a rua
        { 1280, 720 }, // Ponto 3: Desce até o chão direito
        { 710, 720 },  // Ponto 4: Volta pelo chão até a rua
        { 710, 500 },  // Ponto 5: Sobe reto acompanhando a calçada
        { 780, 430 }   // Ponto 6: Faz a diagonal da rotatória e fecha
    };
    int quantidade_pontos = 6;

    // 2. ÁREA DO DILTO (Canto Inferior Esquerdo)
    Vector2 diltu[] = {
        
        { 0, 430 },    // Ponto 1: Borda esquerda da tela
        { 500, 430 },  // Ponto 2: Vai reto até a curvinha
        { 570, 500 },  // Ponto 3: Desce a diagonal da calçada
        { 570, 720 },  // Ponto 4: Desce reto acompanhando a rua
        { 0, 720 },    // Ponto 5: Volta pelo chão até a borda
        { 0, 430 }     // Ponto 6: Sobe pela borda e fecha
    };
    int pontos_diltu = 6;
             // Começa as 6h da manhã

    float saldo_jogador = 20.00; // boni arruma aq dps fznd favor!!!!!!!

    //----------------------- estado da tela grafica do album -----------------------
    bool mostrarAlbum = false; //comeca fechado, abre/fecha com a tecla A
    int paginaAlbum = 0;       //pagina atual do album, lembrada enquanto o jogo roda
    //---------------------------------------------------------------------------------

    Relogio tempoJogo;
    tempoJogo.tempoAcumulado = 0.0f;
    tempoJogo.duracaoTurno = 360.0f; // 6 minutos para virar o turno
    tempoJogo.deDia = true;          // Comeca de dia
    tempoJogo.horaGame = 6;          // Comeca 6 manha

    while (!WindowShouldClose()) {
        UpdateMusicStream(musica);

        tempoJogo.tempoAcumulado += GetFrameTime();
        int horasPassadas = (int)(tempoJogo.tempoAcumulado / (tempoJogo.duracaoTurno / 12.0f));
        
        if (tempoJogo.deDia == true) {
            tempoJogo.horaGame = 6 + horasPassadas; 
        } else {
            tempoJogo.horaGame = 18 + horasPassadas; 
            if (tempoJogo.horaGame >= 24) {
                tempoJogo.horaGame -= 24; 
            }
        }

        if (tempoJogo.tempoAcumulado >= tempoJogo.duracaoTurno) {
            tempoJogo.tempoAcumulado = 0.0f; 
            
            // Inverte o turno
            if (tempoJogo.deDia == true) {
                tempoJogo.deDia = false;
            } else {
                tempoJogo.deDia = true;
            }
        }



        //tecla A abre ou fecha o album, igual um toggle
        if (IsKeyPressed(KEY_A)) {
            mostrarAlbum = !mostrarAlbum;
        }//if abre/fecha album

        if (!mostrarAlbum && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();


            if (CheckCollisionPointPoly(mouse, utfpr, quantidade_pontos)) {
               ExecutarModuloMonitoria(&saldo_jogador, tempoJogo.deDia);//boni
            }

            if (CheckCollisionPointPoly(mouse, diltu, pontos_diltu)) {
               Tela_Diltu(musica, &saldo_jogador, tempoJogo.deDia);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (tempoJogo.deDia == true) {
            DrawTexture(fundo_dia, 0, 0, WHITE);   // Desenha o mapa claro
        } else {
            DrawTexture(fundo_noite, 0, 0, WHITE); // Desenha o mapa noite
        }

        DrawRectangle(10, 10, 140, 40, Fade(BLACK, 0.7f)); 
        DrawText(TextFormat("%02d:00", tempoJogo.horaGame), 20, 20, 20, YELLOW);
        
        if (tempoJogo.deDia == true) {
            DrawText("DIA", 90, 20, 20, SKYBLUE);
        } else {
            DrawText("NOITE", 90, 20, 20, PURPLE);
        }

        //dica na tela pro jogador saber que pode abrir o album
        if (!mostrarAlbum) {
            DrawRectangle(10, 60, 190, 30, Fade(BLACK, 0.7f));
            DrawText("[A] Abrir album", 20, 67, 18, WHITE);
        }

        //se o jogador apertou A, desenha o album por cima de tudo
        if (mostrarAlbum) {
            Desenha_Album(meu_album, &paginaAlbum);
        }//if mostra album

        EndDrawing();
    }
}

int Tela_Home(Texture2D fundo, Music musica_menu) { 

    int piscarJogar = 0;
    bool mostrarJogar = true; 

    Font fontePixel = LoadFontEx("assets/minecraft.ttf", 60, 0, 250);
    
    while (!WindowShouldClose()) {
        
        UpdateMusicStream(musica_menu);

        piscarJogar++;
        if (piscarJogar > 30) {
            mostrarJogar = !mostrarJogar;
            piscarJogar = 0;
        }
        
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            UnloadFont(fontePixel); 
            return 1;
        }

       
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexture(fundo, 0, 0, WHITE);

        if (mostrarJogar) {
            DrawTextEx(fontePixel, " JOGAR\n[ENTER]", (Vector2){ 580, 650 }, 30, 2, YELLOW);
        }//final do if else

        EndDrawing();
    }//while de desenho

    UnloadFont(fontePixel);
    return 0; 
}//fnc