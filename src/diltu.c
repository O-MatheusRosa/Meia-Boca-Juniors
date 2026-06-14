#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "../include/textura.h"
typedef struct {
    int grupo;
    char nome[20];   
}Bixo;

Bixo tabela[25] = {
    {1, "Avestruz"}, 
    {2, "Aguia"}, 
    {3, "Burro"}, 
    {4, "Borboleta"},
    {5, "Cachorro"}, 
    {6, "Cabra"}, 
    {7, "Carneiro"}, 
    {8, "Camelo"},
    {9, "Cobra"}, 
    {10, "Coelho"}, 
    {11, "Cavalo"}, 
    {12, "Elefante"},
    {13, "Galo"}, 
    {14, "Gato"}, 
    {15, "Jacare"}, 
    {16, "Leao"},
    {17, "Macaco"}, 
    {18, "Porco"}, 
    {19, "Pavao"}, 
    {20, "Peru"},
    {21, "Touro"}, 
    {22, "Tigre"}, 
    {23, "Urso"}, 
    {24, "Veado"},
    {25, "Vaca"}
};

void Tela_Bixo(float *saldo_jogador){

    int fase_bicho = 0;
    int bicho_selecionado = -1;//vai do 0 ao 25
    int resultado;
    int status_resultado = 0;

    int largura_btn = 90;  
    int altura_btn = 90;
    int margem_x = 345; // Posição X onde começa o Avestruz
    int margem_y = 40; // Posição Y onde começa o Avestruz
    int espaco_x = 35;  // Espaço horizontal entre os bichos
    int espaco_y = 25;  // Espaço vertical entre os bichos


    int cooldown_tela = 30;//tava com bug de tela, pulava o mapa

    Image img_bixo = LoadImage("assets/bixo (2).png"); 
    ImageResize(&img_bixo, 1280, 720);
    Texture2D fundo_bixo = LoadTextureFromImage(img_bixo);
    UnloadImage(img_bixo);

    while (!WindowShouldClose()){
        Vector2 mouse = GetMousePosition();

        if (cooldown_tela > 0) {
            cooldown_tela--;
        }//diminui

        if (IsKeyPressed(KEY_ESCAPE) && fase_bicho == 0 && cooldown_tela == 0){
            break;
        }//sair do jogo do bicho

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(fundo_bixo,0,0,WHITE);

            DrawText(TextFormat("SALDO: R$ %.2f", *saldo_jogador), 260, 635, 30, GREEN);

                if (fase_bicho == 0){

                    for (int i = 0; i < 25; i++){
                        int coluna = i % 5;
                        int linha = i / 5;

                        // Calcula a posição exata da hitbox atual
                        int pos_x = margem_x + (coluna * (largura_btn + espaco_x));
                        int pos_y = margem_y + (linha * (altura_btn + espaco_y));

                        // Cria o retângulo invisível
                        Rectangle hitbox_animal = { pos_x, pos_y, largura_btn, altura_btn };

                        //DrawRectangleLinesEx(hitbox_animal, 2, RED);

                        // Se o mouse bater na hitbox
                        // Se o mouse bater na hitbox E o cooldown tiver zerado
                        if (CheckCollisionPointRec(mouse, hitbox_animal) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && cooldown_tela == 0) {
                            bicho_selecionado = i; 
                            fase_bicho = 1;
                            cooldown_tela = 30;        
                        }//bicho escolhido
                    }//laco for

                }else if(fase_bicho == 1){

                    Rectangle popup = { 440, 260, 400, 200 };
                   
                    DrawText(TextFormat("Bicho: %s", tabela[bicho_selecionado].nome), 50, 300, 30, YELLOW);
                    DrawText("Valor: R$ 3.50", 50, 380, 30, WHITE);

                    Rectangle btn_sim = { 995, 295, 240, 30 };
                    Rectangle btn_nao = { 995, 380, 240, 30 };

                    DrawRectangleRec(btn_sim, BLACK);
                    DrawText("APOSTAR", 1015, 300, 30, WHITE);

                    DrawRectangleRec(btn_nao, BLACK);
                    DrawText("VOLTAR", 1018, 380, 30, WHITE);

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        
                        // Se ele arregar
                        if (CheckCollisionPointRec(mouse, btn_nao)) {
                            fase_bicho = 0;
                        }
                        
                        // confirmar a aposta
                        else if (CheckCollisionPointRec(mouse, btn_sim)) {
                            
                            // Checa se tem grana
                            if (*saldo_jogador >= 3.50) {
                                *saldo_jogador -= 3.50; // O bar agradece
                                
//////////////////////////////////////////////////////////////////////////////////////////////////////////
                                resultado = rand() % 25;

                                if (bicho_selecionado == resultado) {
                                    status_resultado = 1; // o cara é rabudo
                                    *saldo_jogador += 63.00; // o valor
                                } else {
                                    status_resultado = 2; // é muito otario mesmo (perdeu)
                                }

                                fase_bicho = 2;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            } else {; 
                                fase_bicho = 0; 
                            }//n tem grana
                        }//parte monetaria, de gasto 
                    }//mouse

                }else if(fase_bicho == 2){
                    Rectangle popup_res = { 440, 260, 400, 200 };
                    DrawRectangleRec(popup_res, Fade(BLACK, 0.9f));
                    DrawRectangleLinesEx(popup_res, 3, BLUE);

                    DrawText("RESULTADO DO POSTE", 480, 280, 20, WHITE);
                    DrawText(TextFormat("Deu: %02d - %s", tabela[resultado].grupo, tabela[resultado].nome), 480, 320, 30, ORANGE);

                    if (status_resultado == 1) {
                        DrawText("VOCE GANHOU R$ 63.00!", 480, 360, 20, GREEN);
                    } else {
                        DrawText("BURRO DEMAIS, PERDEU R$3.50", 480, 360, 20, RED);
                    }//dinheiro


                    Rectangle btn_ok = { 580, 410, 120, 40 };
                    DrawRectangleRec(btn_ok, DARKBLUE);
                    DrawText("OK", 630, 420, 15, WHITE);

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn_ok)) {
                        fase_bicho = 0;
                    }//if
                }//final de tudo, devolve o dienheiro ganho
                
            EndDrawing();
    }//while    
    UnloadTexture(fundo_bixo);
}//funcao
typedef struct{
    char codigo[4];
    Texture2D bandeira;
}Selecao;

