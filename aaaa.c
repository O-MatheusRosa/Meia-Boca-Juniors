#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "raylib.h"
#include <math.h> 

#define TEMPO_TROCA 600 // para trocar as figurinhas a cada 10 minutos

// funcao para as funcoes que a padaria ira fazer
typedef struct {
    int qntAlbum;
    float precoAlbum;
    float precoPacote;
    float precoComprarFigurinhas; // qual o valor que a padaria ira pagar em cada figura vendida
    int FigurinhasNoBalcao[5];    // guarda o numero das 5 figurinhas que ira estar disponivel para a troca
    int slotUsado[5];             // slots que ja foi trocados(indisponiveis)
    time_t ultimaAtualizacao;     // guarda o momento exato da ultima atualizacao
} BancaPadaria;

typedef struct {
    float dinero; // mantido a estrutura original do jogador
    float dinheiro;
    int temAlbum;
    int figurinhasPossuidas;
} Jogador;

/*==========================================================================================================
FUNCOES PARA TROCAR AS FIGURINHAS 
============================================================================================================*/

// funcao para gerar 5 novas figurinhas aleatorias para trocar
static void atualizarFigurinhasBalcao(BancaPadaria *b) {
    for (int i = 0; i < 5; i++) {
        b->FigurinhasNoBalcao[i] = (rand() % 981) + 1; // figurinhas de 1 a 981
        b->slotUsado[i] = 0;                           // reseta todos os slots ao atualizar
    }
    b->ultimaAtualizacao = time(NULL); // atualiza o tempo para o momento atual
    printf("[PACODA] O balcão foi atualizado com novas figurinhas para troca!\n");
}

// funcao que identifica se ja passou os 10 minutos
static int verificaTempoDoBalcao(BancaPadaria *b) {
    time_t tempoAtual = time(NULL);
    // difftime calcula a diferenca de segundos entre dois tempos
    double segundosPassados = difftime(tempoAtual, b->ultimaAtualizacao);

    if (segundosPassados >= TEMPO_TROCA) {
        atualizarFigurinhasBalcao(b);
        return 1; // sinaliza que atualizou
    }
    return 0;
}

/*=========================================================================================================
===========================================================================================================*/

// ============================================================
// FUNÇÃO PRINCIPAL DA PADARIA
// Recebe o jogador por ponteiro para modificar dinheiro,
// álbum e figurinhas diretamente.
// ============================================================

