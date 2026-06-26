#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "textura.h"

// Tempo em segundos para o balcao de trocas atualizar as figurinhas (600s = 10 minutos)
#define TEMPO_TROCA 600


// ============================================================
// FUNCOES AUXILIARES DO BALCAO DE TROCAS (usadas internamente)
// ============================================================

// atualizarFigurinhasBalcao
// ENTRA: b (padaria, modificado), catalogo (so leitura)
// SAI:   5 figurinhas sorteadas no balcao, slots zerados, timer atualizado
static void atualizarFigurinhasBalcao(BancaPadaria *b, Album *catalogo) {
    for (int i = 0; i < 5; i++) {
        int idx = rand() % catalogo->quantidade_atual;
        strncpy(b->FigurinhasNoBalcao[i], catalogo->figurinhas[idx].codigo, 14);
        b->FigurinhasNoBalcao[i][14] = '\0';

        // Remove espacos ou quebras de linha que possam vir no codigo
        int len = strlen(b->FigurinhasNoBalcao[i]);
        while (len > 0 && (b->FigurinhasNoBalcao[i][len-1] == ' '  ||
                           b->FigurinhasNoBalcao[i][len-1] == '\r' ||
                           b->FigurinhasNoBalcao[i][len-1] == '\n')) {
            b->FigurinhasNoBalcao[i][--len] = '\0';
        }
        b->slotUsado[i] = 0; // marca slot como disponivel para troca
    }
    b->ultimaAtualizacao = time(NULL); // salva o momento da atualizacao
    printf("[padaria] balcao atualizado com novas figurinhas!\n");
}

// verificaTempoDoBalcao
// ENTRA: b (padaria), catalogo (so leitura)
// SAI:   se 10 min passaram, renova o balcao; caso contrario, nada muda
static void verificaTempoDoBalcao(BancaPadaria *b, Album *catalogo) {
    double segundosPassados = difftime(time(NULL), b->ultimaAtualizacao);
    if (segundosPassados >= TEMPO_TROCA) {
        atualizarFigurinhasBalcao(b, catalogo);
    }
}

// carregarTexFigurinhas
// ENTRA: b (codigos das figurinhas, so leitura), texFigurinhas (array de 5, modificado)
// SAI:   texFigurinhas preenchido com as imagens do balcao (id=0 se nao encontrar)
static void carregarTexFigurinhas(BancaPadaria *b, Texture2D texFigurinhas[5]) {
    for (int i = 0; i < 5; i++) {
        if (texFigurinhas[i].id != 0) UnloadTexture(texFigurinhas[i]); // libera textura antiga
        char caminho[64];
        sprintf(caminho, "assets/figurinhas/%s.png", b->FigurinhasNoBalcao[i]);
        texFigurinhas[i] = LoadTexture(caminho);
        if (texFigurinhas[i].id == 0)
            printf("[erro] figurinha nao encontrada: %s\n", caminho);
    }
}

// acharFigurinhaRepetida
// ENTRA: meu_album (so leitura)
// SAI:   retorna o indice da primeira figurinha repetida, ou -1 se nao tiver nenhuma
static int acharFigurinhaRepetida(Album *meu_album) {
    for (int i = 0; i < meu_album->quantidade_atual; i++) {
        if (meu_album->figurinhas[i].quantidade_repetidas > 0) {
            return i;
        }
    }
    return -1;
}

// limparCodigo
// ENTRA: entrada (codigo original), saida (buffer destino), tamanho (limite do buffer)
// SAI:   saida com o codigo sem espacos, \r ou \n no final
static void limparCodigo(const char *entrada, char *saida, int tamanho) {
    strncpy(saida, entrada, tamanho - 1);
    saida[tamanho - 1] = '\0';
    int len = strlen(saida);
    while (len > 0 && (saida[len-1] == ' ' || saida[len-1] == '\r' || saida[len-1] == '\n'))
        saida[--len] = '\0';
}

