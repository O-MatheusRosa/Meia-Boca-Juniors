#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "../include/textura.h"

int Tela_Home(Texture2D fundo, Music musica){

    Font fonte_brasil = LoadFontEx("assets/Minecraft.ttf",80,0,0);
    Rectangle botao_jogar = { 560,640,180,70};

    Color dourado = {173, 139, 3, 255};
    Color menos_dourado = {217, 174, 4, 255};

    while (!WindowShouldClose()){
        UpdateMusicStream(musica);

        Vector2 mouse = GetMousePosition();//pega o x e y da ponta do mouse
        bool mouse_em_cima_jogar = CheckCollisionPointRec(mouse,botao_jogar);

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){

            if (mouse_em_cima_jogar) {
                UnloadFont(fonte_brasil);
                return 1; 
            }//caso queira ir pra proxima
        }//butaum pressionado
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(fundo,0,0,WHITE);

            Vector2 posicao_texto = {570,645};

            if (mouse_em_cima_jogar){
                DrawTextEx(fonte_brasil,"JOGAR",posicao_texto,40,2,dourado);
            }else{
                DrawTextEx(fonte_brasil,"JOGAR",posicao_texto,40,2,BLACK);
            }//final do else
            
            //DrawRectangleRec(botao_jogar, Fade(RED,0.5f));//um esbo�o pra saber onde ta o butaum
        EndDrawing();
    }//final da fnc

    UnloadFont(fonte_brasil);
    return 0;
}//parte grafica do menu

void Tela_Jogo(Texture2D fundo_jogo, Music musica){

    // 1. �REA DA UTFPR (Canto Inferior Direito)
    Vector2 utfpr[] = {
        { 780, 430 },  // Ponto 1: Quina superior esquerda (come�o da curva da rotat�ria)
        { 1280, 430 }, // Ponto 2: Vai reto acompanhando a rua at� a borda direita
        { 1280, 720 }, // Ponto 3: Desce at� o ch�o direito da tela
        { 710, 720 },  // Ponto 4: Volta pelo ch�o at� o limite da rua vertical
        { 710, 500 },  // Ponto 5: Sobe reto acompanhando a cal�ada
        { 780, 430 }   // Ponto 6: Faz a diagonal da rotat�ria e fecha
    };
    int quantidade_pontos = 6;

    // 2. �REA DO DILTO (Canto Inferior Esquerdo)
    Vector2 diltu[] = {
        { 0, 430 },    // Ponto 1: Borda esquerda da tela (logo abaixo da rua horizontal)
        { 500, 430 },  // Ponto 2: Vai reto at� come�ar a curvinha da rotat�ria
        { 570, 500 },  // Ponto 3: Desce a diagonal da cal�ada
        { 570, 720 },  // Ponto 4: Desce reto acompanhando a rua vertical at� o ch�o
        { 0, 720 },    // Ponto 5: Volta pelo ch�o at� a borda esquerda da tela
        { 0, 430 }     // Ponto 6: Sobe pela borda esquerda e fecha
    };
    int pontos_diltu = 6;
    float saldo_jogador = 0;//boni arruma aq dps fznd favor!!!!!!!
    //acabei de arumar aqui pink(18/06)
    while (!WindowShouldClose()){
        UpdateMusicStream(musica);

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();

                if (CheckCollisionPointPoly(mouse, utfpr, quantidade_pontos)) {
                    ExecutarModuloMonitoria(&saldo_jogador);
                }//if da utfpr

                if (CheckCollisionPointPoly(mouse, diltu, pontos_diltu)) {
                    Tela_Diltu(musica,&saldo_jogador);
                    
                }//if do diltuu
        }//fnc se clicar no butaum

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(fundo_jogo,0,0,WHITE);
        //DrawLineStrip(diltu, pontos_diltu, RED);//teste pra ver onde � o clicavel o dilto
        DrawLineStrip(utfpr, quantidade_pontos, BLUE);//teste pra ver onde � o clicavel a utfpr
        EndDrawing();
    }//while
}//parte grafica do jogo em si, o mapinha dele