int Pais_Apelao(char* codigo){
    char* fortes[] = {"BRA", "ARG", "GER", "FRA", "ESP", "ENG", "MAR", "BEL", "NED", "POR"};//TOP 10 PAISES RANKING FIFA

    for (int i = 0; i < 10; i++){
        if (strcmp(codigo,fortes[i]) == 0){
            return 1;
        }//pais bão,cuiudo       
    }//for externo, que roda dos 10 paises
    return 0;// SE FOR UM BAGRE IGUAL PARAGUAI KKKKKKKKKKKKKKKKKKKKKKKKK
}//funcao


void Tela_Aposta(Music musica,float *saldo_jogador){
    Rectangle btn_futebol = {20,10,1250,300};
    Rectangle btn_bixo    = {20,350,1250,300};

    int cooldown_tela = 30;//tava com bug de tela, pulava o mapa

    Image imagem_aposta = LoadImage("assets/BET.png");
    ImageResize(&imagem_aposta,1280,720);
    Texture2D textura_aposta = LoadTextureFromImage(imagem_aposta);
    UnloadImage(imagem_aposta);

    PauseMusicStream(musica);
    Music musica_bet = LoadMusicStream("assets/bet.mp3");
    PlayMusicStream(musica_bet);
    SetMusicVolume(musica_bet,0.5f);

    while (!WindowShouldClose()){
        UpdateMusicStream(musica_bet);
        Vector2 mouse = GetMousePosition();

        if (cooldown_tela > 0) {
            cooldown_tela--;
        }//diminui

        if (IsKeyPressed(KEY_ESCAPE)){
            break;
        }//caso queira sair

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && cooldown_tela == 0){
            if (CheckCollisionPointRec(mouse,btn_futebol)){
                //
            }else if(CheckCollisionPointRec(mouse,btn_bixo)){
                Tela_Bixo(saldo_jogador);
                cooldown_tela = 30;
            }//chama a tela do jogo do bixo            
        }//escolhe qual jogo quer

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(textura_aposta, 0, 0, WHITE);

            DrawText(TextFormat("%.2f", *saldo_jogador), 710, 678, 30, WHITE);

            //DrawRectangleLinesEx(btn_bixo, 2, RED); // Pra testar a hitbox
            //DrawRectangleLinesEx(btn_futebol, 2, RED); // Pra testar a hitbox
        EndDrawing();
    }//while

    UnloadTexture(textura_aposta);
    UnloadMusicStream(musica_bet);
    ResumeMusicStream(musica);   
}//fnc de aposta


int Tela_Diltu(Music musica, float *saldo_jogador){
    
    Rectangle hitbox_bet = { 1080, 240, 200, 300 };
    int cooldown_tela = 30;

    Image imagem_dilto = LoadImage("assets/dilto.png");
    ImageResize(&imagem_dilto, 1280, 720);
    Texture2D fundo_dilto = LoadTextureFromImage(imagem_dilto);
    UnloadImage(imagem_dilto);
    
    PauseMusicStream(musica);
    Music musica_dilto = LoadMusicStream("assets/musica_dilto2.mp3");
    PlayMusicStream(musica_dilto);
    SetMusicVolume(musica_dilto, 0.2f);
 
    while (!WindowShouldClose()) {
        UpdateMusicStream(musica_dilto);
        Vector2 mouse = GetMousePosition();

        if (cooldown_tela > 0) {
            cooldown_tela--;
        }//diminui

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            // se clicar na maquina de bet
            if (CheckCollisionPointRec(mouse, hitbox_bet) && cooldown_tela == 0) {
                // chama a fnc de qnt dinehiro tem
                Tela_Aposta(musica_dilto,saldo_jogador); 
                cooldown_tela = 30;
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