// adicionarFigurinhaNoAlbum
// ENTRA: meu_album (modificado), catalogo_geral (so leitura), codigo (figurinha a adicionar)
// SAI:   figurinha colada no album; se ja colada vira repetida; se nova e inserida no vetor
static void adicionarFigurinhaNoAlbum(Album *meu_album, Album *catalogo_geral, const char *codigo) {
    char cod_limpo[15];

    // Verifica se a figurinha ja existe no album do jogador
    for (int i = 0; i < meu_album->quantidade_atual; i++) {
        limparCodigo(meu_album->figurinhas[i].codigo, cod_limpo, 15);
        if (strcmp(cod_limpo, codigo) == 0) {
            if (meu_album->figurinhas[i].colada == 0) {
                meu_album->figurinhas[i].colada = 1; // cola a figurinha no album
            } else {
                meu_album->figurinhas[i].quantidade_repetidas++; // ja colada: conta como repetida
            }
            return;
        }
    }

    // Figurinha nao existe no album: procura no catalogo e adiciona
    for (int i = 0; i < catalogo_geral->quantidade_atual; i++) {
        limparCodigo(catalogo_geral->figurinhas[i].codigo, cod_limpo, 15);
        if (strcmp(cod_limpo, codigo) == 0) {
            int qtd = meu_album->quantidade_atual;
            // Aloca ou expande o vetor de figurinhas do album
            if (qtd == 0) {
                meu_album->figurinhas = malloc(sizeof(Dados_Figurinha));
            } else {
                meu_album->figurinhas = realloc(meu_album->figurinhas, (qtd + 1) * sizeof(Dados_Figurinha));
            }
            meu_album->figurinhas[qtd] = catalogo_geral->figurinhas[i];
            meu_album->figurinhas[qtd].colada = 1;
            meu_album->figurinhas[qtd].quantidade_repetidas = 0;
            meu_album->quantidade_atual++;
            return;
        }
    }
}


