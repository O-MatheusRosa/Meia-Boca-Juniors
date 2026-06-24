#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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


    
    float saldo_jogador = 20.00; // boni arruma aq dps fznd favor!!!!!!!

    //----------------------- estado da tela grafica do album -----------------------
    bool mostrarAlbum = false; //comeca fechado, abre/fecha com a tecla A
    int paginaAlbum = 0;       //pagina atual do album, lembrada enquanto o jogo roda
    //---------------------------------------------------------------------------------


    // -------------------------------------------------------
    // AREA DA PADARIA no mapa (ajuste os pontos conforme
    // a posicao real da padaria no seu mapinha.png)
    // -------------------------------------------------------
    Rectangle areaPadaria = { 831, 56, 306, 221 };

    // --- Estado do jogador ---
    
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


     


        //tecla A abre ou fecha o album, igual um toggle
        if (IsKeyPressed(KEY_A)) {
            mostrarAlbum = !mostrarAlbum;
        }//if abre/fecha album

        if (IsKeyPressed(KEY_P)) {
        Animacao_AbrirPacotinho(meu_album, catalogo_geral);
         }

        if (!mostrarAlbum && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

            Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointPoly(mouse, utfpr, quantidade_pontos)) {
                ExecutarModuloMonitoria(&saldo_jogador, tempoJogo.deDia);
            }

            if (CheckCollisionPointPoly(mouse, diltu, pontos_diltu)) {
                Tela_Diltu(musica, &saldo_jogador, tempoJogo.deDia);
            }

            // <<< NOVO: clicar na padaria chama Entra_Padaria >>>
            if (CheckCollisionPointRec(mouse, areaPadaria)) {
                Entra_Padaria(&padaria, catalogo_geral, &saldo_jogador, &tem_album, &pacotes_bolso, &figurinhas_bolso);
            }
        }

        // --- Desenho do mapa ---
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
}//fnc