void entrarNaPadaria(Jogador *jogador) {

// ===========TODAS AS IMAGENS USADAS NA PADARIA=======================================================================
    // Atenção: Certifique-se de que os arquivos reais no Linux tenham exatamente esses nomes em minúsculo!
    Texture2D fundo = LoadTexture("padaria_principal.png");
    if (fundo.id == 0) printf("[ERRO] Imagem nao encontrada!\n");

    Texture2D fundoBalcao = LoadTexture("padaria_segunda.png");
    if (fundoBalcao.id == 0) printf("[ERRO] Imagem segunda nao encontrada!\n");

    Texture2D fundoComprarPacotes = LoadTexture("padaria_comprarpacotes.png");
    if (fundoComprarPacotes.id == 0) printf("[ERRO] Imagem comprar pacotes nao encontrada!\n");

    Texture2D fundoVenderFigurinhas = LoadTexture("padaria_venderfigurinhas.png");
    if (fundoVenderFigurinhas.id == 0) printf("[ERRO] Imagem vender figurinhas nao encontrada!\n");

    Texture2D fundoTrocarFigurinhas = LoadTexture("padaria_trocarfigurinhas.png");
    if (fundoTrocarFigurinhas.id == 0) printf("[ERRO] Imagem trocar figurinhas nao encontrada!\n");

// ===========================================================================================================

    int telaAtual = 0; // 0 = tela principal, 1 = tela do balcao

    // inicializar a padaria
    BancaPadaria padaria;
    padaria.qntAlbum = 5;
    padaria.precoAlbum = 35.00;
    padaria.precoPacote = 5.00;
    padaria.precoComprarFigurinhas = 0.50;

    // gerar as primeiras figurinas no balcao
    atualizarFigurinhasBalcao(&padaria);

    int opcao = -1, quantidadePacotes = 0, quantidadeVendas = 0;
    float custoTotal, ganhoTotal;

    // variaveis da menssagem de feedback
    char msgFeedback[64] = "";
    Color corFeedback = GREEN;
    float timerFeedback = 0.0f;

    // variaveis do input de texto
    char inputTexto[16] = "";
    int inputLen = 0;

// =============posicoes dos lugares clicaveis na tela===================================

    Rectangle areaBalcao = {230, 200, 600, 350};

    // BOTOES DAS CAIXAS NA SEGUNDA TELA
    Rectangle btn1 = {160, 483, 1100, 40}; // texto em 488
    Rectangle btn2 = {160, 525, 1100, 40}; // texto em 530
    Rectangle btn3 = {160, 567, 1100, 40}; // texto em 572
    Rectangle btn4 = {160, 609, 1100, 40}; // texto em 614
    Rectangle btn5 = {160, 651, 1100, 40}; // texto em 656

    Rectangle btnOkPacotes = {885, 610, 100, 55};

    // BOTOES TROCAR NA TELA DE TROCA
    Rectangle btnTrocar[5] = {
        {108, 545, 170, 50},
        {358, 545, 170, 50},
        {593, 545, 170, 50},
        {800, 545, 170, 50},
        {1028, 545, 170, 50},
    };
// =====================================================================================

    // LOOP PRINCIPAL DA PADARIA
    while (!WindowShouldClose()) {
        // Toda vez que o menu roda, o jogo checa se deu o tempo de atualizar o balcao
        verificaTempoDoBalcao(&padaria);

// ======== AREA CLICAVEL NA PADARIA=================================================================

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (telaAtual == 0 && CheckCollisionPointRec(mouse, areaBalcao)) {
                telaAtual = 1; // abre tela do balcao
            }

            // SEGUNDA TELA: clicar nos botoes
            if (telaAtual == 1) {
                if (CheckCollisionPointRec(mouse, btn1)) opcao = 1;
                if (CheckCollisionPointRec(mouse, btn2)) opcao = 2;
                if (CheckCollisionPointRec(mouse, btn3)) opcao = 3;
                if (CheckCollisionPointRec(mouse, btn4)) opcao = 4;
                if (CheckCollisionPointRec(mouse, btn5)) opcao = 0;
            }
            // botao de OK na tela de comprar pacotes
            if (telaAtual == 2 && CheckCollisionPointRec(mouse, btnOkPacotes)) {
                quantidadePacotes = atoi(inputTexto);
                custoTotal = quantidadePacotes * padaria.precoPacote;
                if (quantidadePacotes > 0 && jogador->dinheiro >= custoTotal) {
                    jogador->dinheiro -= custoTotal;
                    jogador->figurinhasPossuidas += (quantidadePacotes * 5);
                    strcpy(msgFeedback, "Compra realizada com sucesso!");
                    corFeedback = GREEN;
                } else if (quantidadePacotes <= 0) {
                    strcpy(msgFeedback, "Digite uma quantidade valida!");
                    corFeedback = RED;
                } else {
                    strcpy(msgFeedback, "Dinheiro insuficiente!");
                    corFeedback = RED;
                }
                timerFeedback = 3.0f;
                telaAtual = 1;
                inputTexto[0] = '\0';
                inputLen = 0;

            } // if //botao de OK na tela de comprar pacotes

            // botao de OK para vender figurinhas
            if (telaAtual == 3 && CheckCollisionPointRec(mouse, btnOkPacotes)) {
                quantidadeVendas = atoi(inputTexto);
                if (quantidadeVendas > 0 && quantidadeVendas <= jogador->figurinhasPossuidas) {
                    ganhoTotal = quantidadeVendas * padaria.precoComprarFigurinhas;
                    jogador->figurinhasPossuidas -= quantidadeVendas;
                    jogador->dinheiro += ganhoTotal;
                    strcpy(msgFeedback, "Venda realizada com sucesso!");
                    corFeedback = GREEN;
                } else if (quantidadeVendas <= 0) {
                    strcpy(msgFeedback, "Digite uma quantidade valida!");
                    corFeedback = RED;
                } else {
                    strcpy(msgFeedback, "Figurinhas insuficientes!");
                    corFeedback = RED;
                }
                timerFeedback = 3.0f;
                telaAtual = 1;
                inputTexto[0] = '\0';
                inputLen = 0;

            } // if //botao de OK na tela de vender pacotes

            // TELA DE TROCAR FIGURINHAS - CLICAR NOS BOTOES DE TROCAR
            if (telaAtual == 4) {
                for (int i = 0; i < 5; i++) {
                    if (CheckCollisionPointRec(mouse, btnTrocar[i])) {
                        if (padaria.slotUsado[i]) {
                            // slot ja foi trocado
                            strcpy(msgFeedback, "Essa figurinha já foi trocada!");
                            corFeedback = RED;
                            timerFeedback = 3.0f;
                        } else if (jogador->figurinhasPossuidas <= 0) {
                            strcpy(msgFeedback, "Sem figurinhas para trocar!");
                            corFeedback = RED;
                            timerFeedback = 3.0f;
                        } else {
                            // troca realizada
                            printf("[SUCESSO] Trocou pela FIGURINHA %d!\n", padaria.FigurinhasNoBalcao[i]);
                            jogador->figurinhasPossuidas--; // gasta uma figurinha
                            jogador->figurinhasPossuidas++; // ganha uma figurinha
                            // ajuste conforme a logia de album
                            padaria.slotUsado[i] = 1; // marca o slot usado/indisponivel
                            strcpy(msgFeedback, "Troca realizada com sucesso!");
                            corFeedback = GREEN;
                            timerFeedback = 3.0f;
                        }
                    }
                }
            }

        } // if principal do botao do mouse

// ===================================================================================================

        // voltar com esc
        if (IsKeyPressed(KEY_ESCAPE) && telaAtual == 1) telaAtual = 0;
        if (IsKeyPressed(KEY_ESCAPE) && telaAtual == 2) {
            telaAtual = 1;
            inputTexto[0] = '\0';
            inputLen = 0;
        }

        // voltar com ESC na tela de vender figurinhas
        if (IsKeyPressed(KEY_ESCAPE) && telaAtual == 3) {
            telaAtual = 1;
            inputTexto[0] = '\0';
            inputLen = 0;
        }

        if (IsKeyPressed(KEY_ESCAPE) && telaAtual == 4) {
            telaAtual = 1;
        }

        // ESC na tela principal da padaria volta para o MAPA
        if (IsKeyPressed(KEY_ESCAPE) && telaAtual == 0) {
            // descarrega as texturas e sai da funcao voltando ao mapa
            UnloadTexture(fundo);
            UnloadTexture(fundoBalcao);
            UnloadTexture(fundoComprarPacotes);
            UnloadTexture(fundoVenderFigurinhas);
            UnloadTexture(fundoTrocarFigurinhas);
            return; // retorna ao loop do mapa
        }

// ===============CAPTURAR DIGITACAO ===========================================
        // tela de comprar pacotes
        if (telaAtual == 2) {
            int tecla = GetCharPressed();
            while (tecla > 0) {
                if (tecla >= '0' && tecla <= '9' && inputLen < 5) {
                    inputTexto[inputLen] = (char)tecla;
                    inputLen++;
                    inputTexto[inputLen] = '\0';
                }
                tecla = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputLen > 0) {
                inputLen--;
                inputTexto[inputLen] = '\0';
            }
        }

        // tela de vender figurinhas
        if (telaAtual == 3) {
            int tecla = GetCharPressed();
            while (tecla > 0) {
                if (tecla >= '0' && tecla <= '9' && inputLen < 5) {
                    inputTexto[inputLen] = (char)tecla;
                    inputLen++;
                    inputTexto[inputLen] = '\0';
                }
                tecla = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputLen > 0) {
                inputLen--;
                inputTexto[inputLen] = '\0';
            }
        }

// ==========================================================================================================

        switch (opcao) {

        case 1:
            if (jogador->temAlbum == 1) {
                strcpy(msgFeedback, "Você já possui o álbum");
                corFeedback = RED;
                timerFeedback = 3.0f;
            } else if (padaria.qntAlbum > 0 && jogador->dinheiro >= padaria.precoAlbum) {
                jogador->dinheiro -= padaria.precoAlbum;
                jogador->temAlbum = 1;
                padaria.qntAlbum--;
                strcpy(msgFeedback, "Compra realizada com sucesso!");
                corFeedback = GREEN;
                timerFeedback = 3.0f;
            } else {
                strcpy(msgFeedback, "Sem dinheiro suficiente!");
                corFeedback = RED;
                timerFeedback = 3.0f;
            }

            opcao = -1;
            break;

        case 2:
            telaAtual = 2;
            inputTexto[0] = '\0';
            inputLen = 0;
            opcao = -1;
            break;

        case 3:
            telaAtual = 4;
            opcao = -1;
            break;

        case 4:
            telaAtual = 3;
            inputTexto[0] = '\0';
            inputLen = 0;
            opcao = -1;
            break;

        case 0:
            printf("Tchau!\n");
            UnloadTexture(fundo);
            UnloadTexture(fundoBalcao);
            UnloadTexture(fundoComprarPacotes);
            UnloadTexture(fundoVenderFigurinhas);
            UnloadTexture(fundoTrocarFigurinhas);
            return; // sai da padaria e volta ao mapa

        default:
            break;
        } // switch

        // atualiza o timer da menssagem
        if (timerFeedback > 0.0f) {
            timerFeedback -= GetFrameTime();
        }

// ===================DESENHO=================================================================================
// ===========================================================================================================
        BeginDrawing();
        ClearBackground(BLACK);

        if (telaAtual == 0) {
            // Tela principal
            DrawTexturePro(fundo,
                           (Rectangle){0, 0, (float)fundo.width, (float)fundo.height},
                           (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                           (Vector2){0, 0}, 0.0f, WHITE);

            // instrucao para voltar ao mapa
            DrawText("ESC para voltar ao mapa", 20, 680, 20, RED);

        } else if (telaAtual == 1) {
            // TELA DO BALCAO
            DrawTexturePro(fundoBalcao,
                           (Rectangle){0, 0, (float)fundoBalcao.width, (float)fundoBalcao.height},
                           (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                           (Vector2){0, 0}, 0.0f, WHITE);

            // TEXTO NAS CAIXAS
            DrawText("ESC para voltar", 20, 680, 20, RED);
            DrawText("Comprar Album", 180, 488, 22, DARKBROWN);
            DrawText("Comprar Pacotes", 180, 530, 22, DARKBROWN);
            DrawText("Balcao de Trocas", 180, 572, 22, DARKBROWN);
            DrawText("Vender Figurinhas", 180, 614, 22, DARKBROWN);
            DrawText("Sair da Padaria", 180, 656, 22, RED);

// ==============TELA DE COMPRAR E VENDER FIGURINHAS========================================================
        } else if (telaAtual == 2) {
            DrawTexturePro(fundoComprarPacotes,
                           (Rectangle){0, 0, (float)fundoComprarPacotes.width, (float)fundoComprarPacotes.height},
                           (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                           (Vector2){0, 0}, 0.0f, WHITE);

            // cursor piscando
            char textoComCursor[18];
            if ((int)(GetTime() * 2) % 2 == 0)
                sprintf(textoComCursor, "%s|", inputTexto);
            else
                sprintf(textoComCursor, "%s", inputTexto);

            DrawText(textoComCursor, 342, 600, 26, DARKBROWN); // cursor

            // total em tempo real
            if (inputLen > 0) {
                char totalTexto[64];
                sprintf(totalTexto, "Total: R$ %.2f", atoi(inputTexto) * padaria.precoPacote);
                DrawText(totalTexto, 345, 558, 22, DARKBROWN);
            }

            DrawText("ESC para voltar", 20, 680, 20, RED);

        // TELA 3 - VENDER FIGURINHA
        } else if (telaAtual == 3) {
            DrawTexturePro(fundoVenderFigurinhas,
                           (Rectangle){0, 0, (float)fundoVenderFigurinhas.width, (float)fundoVenderFigurinhas.height},
                           (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                           (Vector2){0, 0}, 0.0f, WHITE);

            // cursor piscando
            char textoComCursor[18];
            if ((int)(GetTime() * 2) % 2 == 0)
                sprintf(textoComCursor, "%s|", inputTexto);
            else
                sprintf(textoComCursor, "%s", inputTexto);

            DrawText(textoComCursor, 342, 600, 26, DARKBROWN);

            // ganho em tempo real
            if (inputLen > 0) {
                char ganhoTexto[64];
                sprintf(ganhoTexto, "Ganho: R$ %.2f", atoi(inputTexto) * padaria.precoComprarFigurinhas);
                DrawText(ganhoTexto, 345, 558, 22, DARKBROWN);
            }

            DrawText("ESC para voltar", 20, 680, 20, RED);

        // TELA 4 DE TROCA DE FIGURINHAS
        } else if (telaAtual == 4) {

            DrawTexturePro(fundoTrocarFigurinhas,
                           (Rectangle){0, 0, (float)fundoTrocarFigurinhas.width, (float)fundoTrocarFigurinhas.height},
                           (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                           (Vector2){0, 0}, 0.0f, WHITE);

            // timer no topo da tela
            int faltaSeg = TEMPO_TROCA - (int)difftime(time(NULL), padaria.ultimaAtualizacao);
            if (faltaSeg < 0) faltaSeg = 0;
            char timerTexto[64];
            sprintf(timerTexto, "Atualiza em: %02d:%02d", faltaSeg / 60, faltaSeg % 60);
            int larguraTimer = MeasureText(timerTexto, 24);
            int posXTimer = (GetScreenWidth() - larguraTimer) / 2;
            DrawText(timerTexto, posXTimer + 2, 42, 24, BLACK); // sombra
            DrawText(timerTexto, posXTimer, 40, 24, (faltaSeg < 60) ? RED : WHITE);

            // numero da figurinha e estado em cada slot
            for (int i = 0; i < 5; i++) {
                if (padaria.slotUsado[i]) {
                    // slot indisponivel: overlayy escuro sobre o card
                    int larg = MeasureText("TROCADA", 20);
                    int centrosTrocada[5] = {165, 400, 635, 865, 1095};
                    int posX = centrosTrocada[i] - larg / 2;
                    DrawText("TROCADA", posX, 135, 20, GREEN);

                } else {
                    // slot disponivel: mostra numero da figurinha
                    char numFig[8];
                    sprintf(numFig, "#%d", padaria.FigurinhasNoBalcao[i]);
                    int centrosX[5] = {175, 410, 645, 875, 1105};
                    int larg = MeasureText(numFig, 20);
                    int textoX = centrosX[i] - larg / 2 - 15;
                    int textoY = (int)btnTrocar[i].y + (int)btnTrocar[i].height + 5;
                    DrawText(numFig, textoX + 1, textoY + 1, 20, BLACK);
                    DrawText(numFig, textoX, textoY, 20, WHITE);
                }
            }
            // feedback de troca na tela
            if (timerFeedback > 0.0f) {
                int larguraMsg = MeasureText(msgFeedback, 24);
                int posX = (GetScreenWidth() - larguraMsg) / 2;
                DrawRectangle(posX - 20, 620, larguraMsg + 40, 38, Fade(BLACK, 0.75f));
                DrawText(msgFeedback, posX, 627, 24, corFeedback);
            }

            DrawText("ESC para voltar", 20, 680, 20, RED);
        }

// =============================================================================================================

        // STATUS DO JOGADOR (aparece nas duas telas)
        char statusDinheiro[64];
        sprintf(statusDinheiro, "Dinheiro: R$ %.2f", jogador->dinheiro);
        DrawText(statusDinheiro, 22, 22, 26, BLACK);
        DrawText(statusDinheiro, 20, 20, 26, YELLOW);

        char statusAlbum[32];
        sprintf(statusAlbum, "Album: %s", jogador->temAlbum ? "SIM" : "NAO");
        DrawText(statusAlbum, 22, 52, 26, BLACK);
        DrawText(statusAlbum, 20, 50, 26, WHITE);

        char statusFigurinhas[64];
        sprintf(statusFigurinhas, "Figurinhas: %d", jogador->figurinhasPossuidas);
        DrawText(statusFigurinhas, 22, 82, 26, BLACK);
        DrawText(statusFigurinhas, 20, 80, 26, WHITE);

        if (timerFeedback > 0.0f && telaAtual == 1) {
            int larguraMsg = MeasureText(msgFeedback, 24);
            int posX = (GetScreenWidth() - larguraMsg) / 2;
            DrawRectangle(posX - 20, 460, larguraMsg + 40, 45, Fade(BLACK, 0.75f));
            DrawText(msgFeedback, posX, 470, 24, corFeedback);
        }

        EndDrawing();

    } // while

    // libera a memoria das texturas adicionais também para evitar memory leak
    UnloadTexture(fundo);
    UnloadTexture(fundoBalcao);
    UnloadTexture(fundoComprarPacotes);
    UnloadTexture(fundoVenderFigurinhas);
    UnloadTexture(fundoTrocarFigurinhas);
}

// ============================================================
// LOOP DO MAPA PRINCIPAL
// Exibe o mapa da cidade. Ao clicar na PACODA, chama
// entrarNaPadaria() passando o jogador por ponteiro.
// ============================================================

void rodarMapa(Jogador *jogador) {

    // carrega a imagem do mapa
    Texture2D mapa = LoadTexture("assets/padaria_mapa.png");
    if (mapa.id == 0) printf("[ERRO] Imagem do mapa nao encontrada!\n");

    // area clicavel da PACODA no mapa (calculada proporcionalmente para 1280x720)
    // a padaria fica no canto superior direito da imagem do mapa
    Rectangle areaPacoda = {831, 56, 306, 221};

    // variaveis do efeito de hover (destaque ao passar o mouse em cima da padaria)
    float pulso = 0.0f;

    // LOOP DO MAPA
    while (!WindowShouldClose()) {

        Vector2 mouse = GetMousePosition();
        pulso += GetFrameTime() * 3.0f; // velocidade do pulso

        // verifica se o mouse esta sobre a padaria para dar feedback visual
        int mouseNaPadaria = CheckCollisionPointRec(mouse, areaPacoda);

        // clique na padaria: entra na funcao da padaria
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseNaPadaria) {
            printf("[MAPA] Entrando na PACODA...\n");
            // descarrega o mapa antes de entrar na padaria para liberar memoria
            UnloadTexture(mapa);
            entrarNaPadaria(jogador);
            // ao retornar da padaria, recarrega o mapa
            mapa = LoadTexture("padaria_mapa.png");
            if (mapa.id == 0) printf("[ERRO] Falha ao recarregar o mapa!\n");
        }

// ===================DESENHO DO MAPA=================================================================================

        BeginDrawing();
        ClearBackground(BLACK);

        // desenha o mapa ocupando toda a tela
        DrawTexturePro(mapa,
                       (Rectangle){0, 0, (float)mapa.width, (float)mapa.height},
                       (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                       (Vector2){0, 0}, 0.0f, WHITE);

        // destaque visual ao passar o mouse sobre a padaria
        if (mouseNaPadaria) {
            // contorno piscante ao redor da padaria
            float alpha = (sinf(pulso) + 1.0f) / 2.0f; // valor entre 0.0 e 1.0
            DrawRectangleLinesEx(areaPacoda, 3, Fade(YELLOW, 0.4f + alpha * 0.6f));

            // tooltip "Clique para entrar" acima da padaria
            const char *tooltip = "Clique para entrar!";
            int largTooltip = MeasureText(tooltip, 18);
            int txTooltip = (int)areaPacoda.x + ((int)areaPacoda.width - largTooltip) / 2;
            int tyTooltip = (int)areaPacoda.y - 30;
            DrawRectangle(txTooltip - 8, tyTooltip - 4, largTooltip + 16, 26, Fade(BLACK, 0.7f));
            DrawText(tooltip, txTooltip, tyTooltip, 18, YELLOW);
        }

        // cursor personalizado: mao ao passar na padaria, seta normal fora
        if (mouseNaPadaria) {
            // desenha um pequeno icone de "porta" no cursor para indicar entrada
            DrawCircle((int)mouse.x, (int)mouse.y, 6, Fade(YELLOW, 0.8f));
            DrawCircleLines((int)mouse.x, (int)mouse.y, 8, YELLOW);
        }

        // STATUS DO JOGADOR visivel no mapa
        char statusDinheiro[64];
        sprintf(statusDinheiro, "Dinheiro: R$ %.2f", jogador->dinheiro);
        DrawText(statusDinheiro, 12, 12, 22, BLACK); // sombra
        DrawText(statusDinheiro, 10, 10, 22, YELLOW);

        char statusFigurinhas[64];
        sprintf(statusFigurinhas, "Figurinhas: %d", jogador->figurinhasPossuidas);
        DrawText(statusFigurinhas, 12, 38, 22, BLACK); // sombra
        DrawText(statusFigurinhas, 10, 36, 22, WHITE);

        EndDrawing();

    } // while mapa

    // libera o mapa ao sair do jogo
    UnloadTexture(mapa);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand((unsigned int)time(NULL));

    // ABRE A JANELA UMA UNICA VEZ aqui no main
    // a padaria nao abre mais janela propria, usa a mesma janela
    InitWindow(1280, 720, "PACODA");
    SetExitKey(KEY_NULL); // desativa o ESC para fechar o jogo
    ClearWindowState(FLAG_WINDOW_MAXIMIZED);
    SetWindowPosition(100, 100); // posiciona a janela no canto superior esquerdo
    SetTargetFPS(60);

    Jogador jogador = {0.0f, 100.00f, 0, 20}; // preenchimento correto da struct iniciada

    // inicia pelo mapa, que por sua vez chama entrarNaPadaria quando necessario
    rodarMapa(&jogador);

    CloseWindow();
    return 0;
}