// Entra_Padaria  —  funcao principal da padaria
// ENTRA: padaria (precos/balcao, modificado), catalogo_geral (so leitura),
//        meu_album (modificado), saldo_jogador (modificado),
//        tem_album (modificado), pacotes_bolso (modificado), figurinhas_bolso (nao alterado)
// SAI:   saldo/album/pacotes/figurinhas atualizados conforme acoes do jogador;
//        termina quando o jogador sai ou fecha a janela
void Entra_Padaria(BancaPadaria *padaria, Album *catalogo_geral, Album *meu_album,
                   float *saldo_jogador, int *tem_album, int *pacotes_bolso, int *figurinhas_bolso) {

    // Se for a primeira vez entrando na padaria, gera as figurinhas do balcao
    if (padaria->ultimaAtualizacao == 0) {
        atualizarFigurinhasBalcao(padaria, catalogo_geral);
    }


    // ============================================================
    // CARREGAMENTO DE IMAGENS DE FUNDO DE CADA TELA
    // ============================================================
    Texture2D fundo        = LoadTexture("assets/padaria_principal.jpeg");  // tela 0: entrada da padaria
    Texture2D fundoBalcao  = LoadTexture("assets/padaria_segunda.png");     // tela 1: menu principal da padaria
    Texture2D fundoComprar = LoadTexture("assets/padaria_comprarpacotes.png"); // tela 2: comprar pacotes
    Texture2D fundoVender  = LoadTexture("assets/padaria_venderfigurinhas.png"); // tela 3: vender figurinhas
    Texture2D fundoTrocar  = LoadTexture("assets/padaria_trocarfigurinhas.png"); // tela 4: balcao de trocas

    if (fundo.id == 0)        printf("[erro] padaria_principal.png nao encontrada!\n");
    if (fundoBalcao.id == 0)  printf("[erro] padaria_segunda.png nao encontrada!\n");
    if (fundoComprar.id == 0) printf("[erro] padaria_comprarpacotes.png nao encontrada!\n");
    if (fundoVender.id == 0)  printf("[erro] padaria_venderfigurinhas.png nao encontrada!\n");
    if (fundoTrocar.id == 0)  printf("[erro] padaria_trocarfigurinhas.png nao encontrada!\n");

    // Carrega as texturas das figurinhas do balcao de trocas
    Texture2D texFigurinhas[5] = {0};
    carregarTexFigurinhas(padaria, texFigurinhas);
    time_t ultimaTextura = padaria->ultimaAtualizacao; // guarda quando foi carregado pela ultima vez


    // ============================================================
    // VARIAVEIS DE CONTROLE DE ESTADO
    // ============================================================
    int telaAtual = 0; // qual tela esta sendo exibida agora (0=entrada, 1=menu, 2=comprar, 3=vender, 4=trocar)
    int opcao     = -1; // qual opcao do menu foi clicada (-1 = nenhuma)

    char inputTexto[16] = ""; // texto que o jogador digita (quantidade de pacotes/figurinhas)
    int  inputLen       = 0;  // tamanho atual do texto digitado

    char  msgFeedback[64] = ""; // mensagem exibida apos uma acao (ex: "Compra realizada!")
    Color corFeedback     = GREEN; // cor da mensagem de feedback
    float timerFeedback   = 0.0f;  // tempo restante para a mensagem sumir


    // ============================================================
    // AREAS CLIVAVEIS DAS TELAS
    // ============================================================

    // Tela 0: area clicavel para entrar no balcao
    Rectangle areaBalcao = {230, 200, 600, 350};

    // Tela 1: botoes do menu principal
    Rectangle btn1 = {160, 483, 1100, 40}; // Comprar Album
    Rectangle btn2 = {160, 525, 1100, 40}; // Comprar Pacotes
    Rectangle btn3 = {160, 567, 1100, 40}; // Balcao de Trocas
    Rectangle btn4 = {160, 609, 1100, 40}; // Vender Figurinhas
    Rectangle btn5 = {160, 651, 1100, 40}; // Sair da Padaria

    // Botao de confirmar (telas 2 e 3)
    Rectangle btnOk = {885, 610, 100, 55};

    // Tela 4: botoes de trocar figurinha para cada slot do balcao
    Rectangle btnTrocar[5] = {
        {108, 510, 170, 50},
        {358, 510, 170, 50},
        {593, 510, 170, 50},
        {800, 510, 170, 50},
        {1028, 510, 170, 50},
    };

    // Dimensoes das imagens das figurinhas na tela de trocas
    int imgLarg  = 180;
    int imgAltu  = 250;
    int imgTopoY = 200;


    // ============================================================
    // LOOP PRINCIPAL DA PADARIA
    // Roda enquanto a janela estiver aberta
    // ============================================================
    while (!WindowShouldClose()) {

        // A cada frame: verifica se o balcao de trocas precisa ser atualizado
        verificaTempoDoBalcao(padaria, catalogo_geral);

        // Se o balcao foi atualizado, recarrega as texturas das figurinhas
        if (padaria->ultimaAtualizacao != ultimaTextura) {
            carregarTexFigurinhas(padaria, texFigurinhas);
            ultimaTextura = padaria->ultimaAtualizacao;
        }

        // --------------------------------------------------------
        // ENTRADA DE TEXTO (telas 2 e 3: digitar quantidade)
        // So aceita numeros de 0 a 9, maximo 5 digitos
        // --------------------------------------------------------
        if (telaAtual == 2 || telaAtual == 3) {
            int tecla = GetCharPressed();
            while (tecla > 0) {
                if (tecla >= '0' && tecla <= '9' && inputLen < 5) {
                    inputTexto[inputLen++] = (char)tecla;
                    inputTexto[inputLen]   = '\0';
                }
                tecla = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputLen > 0) {
                inputTexto[--inputLen] = '\0'; // apaga o ultimo caractere
            }
        }


        // --------------------------------------------------------
        // TELA 0: ENTRADA DA PADARIA
        // Clique na area do balcao -> vai para o menu principal
        // ESC -> volta ao mapa
        // --------------------------------------------------------
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (telaAtual == 0) {
                // Libera todas as texturas e sai da funcao (volta ao mapa)
                UnloadTexture(fundo); UnloadTexture(fundoBalcao);
                UnloadTexture(fundoComprar); UnloadTexture(fundoVender);
                UnloadTexture(fundoTrocar);
                for (int i = 0; i < 5; i++)
                    if (texFigurinhas[i].id != 0) UnloadTexture(texFigurinhas[i]);
                return;
            } else if (telaAtual == 1) {
                telaAtual = 0; // volta para a entrada da padaria
            } else if (telaAtual == 2 || telaAtual == 3) {
                telaAtual = 1; inputTexto[0] = '\0'; inputLen = 0; // volta ao menu e limpa o input
            } else if (telaAtual == 4) {
                telaAtual = 1; // volta ao menu
            }
        }


        // --------------------------------------------------------
        // DETECCAO DE CLIQUES DO MOUSE
        // --------------------------------------------------------
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            // TELA 0: clica no balcao -> vai para o menu da padaria
            if (telaAtual == 0 && CheckCollisionPointRec(mouse, areaBalcao))
                telaAtual = 1;

            // TELA 1: menu principal -> define qual opcao foi escolhida
            if (telaAtual == 1) {
                if (CheckCollisionPointRec(mouse, btn1)) opcao = 1; // Comprar Album
                if (CheckCollisionPointRec(mouse, btn2)) opcao = 2; // Comprar Pacotes
                if (CheckCollisionPointRec(mouse, btn3)) opcao = 3; // Balcao de Trocas
                if (CheckCollisionPointRec(mouse, btn4)) opcao = 4; // Vender Figurinhas
                if (CheckCollisionPointRec(mouse, btn5)) opcao = 0; // Sair da Padaria
            }

            // --------------------------------------------------------
            // TELA 2: COMPRAR PACOTES
            // Jogador digita a quantidade e clica em OK
            // --------------------------------------------------------
            if (telaAtual == 2 && CheckCollisionPointRec(mouse, btnOk)) {
                int qtd     = atoi(inputTexto);       // converte o texto digitado em numero
                float custo = qtd * padaria->precoPacote; // calcula o custo total

                if (qtd <= 0) {
                    strcpy(msgFeedback, "Digite uma quantidade valida!");
                    corFeedback = RED;
                } else if (*saldo_jogador < custo) {
                    strcpy(msgFeedback, "Dinheiro insuficiente!");
                    corFeedback = RED;
                } else {
                    // Compra realizada: desconta o dinheiro e adiciona os pacotes
                    *saldo_jogador -= custo;
                    *pacotes_bolso += qtd;
                    strcpy(msgFeedback, "Compra realizada com sucesso!");
                    corFeedback = GREEN;
                }
                timerFeedback = 3.0f; // mensagem aparece por 3 segundos
                telaAtual = 1;         // volta ao menu
                inputTexto[0] = '\0'; inputLen = 0; // limpa o campo de texto
            }

            // --------------------------------------------------------
            // TELA 3: VENDER FIGURINHAS REPETIDAS
            // Jogador digita quantas quer vender e clica em OK
            // --------------------------------------------------------
            if (telaAtual == 3 && CheckCollisionPointRec(mouse, btnOk)) {
                int qtd = atoi(inputTexto);

                // Conta quantas figurinhas repetidas o jogador tem no total
                int totalRepetidas = 0;
                for (int j = 0; j < meu_album->quantidade_atual; j++)
                    totalRepetidas += meu_album->figurinhas[j].quantidade_repetidas;

                if (qtd <= 0) {
                    strcpy(msgFeedback, "Digite uma quantidade valida!");
                    corFeedback = RED;
                } else if (qtd > totalRepetidas) {
                    strcpy(msgFeedback, "Figurinhas repetidas insuficientes!");
                    corFeedback = RED;
                } else {
                    // Remove as repetidas do album uma a uma ate completar a quantidade vendida
                    int restante = qtd;
                    for (int j = 0; j < meu_album->quantidade_atual && restante > 0; j++) {
                        int tirar = meu_album->figurinhas[j].quantidade_repetidas;
                        if (tirar > restante) tirar = restante;
                        meu_album->figurinhas[j].quantidade_repetidas -= tirar;
                        restante -= tirar;
                    }
                    // Adiciona o dinheiro ganho ao saldo
                    *saldo_jogador += qtd * padaria->precoComprarFigurinhas;
                    strcpy(msgFeedback, "Venda realizada com sucesso!");
                    corFeedback = GREEN;
                }
                timerFeedback = 3.0f;
                telaAtual = 1;
                inputTexto[0] = '\0'; inputLen = 0;
            }

            // --------------------------------------------------------
            // TELA 4: BALCAO DE TROCAS
            // Jogador clica em um dos 5 slots para trocar uma figurinha repetida
            // pela figurinha exibida naquele slot
            // --------------------------------------------------------
            if (telaAtual == 4) {
                for (int i = 0; i < 5; i++) {
                    if (CheckCollisionPointRec(mouse, btnTrocar[i])) {
                        if (padaria->slotUsado[i]) {
                            // Esse slot ja foi usado nesta rodada do balcao
                            strcpy(msgFeedback, "Essa figurinha ja foi trocada!");
                            corFeedback = RED;
                        } else {
                            // Verifica se o jogador tem alguma figurinha repetida para usar
                            int idxRepetida = acharFigurinhaRepetida(meu_album);
                            if (idxRepetida == -1) {
                                strcpy(msgFeedback, "Sem figurinhas repetidas para trocar!");
                                corFeedback = RED;
                            } else {
                                // Troca realizada:
                                // 1. Remove 1 repetida do album do jogador
                                meu_album->figurinhas[idxRepetida].quantidade_repetidas--;
                                // 2. Adiciona a figurinha do balcao ao album do jogador
                                adicionarFigurinhaNoAlbum(meu_album, catalogo_geral, padaria->FigurinhasNoBalcao[i]);
                                // 3. Marca o slot como usado (nao pode trocar de novo a mesma figurinha)
                                padaria->slotUsado[i] = 1;
                                printf("[padaria] trocou pela figurinha %s!\n", padaria->FigurinhasNoBalcao[i]);
                                strcpy(msgFeedback, "Troca realizada com sucesso!");
                                corFeedback = GREEN;
                            }
                        }
                        timerFeedback = 3.0f;
                    }
                }
            }
        }


        // --------------------------------------------------------
        // PROCESSAMENTO DAS OPCOES DO MENU PRINCIPAL (tela 1)
        // --------------------------------------------------------
        switch (opcao) {

            // OPCAO 1: COMPRAR ALBUM
            // So pode comprar se nao tiver album, se tiver no estoque e se tiver dinheiro
            case 1:
                if (*tem_album) {
                    strcpy(msgFeedback, "Voce ja possui o album!");
                    corFeedback = RED;
                } else if (padaria->qntAlbum > 0 && *saldo_jogador >= padaria->precoAlbum) {
                    *saldo_jogador -= padaria->precoAlbum;
                    *tem_album      = 1;
                    padaria->qntAlbum--;
                    strcpy(msgFeedback, "Album comprado com sucesso!");
                    corFeedback = GREEN;
                } else {
                    strcpy(msgFeedback, "Sem dinheiro suficiente!");
                    corFeedback = RED;
                }
                timerFeedback = 3.0f; opcao = -1; break;

            // OPCAO 2: IR PARA TELA DE COMPRAR PACOTES
            case 2:
                telaAtual = 2; inputTexto[0] = '\0'; inputLen = 0; opcao = -1; break;

            // OPCAO 3: IR PARA TELA DO BALCAO DE TROCAS
            case 3:
                telaAtual = 4; opcao = -1; break;

            // OPCAO 4: IR PARA TELA DE VENDER FIGURINHAS
            case 4:
                telaAtual = 3; inputTexto[0] = '\0'; inputLen = 0; opcao = -1; break;

            // OPCAO 0: SAIR DA PADARIA (libera texturas e retorna)
            case 0:
                UnloadTexture(fundo); UnloadTexture(fundoBalcao);
                UnloadTexture(fundoComprar); UnloadTexture(fundoVender);
                UnloadTexture(fundoTrocar);
                for (int i = 0; i < 5; i++)
                    if (texFigurinhas[i].id != 0) UnloadTexture(texFigurinhas[i]);
                return;

            default: break;
        }

        // Conta regressiva do feedback (diminui com o tempo do frame)
        if (timerFeedback > 0.0f) timerFeedback -= GetFrameTime();


        // ============================================================
        // DESENHO DE CADA TELA
        // ============================================================
        BeginDrawing();
        ClearBackground(BLACK);

        // ------------------------------------------------------------
        // TELA 0: ENTRADA DA PADARIA
        // Exibe o fundo da padaria e instrucao de ESC para voltar ao mapa
        // ------------------------------------------------------------
        if (telaAtual == 0) {
            DrawTexturePro(fundo,
                (Rectangle){0,0,(float)fundo.width,(float)fundo.height},
                (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},
                (Vector2){0,0}, 0.0f, WHITE);
            DrawText("ESC para voltar ao mapa", 20, 680, 20, RED);

        // ------------------------------------------------------------
        // TELA 1: MENU PRINCIPAL DA PADARIA
        // Exibe as opcoes: album, pacotes, trocas, vender, sair
        // Tambem exibe mensagem de feedback se houver
        // ------------------------------------------------------------
        } else if (telaAtual == 1) {
            DrawTexturePro(fundoBalcao,
                (Rectangle){0,0,(float)fundoBalcao.width,(float)fundoBalcao.height},
                (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},
                (Vector2){0,0}, 0.0f, WHITE);
            DrawText("ESC para voltar",   20,  680, 20, RED);
            DrawText("Comprar Album",     180, 488, 22, DARKBROWN);
            DrawText("Comprar Pacotes",   180, 530, 22, DARKBROWN);
            DrawText("Balcao de Trocas",  180, 572, 22, DARKBROWN);
            DrawText("Vender Figurinhas", 180, 614, 22, DARKBROWN);
            DrawText("Sair da Padaria",   180, 656, 22, RED);

            // Exibe a mensagem de feedback centralizada (se ainda estiver no tempo)
            if (timerFeedback > 0.0f) {
                int larg = MeasureText(msgFeedback, 24);
                int px   = (GetScreenWidth() - larg) / 2;
                DrawRectangle(px - 20, 460, larg + 40, 45, Fade(BLACK, 0.75f));
                DrawText(msgFeedback, px, 470, 24, corFeedback);
            }

        // ------------------------------------------------------------
        // TELA 2: COMPRAR PACOTES
        // Exibe campo de texto com cursor piscante e total calculado
        // ------------------------------------------------------------
        } else if (telaAtual == 2) {
            DrawTexturePro(fundoComprar,
                (Rectangle){0,0,(float)fundoComprar.width,(float)fundoComprar.height},
                (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},
                (Vector2){0,0}, 0.0f, WHITE);

            // Cursor piscante: alterna entre "texto|" e "texto" a cada meio segundo
            char texCursor[18];
            sprintf(texCursor, (int)(GetTime()*2)%2==0 ? "%s|" : "%s", inputTexto);
            DrawText(texCursor, 342, 600, 26, DARKBROWN);

            // Mostra o valor total enquanto o jogador digita
            if (inputLen > 0) {
                char total[64];
                sprintf(total, "Total: R$ %.2f", atoi(inputTexto) * padaria->precoPacote);
                DrawText(total, 345, 558, 22, DARKBROWN);
            }
            DrawText("ESC para voltar", 20, 680, 20, RED);

        // ------------------------------------------------------------
        // TELA 3: VENDER FIGURINHAS REPETIDAS
        // Exibe campo de texto e ganho estimado
        // ------------------------------------------------------------
        } else if (telaAtual == 3) {
            DrawTexturePro(fundoVender,
                (Rectangle){0,0,(float)fundoVender.width,(float)fundoVender.height},
                (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},
                (Vector2){0,0}, 0.0f, WHITE);

            char texCursor[18];
            sprintf(texCursor, (int)(GetTime()*2)%2==0 ? "%s|" : "%s", inputTexto);
            DrawText(texCursor, 342, 600, 26, DARKBROWN);

            // Mostra o valor que o jogador vai ganhar com a venda
            if (inputLen > 0) {
                char ganho[64];
                sprintf(ganho, "Ganho: R$ %.2f", atoi(inputTexto) * padaria->precoComprarFigurinhas);
                DrawText(ganho, 345, 558, 22, DARKBROWN);
            }
            DrawText("ESC para voltar", 20, 680, 20, RED);

        // ------------------------------------------------------------
        // TELA 4: BALCAO DE TROCAS
        // Exibe as 5 figurinhas do balcao, timer de atualizacao e quantidade de repetidas
        // ------------------------------------------------------------
        } else if (telaAtual == 4) {
            DrawTexturePro(fundoTrocar,
                (Rectangle){0,0,(float)fundoTrocar.width,(float)fundoTrocar.height},
                (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},
                (Vector2){0,0}, 0.0f, WHITE);

            // Calcula e exibe o tempo restante para o balcao ser atualizado
            int faltaSeg = TEMPO_TROCA - (int)difftime(time(NULL), padaria->ultimaAtualizacao);
            if (faltaSeg < 0) faltaSeg = 0;
            char timerTxt[64];
            sprintf(timerTxt, "Atualiza em: %02d:%02d", faltaSeg/60, faltaSeg%60);
            int lw  = MeasureText(timerTxt, 24);
            int lpx = (GetScreenWidth() - lw) / 2;
            DrawText(timerTxt, lpx+2, 42, 24, BLACK); // sombra
            DrawText(timerTxt, lpx,   40, 24, (faltaSeg < 60) ? RED : WHITE); // fica vermelho quando falta menos de 1 minuto

            // Conta e exibe quantas figurinhas repetidas o jogador tem para usar em trocas
            int totalRepetidas = 0;
            for (int j = 0; j < meu_album->quantidade_atual; j++)
                totalRepetidas += meu_album->figurinhas[j].quantidade_repetidas;
            char repTxt[64];
            sprintf(repTxt, "Figurinhas repetidas: %d", totalRepetidas);
            int lrep = MeasureText(repTxt, 20);
            DrawText(repTxt, (GetScreenWidth()-lrep)/2 + 1, 72, 20, BLACK); // sombra
            DrawText(repTxt, (GetScreenWidth()-lrep)/2,     70, 20, (totalRepetidas > 0) ? YELLOW : RED);

            // Desenha cada um dos 5 slots com a imagem da figurinha ou "TROCADA"
            int centrosTrocada[5] = {165, 400, 635, 865, 1095};
            int offsets[5] = {-40, -40, -40, 0, 5};
            for (int i = 0; i < 5; i++) {
                int centroX = (int)btnTrocar[i].x + (int)btnTrocar[i].width/2 + offsets[i];
                Rectangle destImg = { centroX - imgLarg/2, imgTopoY, imgLarg, imgAltu };

                if (padaria->slotUsado[i]) {
                    // Slot ja foi trocado: mostra "TROCADA" em verde
                    int larg = MeasureText("TROCADA", 20);
                    DrawText("TROCADA", centrosTrocada[i] - larg/2, 135, 20, GREEN);
                } else {
                    // Slot disponivel: desenha a imagem da figurinha (ou o codigo se a imagem falhar)
                    if (texFigurinhas[i].id != 0) {
                        DrawTexturePro(texFigurinhas[i],
                            (Rectangle){0,0,(float)texFigurinhas[i].width,(float)texFigurinhas[i].height},
                            destImg, (Vector2){0,0}, 0.0f, WHITE);
                    } else {
                        // Fallback: exibe o codigo da figurinha em texto
                        int larg = MeasureText(padaria->FigurinhasNoBalcao[i], 18);
                        DrawText(padaria->FigurinhasNoBalcao[i], centroX-larg/2+1, imgTopoY+imgAltu/2+1, 18, BLACK);
                        DrawText(padaria->FigurinhasNoBalcao[i], centroX-larg/2,   imgTopoY+imgAltu/2,   18, WHITE);
                    }
                }
            }

            // Mensagem de feedback (troca feita ou erro)
            if (timerFeedback > 0.0f) {
                int larg = MeasureText(msgFeedback, 24);
                int px   = (GetScreenWidth() - larg) / 2;
                DrawRectangle(px-20, 620, larg+40, 38, Fade(BLACK, 0.75f));
                DrawText(msgFeedback, px, 627, 24, corFeedback);
            }
            DrawText("ESC para voltar", 20, 680, 20, RED);
        }


        // ============================================================
        // HUD: INFORMACOES DO JOGADOR (aparecem em todas as telas)
        // ============================================================
        char hud[64];
        sprintf(hud, "Dinheiro: R$ %.2f", *saldo_jogador);
        DrawText(hud, 22, 22, 26, BLACK); DrawText(hud, 20, 20, 26, YELLOW); // sombra + texto

        sprintf(hud, "Album: %s", *tem_album ? "SIM" : "NAO");
        DrawText(hud, 22, 52, 26, BLACK); DrawText(hud, 20, 50, 26, WHITE);

        sprintf(hud, "Pacotes: %d", *pacotes_bolso);
        DrawText(hud, 22, 142, 26, BLACK); DrawText(hud, 20, 140, 26, WHITE);

        // Conta total de repetidas para exibir no HUD
        int totalRepHud = 0;
        for (int j = 0; j < meu_album->quantidade_atual; j++)
            totalRepHud += meu_album->figurinhas[j].quantidade_repetidas;
        sprintf(hud, "Repetidas: %d", totalRepHud);
        DrawText(hud, 22, 172, 26, BLACK); DrawText(hud, 20, 170, 26, WHITE);

        EndDrawing();
    }

    // Janela fechada: libera todas as texturas antes de sair
    UnloadTexture(fundo); UnloadTexture(fundoBalcao);
    UnloadTexture(fundoComprar); UnloadTexture(fundoVender);
    UnloadTexture(fundoTrocar);
    for (int i = 0; i < 5; i++)
        if (texFigurinhas[i].id != 0) UnloadTexture(texFigurinhas[i]);
}