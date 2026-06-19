#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "../include/textura.h"
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//&&&&&&&&&&&&&&&&&&&   BET DE FUTEBOL  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&
void Tela_Futebol(Music musica,float *saldo_jogador){
    int fase_copa = 0;
    int cooldown_tela = 30;//tava com bug de tela, pulava o mapa
    int aviso_sem_grana = 0;

    int aposta_jogo1 = -1;
    int aposta_jogo2 = -1;
    int resultado_jogo1 = -1;
    int resultado_jogo2 = -1;
    float ganhos_totais = 0.0f;
    int gols_j1_e = 0, gols_j1_d = 0; // Gols Jogo 1 (Esquerda e Direita)
    int gols_j2_e = 0, gols_j2_d = 0; // Gols Jogo 2 (Esquerda e Direita)
    
    float tempo_inicio = 0.0;

    char* todos_times[48] = {
        "RSA", "GER", "KSA", "ALG", "ARG", "AUS", "AUT", "BEL", "BIH", "BRA", 
        "CPV", "CAN", "QAT", "COL", "KOR", "CIV", "CRO", "CUR", "EGY", "ECU", 
        "SCO", "ESP", "USA", "FRA", "GHA", "HAI", "NED", "ENG", "IRN", "IRQ", 
        "JPN", "JOR", "MAR", "MEX", "NOR", "NZL", "PAN", "PAR", "POR", "RDC", 
        "CZE", "SEN", "SWE", "SUI", "TUN", "TUR", "URU", "UZB"
    };

    char* nomes_completos[48] = {
        "AFRICA DO SUL", "ALEMANHA", "A. SAUDITA", "ARGELIA", "ARGENTINA", "AUSTRALIA", "AUSTRIA", "BELGICA", "BOSNIA", "BRASIL", 
        "CABO VERDE", "CANADA", "CATAR", "COLOMBIA", "C. DO SUL", "C. MARFIM", "CROACIA", "CURACAU", "EGITO", "EQUADOR", 
        "ESCOCIA", "ESPANHA", "EUA", "FRANCA", "GANA", "HAITI", "HOLANDA", "INGLATERRA", "IRA", "IRAQUE", 
        "JAPAO", "JORDANIA", "MARROCOS", "MEXICO", "NORUEGA", "N. ZELANDIA", "PANAMA", "PARAGUAI", "PORTUGAL", "RD CONGO", 
        "REP. TCHECA", "SENEGAL", "SUECIA", "SUICA", "TUNISIA", "TURQUIA", "URUGUAI", "UZBEQUISTAO"
    };

    //escolhe qlqr time de 0 a 48
    int t1 = rand() % 48;
    
    // 2º Time
    int t2 = rand() % 48;
    while(t2 == t1) {
        t2 = rand() % 48; 
    }
    
    // 3º Time
    int t3 = rand() % 48;
    while(t3 == t1 || t3 == t2) {
        t3 = rand() % 48;
    }
    
    // 4º Time
    int t4 = rand() % 48;
    while(t4 == t1 || t4 == t2 || t4 == t3) {
        t4 = rand() % 48;
    }

    float multiplicador_time1, multiplicador_time2, multiplicador_time3, multiplicador_time4;

    multiplicador_time1 = 1.20f + (rand() % 100) / 100.0f; // Os miozin
    multiplicador_time2 = 2.50f + (rand() % 200) / 100.0f; // Zebra

    //jogo 1
    if (rand() % 2 == 0) { 
        float valor_temporario = multiplicador_time1; 
        multiplicador_time1 = multiplicador_time2; 
        multiplicador_time2 = valor_temporario; 
    }

    // Jogo 2
    multiplicador_time3 = 1.20f + (rand() % 100) / 100.0f; 
    multiplicador_time4 = 2.50f + (rand() % 200) / 100.0f; 
    if (rand() % 2 == 0) { 
        float valor_temporario = multiplicador_time3; 
        multiplicador_time3 = multiplicador_time4; 
        multiplicador_time4 = valor_temporario; 
    }

    Image img_futebol = LoadImage("assets/FUTEBOLBET.png"); 
    ImageResize(&img_futebol, 1280, 720);
    Texture2D fundo_futebol = LoadTextureFromImage(img_futebol);
    UnloadImage(img_futebol);

    PauseMusicStream(musica);
    Music musica_futebol = LoadMusicStream("assets/bet-nova.mp3");
    PlayMusicStream(musica_futebol);
    SetMusicVolume(musica_futebol,0.5f);

    Texture2D bandeiras[48];
    
    for (int i = 0; i < 48; i++) {
        bandeiras[i] = LoadTexture(TextFormat("assets/bandeiras/%s.png", todos_times[i]));
    }//carrega as 48 bandeirinhas
    
    while (!WindowShouldClose()){
        UpdateMusicStream(musica_futebol);
        Vector2 mouse = GetMousePosition();
        
        if (cooldown_tela > 0){
            cooldown_tela--;
        }//tenta arrumar o bug de pular a tela
        
        if (IsKeyReleased(KEY_ESCAPE) && fase_copa == 0 && cooldown_tela == 0) {
            break;
        }//saida
        
    // Jogo 1 (Painel da Esquerda)
    Rectangle hitbox_b1 = { 59, 200, 98, 95 };  // Bandeira 1 (Esquerda)
    Rectangle hitbox_b2 = { 499, 200, 98, 95 }; // Bandeira 2 (Direita)
    
    // Jogo 2 (Painel da Direita)
    Rectangle hitbox_b3 = { 683, 200, 98, 95 }; // Bandeira 3 (Esquerda)
    Rectangle hitbox_b4 = { 1123, 200, 98, 95 };// Bandeira 4 (Direita)

        BeginDrawing();

            DrawTexture(fundo_futebol, 0, 0, WHITE); 

            DrawText(TextFormat("%.2f", *saldo_jogador), 1100, 575, 30, GREEN);
            
            // BANDEIRA 1 (Jogo 1 - Esquerda)
            Rectangle fonte_t1 = { 0, 0, bandeiras[t1].width, bandeiras[t1].height }; // Tamanho real da foto original
            DrawTexturePro(bandeiras[t1], fonte_t1, hitbox_b1, (Vector2){0,0}, 0.0f, WHITE);

            // BANDEIRA 2 (Jogo 1 - Direita)
            Rectangle fonte_t2 = { 0, 0, bandeiras[t2].width, bandeiras[t2].height };
            DrawTexturePro(bandeiras[t2], fonte_t2, hitbox_b2, (Vector2){0,0}, 0.0f, WHITE);

            // BANDEIRA 3 (Jogo 2 - Esquerda)
            Rectangle fonte_t3 = { 0, 0, bandeiras[t3].width, bandeiras[t3].height };
            DrawTexturePro(bandeiras[t3], fonte_t3, hitbox_b3, (Vector2){0,0}, 0.0f, WHITE);

            // BANDEIRA 4 (Jogo 2 - Direita)
            Rectangle fonte_t4 = { 0, 0, bandeiras[t4].width, bandeiras[t4].height };
            DrawTexturePro(bandeiras[t4], fonte_t4, hitbox_b4, (Vector2){0,0}, 0.0f, WHITE);    

            if (fase_copa == 0) {
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && cooldown_tela == 0) {
                    // Jogo 1 (Painel da Esquerda)
                    if (CheckCollisionPointRec(mouse, hitbox_b1)){
                         aposta_jogo1 = 0; 
                    }// 0 = Esquerda
                    else if (CheckCollisionPointRec(mouse, hitbox_b2)){
                         aposta_jogo1 = 1;
                     } // 1 = Direita

                    // Jogo 2 (Painel da Direita)
                    if (CheckCollisionPointRec(mouse, hitbox_b3)){
                        aposta_jogo2 = 0;
                    } 

                    else if (CheckCollisionPointRec(mouse, hitbox_b4)) aposta_jogo2 = 1;
                }//butaum    

                ////////////////////////////////////////////////////////////////////////////////////
                /////////////// nomes dos times embaixo da bandeira  ///////////////////////////////
                DrawText(nomes_completos[t1], 60, 320, 30, WHITE);
                DrawText(nomes_completos[t2], 390, 320, 30, WHITE);
                DrawText(nomes_completos[t3], 684, 320, 30, WHITE);
                DrawText(nomes_completos[t4], 1023, 320, 30, WHITE);

                DrawText(TextFormat("PAGA: %.2fx", multiplicador_time1), 60, 400, 25, GREEN);
                DrawText(TextFormat("PAGA: %.2fx", multiplicador_time2), 390, 400, 25, GREEN);
                DrawText(TextFormat("PAGA: %.2fx", multiplicador_time3), 684, 400, 25, GREEN);
                DrawText(TextFormat("PAGA: %.2fx", multiplicador_time4), 1023, 400, 25, GREEN);

                if (aposta_jogo1 == 0) DrawRectangleLinesEx(hitbox_b1, 5, YELLOW);
                if (aposta_jogo1 == 1) DrawRectangleLinesEx(hitbox_b2, 5, YELLOW);
                if (aposta_jogo2 == 0) DrawRectangleLinesEx(hitbox_b3, 5, YELLOW);
                if (aposta_jogo2 == 1) DrawRectangleLinesEx(hitbox_b4, 5, YELLOW);

                Rectangle btn_apostar = { 540, 540, 200, 60 }; 
                
                int qtd_apostas = 0;
                if (aposta_jogo1 != -1) qtd_apostas++;
                if (aposta_jogo2 != -1) qtd_apostas++;

                DrawRectangleRec(btn_apostar, (qtd_apostas > 0) ? DARKGREEN : GRAY);
                DrawRectangleLinesEx(btn_apostar, 3, WHITE);
                DrawText("APOSTAR", 570, 560, 30, WHITE);

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn_apostar) && cooldown_tela == 0) {
                    
                    float custo_total = qtd_apostas * 3.50; // Cada jogo custa 3.50

                    if (qtd_apostas > 0 && *saldo_jogador >= custo_total) {
                        *saldo_jogador -= custo_total; // Debita a grana
                        
                        tempo_inicio = GetTime(); 
                        fase_copa = 1; 
                    }else{
                        aviso_sem_grana = 120;
                    }
                }//apostou

                if (aviso_sem_grana > 0) {
                    DrawText("SALDO INSUFICIENTE PARA A APOSTA!", 380, 620, 30, RED);
                    aviso_sem_grana--;
                }
            }//aposta em si,primeira parte
            else if (fase_copa == 1) {

                float tempo_passado = GetTime() - tempo_inicio;
                
                int minutos_futebol = (int)(tempo_passado * 10); 
                
                if (minutos_futebol > 90){
                    minutos_futebol = 90;
                }//tempo

                DrawText("JOGO COMECOU! BOLA ROLANDO...", 310, 310, 40, RED);
                DrawText(TextFormat("%02d MINUTOS", minutos_futebol), 550, 370, 40, WHITE);

                Rectangle barra_fundo = { 340, 440, 600, 30 };
                Rectangle barra_enchimento = { 340, 440, (tempo_passado / 9.0) * 600, 30 }; 

                DrawRectangleRec(barra_fundo, DARKGRAY);
                DrawRectangleRec(barra_enchimento, RED);
                DrawRectangleLinesEx(barra_fundo, 3, WHITE);

                if (tempo_passado >= 9.0) {  
                    // Sorteia os vencedores (0 ou 1)
                    resultado_jogo1 = rand() % 2;
                    resultado_jogo2 = rand() % 2;

                    ganhos_totais = 0.0f;
                   
                    if (aposta_jogo1 == resultado_jogo1) {
                        if (resultado_jogo1 == 0) ganhos_totais += (3.50 * multiplicador_time1);
                        else ganhos_totais += (3.50 * multiplicador_time2);
                    }
                    if (aposta_jogo2 == resultado_jogo2) {
                        if (resultado_jogo2 == 0) ganhos_totais += (3.50 * multiplicador_time3);
                        else ganhos_totais += (3.50 * multiplicador_time4);
                    }

                    *saldo_jogador += ganhos_totais;

                    fase_copa = 2;
                }//if final da aposta
            }//apostou, so falta o resultado 
            else if (fase_copa == 2) {

                Rectangle popup = { 340, 150, 600, 450 };
                DrawRectangleRec(popup, Fade(BLACK, 0.95f));
                DrawRectangleLinesEx(popup, 4, GREEN);

                DrawText("FIM DE JOGO!", 520, 180, 40, WHITE);

                DrawText("JOGO 1:", 400, 260, 25, LIGHTGRAY);
                if (resultado_jogo1 == 0) DrawText(TextFormat("VENCEU: %s", nomes_completos[t1]), 520, 260, 25, YELLOW);
                else DrawText(TextFormat("VENCEU: %s", nomes_completos[t2]), 520, 260, 25, SKYBLUE);

                // Mostra quem ganhou o Jogo 2
                DrawText("JOGO 2:", 400, 320, 25, LIGHTGRAY);
                if (resultado_jogo2 == 0) DrawText(TextFormat("VENCEU: %s", nomes_completos[t3]), 520, 320, 25, WHITE);
                else DrawText(TextFormat("VENCEU: %s", nomes_completos[t4]), 520, 320, 25, BLUE);

                // A Hora da Verdade: Deu Red ou Green
                if (ganhos_totais > 0) {
                    DrawText(TextFormat("LUCROU: R$ %.2f", ganhos_totais), 480, 400, 30, GREEN);
                } else {
                    DrawText("DEU RED! BANCA QUEBROU.", 450, 400, 30, RED);
                }//msgzinha do resultado

                // Botão de jogar de novo
                Rectangle btn_ok = { 540, 500, 200, 50 };
                DrawRectangleRec(btn_ok, DARKGRAY);
                DrawRectangleLinesEx(btn_ok, 2, WHITE);
                DrawText("NOVA APOSTA", 565, 515, 20, WHITE);

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn_ok)) {
                    aposta_jogo1 = -1; 
                    aposta_jogo2 = -1; 
                    ganhos_totais = 0.0f;
                    
                    // Sorteia times novos
                    t1 = rand() % 48;
                    t2 = rand() % 48;
                    while(t2 == t1) t2 = rand() % 48;
                    t3 = rand() % 48;
                    while(t3 == t1 || t3 == t2) t3 = rand() % 48;
                    t4 = rand() % 48;
                    while(t4 == t1 || t4 == t2 || t4 == t3) t4 = rand() % 48;



                    multiplicador_time1 = 1.20f + (rand() % 100) / 100.0f; 
                    multiplicador_time2 = 2.50f + (rand() % 200) / 100.0f; 
                    if (rand() % 2 == 0) { float valor_temporario = multiplicador_time1; multiplicador_time1 = multiplicador_time2; multiplicador_time2 = valor_temporario; }

                    multiplicador_time3 = 1.20f + (rand() % 100) / 100.0f; 
                    multiplicador_time4 = 2.50f + (rand() % 200) / 100.0f; 
                    if (rand() % 2 == 0) { float valor_temporario = multiplicador_time3; multiplicador_time3 = multiplicador_time4; multiplicador_time4 = valor_temporario; }

                    fase_copa = 0; // Joga de volta pra Fase 0
                }//bandeirinhas
            }//ultimo else if
        EndDrawing();
    }//while
    UnloadTexture(fundo_futebol);
}//funcao
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
//&&&&&&&&&&&&&&&&&&&   JOGO DO BIXO  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
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

