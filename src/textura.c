#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "../include/textura.h"

int Tela_Home(Texture2D fundo, Music musica){

    Rectangle botao_taca = { 570, 250, 150, 250 };

    while (!WindowShouldClose()){
        UpdateMusicStream(musica);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 mouse = GetMousePosition();//pega o x e y da ponta do mouse
            
            if (CheckCollisionPointRec(mouse,botao_taca)){
                return 1;
            }//caso clico na taça            
        }//butaum pressionado
        
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(fundo,0,0,WHITE);
        //DrawRectangleRec(botao_taca, Fade(RED,0.5f));//um esboço pra saber onde ta o butaum
        EndDrawing();
    }//final da fnc
    return 0;
}//parte grafica do menu

void Tela_Jogo(Texture2D fundo_jogo, Music musica){

    Rectangle utfpr = { 655, 550, 670, 200};
    while (!WindowShouldClose()){
        UpdateMusicStream(musica);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(fundo_jogo,0,0,WHITE);
        DrawRectangleRec(utfpr, Fade(RED,0.5f));//um esboço pra saber onde ta o butaum
        EndDrawing();
    }//while
}//parte grafica do jogo em si, o mapinha dele

