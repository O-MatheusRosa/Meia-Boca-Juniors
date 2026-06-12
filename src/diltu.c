#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "../include/textura.h"

typedef struct{
    char codigo[4];
    Texture2D bandeira;
}Selecao;

void Tela_Aposta(float *saldo_jogador){

    int time_selecionado = 0; // 0 = Nenhum, 1 = Time A, 2 = Time B
    int status_aposta = 0;    // 0 = Aguardando, 1 = Ganhou, 2 = Perdeu, 3 = Sem Saldo

    Rectangle hitbox_A = { 70, 240, 250, 200 }; 
    Rectangle hitbox_B = { 960, 240, 250, 200 };
    Rectangle hitbox_bet = { 430, 580, 410, 180 };
    
    Image imagem_bet = LoadImage("assets/bet2.png");
    ImageResize(&imagem_bet, 1280, 720);
    Texture2D fundo_bet = LoadTextureFromImage(imagem_bet);
    UnloadImage(imagem_bet);


    int selecoes = 48;
    char* lista_codigos[] = {"ALG", "ARG", "AUS", "AUT", "BEL", "BIH", "BRA", "CIV", 
                             "COD", "COL", "CPV", "CRO", "CUW", "CZE", "ECU", "EGY", 
                             "ENG", "ESP", "FRA", "GER", "GHA", "HAI", "IRN", "IRQ", 
                             "JOR", "JPN", "KOR", "KSA", "MAR", "NED", "NOR", "NZL", 
                             "PAN", "PAR", "POR", "QAT", "RSA", "SCO", "SEN", "SUI", 
                             "SWE", "TUN", "TUR", "URU", "UZB", "CAN", "MEX", "USA"};
    
    
    //sorteio da partida
    int indiceA = rand() % selecoes;
    int indiceB;
    do{
        indiceB = rand() % selecoes;
    } while (indiceA == indiceB);
    
    Selecao timeA;
    Selecao timeB;
    
    strcpy(timeA.codigo, lista_codigos[indiceA]);
    strcpy(timeB.codigo, lista_codigos[indiceB]);
    
    timeA.bandeira = LoadTexture(TextFormat("assets/bandeiras/%s.png", timeA.codigo));
    timeB.bandeira = LoadTexture(TextFormat("assets/bandeiras/%s.png", timeB.codigo));

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;

        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            // Se clicou na bandeira da esquerda
            if (CheckCollisionPointRec(mouse, hitbox_A)) {
                time_selecionado = 1;
            }
            else if (CheckCollisionPointRec(mouse, hitbox_B)) {
                time_selecionado = 2;
            }else if (CheckCollisionPointRec(mouse, hitbox_bet) && time_selecionado != 0) {
                
                if (*saldo_jogador >= 3.50) {
                    
                    *saldo_jogador -= 3.50; 

                    int roleta = rand() % 100; 
                    if (roleta < 70) {
                        status_aposta = 2; // Perdeu
                    } else {
                        *saldo_jogador += 7.00; 
                        status_aposta = 1; // Ganhou
                    }
                    
                    time_selecionado = 0; // Tira a seleção
                    
                    // ==========================================
                    // A MÁGICA DA TROCA DE PAÍSES ACONTECE AQUI!
                    // ==========================================
                    
                    // 1. Joga as bandeiras velhas fora (MUITO IMPORTANTE pra não travar o PC!)
                    UnloadTexture(timeA.bandeira);
                    UnloadTexture(timeB.bandeira);

                    // 2. Sorteia dois novos números
                    indiceA = rand() % selecoes;
                    do {
                        indiceB = rand() % selecoes;
                    } while (indiceA == indiceB);

                    // 3. Atualiza os nomes nas structs
                    strcpy(timeA.codigo, lista_codigos[indiceA]);
                    strcpy(timeB.codigo, lista_codigos[indiceB]);

                    // 4. Carrega as novas bandeiras pra placa de vídeo
                    timeA.bandeira = LoadTexture(TextFormat("assets/bandeiras/%s.png", timeA.codigo));
                    timeB.bandeira = LoadTexture(TextFormat("assets/bandeiras/%s.png", timeB.codigo));

                } else {
                    status_aposta = 3; // Sem saldo
                }//sem money
            }//tudo relacionado a bet
        }//ato de pressionar o botao

        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawTexture(fundo_bet, 0, 0, WHITE); 

        // Bandeiras
        DrawTexture(timeA.bandeira, 70, 240, WHITE);
        DrawTexture(timeB.bandeira, 960, 240, WHITE);
        
        // Borda verde de seleção
        if (time_selecionado == 1) {
            DrawRectangleLinesEx(hitbox_A, 5, GREEN);
        } else if (time_selecionado == 2) {
            DrawRectangleLinesEx(hitbox_B, 5, GREEN); 
        }

        // Textos da Interface
        DrawText(TextFormat("SALDO: R$ %.2f", *saldo_jogador), 450, 100, 40, GREEN);

        if (status_aposta == 1) {
            DrawText("WIN!! Ganhou R$ 7.00!", 350, 680, 30, YELLOW);
        } else if (status_aposta == 2) {
            DrawText("Perdeu seus R$ 3.50, burro demais", 350, 680, 30, RED);
        } else if (status_aposta == 3) {
            DrawText("Ce ta sem dinheiro cara", 400, 680, 30, GRAY);
        }

        //DrawRectangleLinesEx(hitbox_A, 3, RED);      
        //DrawRectangleLinesEx(hitbox_B, 3, BLUE);     
        //DrawRectangleLinesEx(hitbox_bet, 3, GREEN);

        EndDrawing();
    }//while

    UnloadTexture(fundo_bet);
    UnloadTexture(timeA.bandeira);
    UnloadTexture(timeB.bandeira);

}//fnc de aposta

int Tela_Diltu(Music musica, float *saldo_jogador){
    
    Rectangle hitbox_bet = { 78, 220, 200, 300 };

    Image imagem_dilto = LoadImage("assets/dilto2.png");
    ImageResize(&imagem_dilto, 1280, 720);
    Texture2D fundo_dilto = LoadTextureFromImage(imagem_dilto);
    UnloadImage(imagem_dilto);
    
    PauseMusicStream(musica);
    Music musica_dilto = LoadMusicStream("assets/musica2.mp3");
    PlayMusicStream(musica_dilto);
 
    while (!WindowShouldClose()) {
        UpdateMusicStream(musica_dilto);
        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // se clicar na maquina de bet
            if (CheckCollisionPointRec(mouse, hitbox_bet)) {
                // chama a fnc de qnt dinehiro tem
                Tela_Aposta(saldo_jogador); 
            }//if
        }//if
        
        // Se apertar ESC, sai do bar e volta pra rua
        if (IsKeyPressed(KEY_ESCAPE)) break;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(fundo_dilto, 0, 0, WHITE);
        //DrawRectangleLinesEx(hitbox_bet, 2, RED); // Pra testar a hitbox
        EndDrawing();
    }//while

    UnloadTexture(fundo_dilto);
    UnloadMusicStream(musica_dilto);
    ResumeMusicStream(musica);
}//fnc