void Tela_Bixo(Music musica,float *saldo_jogador){

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
    int aviso_sem_grana = 0;

    Image img_bixo = LoadImage("assets/bixo (2).png"); 
    ImageResize(&img_bixo, 1280, 720);
    Texture2D fundo_bixo = LoadTextureFromImage(img_bixo);
    UnloadImage(img_bixo);

    PauseMusicStream(musica);
    Music musica_bicho = LoadMusicStream("assets/futebolbet.mp3");
    PlayMusicStream(musica_bicho);
    SetMusicVolume(musica_bicho,0.5f);


    while (!WindowShouldClose()){
        UpdateMusicStream(musica_bicho);
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
                                aviso_sem_grana = 120;
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

                if (aviso_sem_grana > 0) {
                    DrawRectangle(380, 330, 520, 60, Fade(BLACK, 0.8f));
                    DrawText("Ta duro dorme!", 400, 350, 25, RED);
                    aviso_sem_grana--; // Vai diminuindo até dar 0 e sumir
                }
                
            EndDrawing();
    }//while    
    UnloadTexture(fundo_bixo);
}//funcao
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
                Tela_Futebol(musica,saldo_jogador);
                cooldown_tela = 30;
            }else if(CheckCollisionPointRec(mouse,btn_bixo)){
                Tela_Bixo(musica,saldo_jogador);
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

///////////////////////////////////////////////////////////////////////////////////////////////
int Tela_Diltu(Music musica, float *saldo_jogador, bool deDia) {
    
    Rectangle hitbox_bet = { 1080, 240, 200, 300 };
    int cooldown_tela = 30;

    Image imagem_dilto;
    if (deDia == true) {
        imagem_dilto = LoadImage("assets/dilto.png"); // Imagem do bar de dia
    } else {
        imagem_dilto = LoadImage("assets/dilto_noite.png"); // Imagem do bar insalubre
    }
    ImageResize(&imagem_dilto, 1280, 720);
    Texture2D fundo_dilto = LoadTextureFromImage(imagem_dilto);
    UnloadImage(imagem_dilto);
    
    PauseMusicStream(musica);
    Music musica_dilto;

    if (deDia == true) {
        musica_dilto = LoadMusicStream("assets/patioposto.mp3"); // sertanejo
    } else {
        musica_dilto = LoadMusicStream("assets/mandelao.mp3"); // Mandelao
        SetMusicVolume(musica_dilto,0.7f);
    }
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
                // chama a fnc de qnt dinheiro tem
                Tela_Aposta(musica_dilto, saldo_jogador); 
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