void Animacao_AbrirPacotinho(Album *meu_album, Album *catalogo_geral){ 
    int fase_pacote = 0; 
    
    // --- 1. SORTEIO DAS 5 CARTAS (De 0 a 980) ---
    int indices_sorteados[5];
    
    for (int i = 0; i < 5; i++) {
        int numero_sorteado;
        bool repetido;
        
        do {
            repetido = false;
            numero_sorteado = rand() % 981; 
            
            for (int j = 0; j < i; j++) {
                if (indices_sorteados[j] == numero_sorteado) {
                    repetido = true;
                    break; 
                }
            }
        } while (repetido == true);
        
        indices_sorteados[i] = numero_sorteado;
    }

    Texture2D texturas_figurinhas[5];
    int estado_carta[5] = {0, 0, 0, 0, 0}; 
    float escala_x[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};


    for (int i = 0; i < 5; i++) {
        int id_da_carta = indices_sorteados[i];
        
        const char* codigo_sujo = catalogo_geral->figurinhas[id_da_carta].codigo; 
        

        char codigo_limpo[15];
        strcpy(codigo_limpo, codigo_sujo);

        
        
        for(int k = 0; k < strlen(codigo_limpo); k++) {
            if(codigo_limpo[k] == ' ' || codigo_limpo[k] == '\r' || codigo_limpo[k] == '\n') {
                codigo_limpo[k] = '\0';
                break; 
            }
        }
        

        const char* caminho = TextFormat("assets/figurinhas/%s.png", codigo_limpo);
        texturas_figurinhas[i] = LoadTexture(caminho);
    }

    Texture2D textura_costas = LoadTexture("assets/figurinhas/costas.png");

    Image img_pacote = LoadImage("assets/pacotinho.png");
    ImageColorReplace(&img_pacote, WHITE, BLANK);
    Texture2D textura_pacote = LoadTextureFromImage(img_pacote);
    UnloadImage(img_pacote);

    int largura_fig = 180;  
    int altura_fig = 250;
    int espacamento = 40;
    int margem_x = (1280 - (5 * largura_fig + 4 * espacamento)) / 2; 

    Rectangle hitbox_pacote = { 540, 210, 200, 300 }; 

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) break;

        BeginDrawing();
        
        Color cor_topo = { 15, 45, 100, 255 }; // Azul royal escuro
        Color cor_base = { 5, 10, 25, 255 };   // Quase preto
        DrawRectangleGradientV(0, 0, 1280, 720, cor_topo, cor_base);

        float tempo = GetTime();
        float brilho_luz = 0.3f + sin(tempo * 2.0f) * 0.2f;
        DrawCircleGradient(640, 360, 450, Fade(SKYBLUE, brilho_luz), BLANK);

        if (fase_pacote == 0) {

            DrawText("ABRA SEU PACOTE!", 443, 103, 40, Fade(BLACK, 0.7f));
            DrawText("ABRA SEU PACOTE!", 440, 100, 40, GOLD);
            
            float variacao_y = sin(tempo * 3.0f) * 10.0f; 
            float rotacao = 0.0f;
            
            if (CheckCollisionPointRec(mouse, hitbox_pacote)) {
                rotacao = sin(tempo * 40.0f) * 3.0f; 
            }

            Rectangle fonte_pacote = { 0, 0, textura_pacote.width, textura_pacote.height };
            
            Rectangle destino_pacote = { 
                hitbox_pacote.x + hitbox_pacote.width / 2, 
                hitbox_pacote.y + hitbox_pacote.height / 2 + variacao_y, 
                hitbox_pacote.width, 
                hitbox_pacote.height 
            };
            Vector2 origem = { hitbox_pacote.width / 2, hitbox_pacote.height / 2 };

            Rectangle destino_sombra = destino_pacote;
            destino_sombra.x += 15; // Desloca a sombra pra direita
            destino_sombra.y += 15; // Desloca a sombra pra baixo

            DrawTexturePro(textura_pacote, fonte_pacote, destino_sombra, origem, rotacao, Fade(BLACK, 0.6f));

            DrawTexturePro(textura_pacote, fonte_pacote, destino_pacote, origem, rotacao, WHITE);

            if (CheckCollisionPointRec(mouse, hitbox_pacote) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                fase_pacote = 1; 
            }
        }
        else if (fase_pacote == 1) {
            
            DrawText("CLIQUE NAS CARTAS PARA REVELAR!", 310, 80, 40, GOLD);

            int qtd_reveladas = 0; 

            for (int i = 0; i < 5; i++) {
                int pos_x = margem_x + i * (largura_fig + espacamento);
                int pos_y = 220; 

                float offset_y = 0.0f;
                Rectangle hitbox_mouse = { pos_x, pos_y, largura_fig, altura_fig };
                
                if (estado_carta[i] == 0 && CheckCollisionPointRec(mouse, hitbox_mouse)) {
                    offset_y = -15.0f; 
                }

                float largura_atual = largura_fig * escala_x[i];

                float pos_x_centro = pos_x + (largura_fig - largura_atual) / 2.0f; 

                Rectangle destino = { pos_x_centro, pos_y + offset_y, largura_atual, altura_fig };
                
                // ESTADO 0: PARADA DE COSTAS
                if (estado_carta[i] == 0) { 
                    Rectangle fonte_costas = { 0, 0, textura_costas.width, textura_costas.height };

                    // Sombra da carta
                    Rectangle sombra_carta = { destino.x + 10, destino.y + 15, destino.width, destino.height };
                    DrawRectangleRec(sombra_carta, Fade(BLACK, 0.4f));

                    DrawTexturePro(textura_costas, fonte_costas, destino, (Vector2){0,0}, 0.0f, WHITE);
                    
                    if (CheckCollisionPointRec(mouse, hitbox_mouse)) {
                        DrawRectangleLinesEx(destino, 4, GOLD); // Brilho de sele��o
                        
                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                            estado_carta[i] = 1; // Inicia a anima��o de encolher!
                        }
                    }
                }

                else if (estado_carta[i] == 1) { 
                    escala_x[i] -= 6.0f * GetFrameTime(); // 6.0f � a velocidade do giro
                    if (escala_x[i] <= 0.0f) {
                        escala_x[i] = 0.0f;
                        estado_carta[i] = 2; // Quando sumir, muda o estado pra crescer a frente!
                    }

                        int id_carta_sorteada = indices_sorteados[i];
                        char* nome_tirado = catalogo_geral->figurinhas[id_carta_sorteada].nome_Jogador;
                        
                        int pos_no_album = Procura_Jogador(meu_album, nome_tirado);
                        
                        if (pos_no_album != -1) { // Se j� existe no seu �lbum...
                            if (meu_album->figurinhas[pos_no_album].colada == 0) {
                                meu_album->figurinhas[pos_no_album].colada = 1;
                            } else {
                                meu_album->figurinhas[pos_no_album].quantidade_repetidas++;
                            }
                        } else {
                           
                        }
                    Rectangle fonte_costas = { 0, 0, textura_costas.width, textura_costas.height };

                    // Sombra da carta
                    Rectangle sombra_carta = { destino.x + 10, destino.y + 15, destino.width, destino.height };
                    DrawRectangleRec(sombra_carta, Fade(BLACK, 0.4f));

                    DrawTexturePro(textura_costas, fonte_costas, destino, (Vector2){0,0}, 0.0f, WHITE);
                }
                else if (estado_carta[i] == 2) { 
                    escala_x[i] += 6.0f * GetFrameTime();
                    if (escala_x[i] >= 1.0f) {
                        escala_x[i] = 1.0f;
                        estado_carta[i] = 3; 
                        
                        
                        int id_da_carta = indices_sorteados[i];
                        const char* codigo_sorteado = catalogo_geral->figurinhas[id_da_carta].codigo;
                        

                        int index_encontrado = -1;
                        for (int j = 0; j < meu_album->quantidade_atual; j++) {
                            if (strcmp(meu_album->figurinhas[j].codigo, codigo_sorteado) == 0) {
                                index_encontrado = j;
                                break;
                            }
                        }
                        

                        if (index_encontrado != -1) {
                            if (meu_album->figurinhas[index_encontrado].colada == 0) {
                                meu_album->figurinhas[index_encontrado].colada = 1;
                            } else {
                                meu_album->figurinhas[index_encontrado].quantidade_repetidas++;
                            }
                        } 

                        else {
                            int qtd = meu_album->quantidade_atual;
                            

                            if (qtd == 0) {

                                meu_album->figurinhas = malloc(1 * sizeof(Dados_Figurinha));
                            } else {

                                meu_album->figurinhas = realloc(meu_album->figurinhas, (qtd + 1) * sizeof(Dados_Figurinha));
                            }
                            
                            meu_album->figurinhas[qtd] = catalogo_geral->figurinhas[id_da_carta];
                            meu_album->figurinhas[qtd].colada = 1;
                            meu_album->figurinhas[qtd].quantidade_repetidas = 0;
                            
                            meu_album->quantidade_atual++; // Atualiza o tamanho do �lbum
                        }
                        // =======================================================
                    }
                    
                    Rectangle fonte_frente = { 0, 0, texturas_figurinhas[i].width, texturas_figurinhas[i].height };
                    DrawTexturePro(texturas_figurinhas[i], fonte_frente, destino, (Vector2){0,0}, 0.0f, WHITE);
                }

                else if (estado_carta[i] == 3) { 
                    Rectangle fonte_frente = { 0, 0, texturas_figurinhas[i].width, texturas_figurinhas[i].height };

                    Rectangle sombra_carta = { destino.x + 10, destino.y + 15, destino.width, destino.height };
                    DrawRectangleRec(sombra_carta, Fade(BLACK, 0.4f));
                    
                    DrawTexturePro(texturas_figurinhas[i], fonte_frente, destino, (Vector2){0,0}, 0.0f, WHITE);
                    
                    int id_da_carta = indices_sorteados[i];
                    DrawText(catalogo_geral->figurinhas[id_da_carta].codigo, pos_x + 50, pos_y + altura_fig + 20, 20, LIGHTGRAY);
                    
                    qtd_reveladas++; 
                }
            }
            
                if (qtd_reveladas == 5) {
                    DrawText("Aperte ENTER ou ESC para fechar.", 440, 600, 20, GRAY);
                }
            }

        EndDrawing();
    }

    for (int i = 0; i < 5; i++) {
        UnloadTexture(texturas_figurinhas[i]);
    }
    UnloadTexture(textura_costas); 
    UnloadTexture(textura_pacote);
}

