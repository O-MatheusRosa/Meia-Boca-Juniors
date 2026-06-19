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

void Tela_Jogo(Texture2D fundo_dia, Texture2D fundo_noite, Music musica) {


    // 1. �REA DA UTFPR (Canto Inferior Direito)
    Vector2 utfpr[] = {

        { 780, 430 },  // Ponto 1: Quina superior esquerda
        { 1280, 430 }, // Ponto 2: Vai reto acompanhando a rua
        { 1280, 720 }, // Ponto 3: Desce at� o ch�o direito
        { 710, 720 },  // Ponto 4: Volta pelo ch�o at� a rua
        { 710, 500 },  // Ponto 5: Sobe reto acompanhando a cal�ada
        { 780, 430 }   // Ponto 6: Faz a diagonal da rotat�ria e fecha
    };
    int quantidade_pontos = 6;

    // 2. �REA DO DILTO (Canto Inferior Esquerdo)
    Vector2 diltu[] = {
        { 0, 430 },    // Ponto 1: Borda esquerda da tela
        { 500, 430 },  // Ponto 2: Vai reto at� a curvinha
        { 570, 500 },  // Ponto 3: Desce a diagonal da cal�ada
        { 570, 720 },  // Ponto 4: Desce reto acompanhando a rua
        { 0, 720 },    // Ponto 5: Volta pelo ch�o at� a borda
        { 0, 430 }     // Ponto 6: Sobe pela borda e fecha
    };
    int pontos_diltu = 6;
    
    float saldo_jogador = 20.00; // boni arruma aq dps fznd favor!!!!!!!

    Relogio tempoJogo;
    tempoJogo.tempoAcumulado = 0.0f;
    tempoJogo.duracaoTurno = 60.0f; // 6 minutos para virar o turno
    tempoJogo.deDia = true;          // Come�a de dia
    tempoJogo.horaGame = 6;          // Come�a as 6h da manh�

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



        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
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

        EndDrawing();
    }
}

int Tela_Home(Texture2D fundo, Music musica_menu) {
    
    const char* textoPrincipal = "Congratulations, you're moving to the next level.\nYour next opponent is...";
    const char* textoSecundario = "YOU"; 
    
    int tamanhoTexto = TextLength(textoPrincipal);
    int letrasDesenhadas = 0; 
    int frameCounter = 0;     

    int delayDrop = 0;
    bool dropAconteceu = false; 

    int piscarJogar = 0;
    bool mostrarJogar = false;

    float tempoAnterior = 0.0f;

    Font fontePixel = LoadFontEx("assets/minecraft.ttf", 60, 0, 250);
    
    while (!WindowShouldClose()) {
        
        UpdateMusicStream(musica_menu);

        float tempoMusica = GetMusicTimePlayed(musica_menu);

        if (tempoMusica < tempoAnterior) {
            letrasDesenhadas = 0;
            frameCounter = 0;
            delayDrop = 0;
            dropAconteceu = false;
            piscarJogar = 0;
            mostrarJogar = false;
        }
        tempoAnterior = tempoMusica;

        if (letrasDesenhadas < tamanhoTexto) {
            frameCounter++;
            if (frameCounter >= 3) { 
                frameCounter = 0;
                letrasDesenhadas++;
            }
        } 
        else if (!dropAconteceu) {
            delayDrop++;
            if (delayDrop >= 40) { 
                dropAconteceu = true;
            }
        }

        if (dropAconteceu) {
            piscarJogar++;
            if (piscarJogar > 30) {
                mostrarJogar = !mostrarJogar;
                piscarJogar = 0;
            }
            
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                UnloadFont(fontePixel); 
                return 1;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexture(fundo, 0, 0, WHITE);

        DrawTextEx(fontePixel, TextSubtext(textoPrincipal, 0, letrasDesenhadas), (Vector2){ 50, 20 }, 40, 2, LIGHTGRAY);

        if (dropAconteceu) {
            DrawTextEx(fontePixel, textoSecundario, (Vector2){ 50, 140 }, 80, 2, MAROON);
            if (mostrarJogar) {
                DrawTextEx(fontePixel, " JOGAR\n[ENTER]", (Vector2){ 1120, 600 }, 30, 2, RED);
            }
        }

        EndDrawing();
    }

    UnloadFont(fontePixel);
    return 0; 
}