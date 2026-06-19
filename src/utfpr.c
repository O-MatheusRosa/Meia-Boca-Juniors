#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "../include/textura.h" // Contém a definição da struct 'Questao'

// Define a quantidade máxima de perguntas que o banco de dados do jogo pode suportar
#define TOTAL_BANCO 20

// Máquina de Estados: Define em qual tela o jogador está atualmente
typedef enum { 
    TELA_SALA_AULA,   // Tela inicial da sala, esperando clicar no aluno
    TELA_VS_CODE,     // Tela de perguntas (simulando o editor de código)
    TELA_RESULTADO,   // Tela que mostra se ganhou dinheiro ou trabalhou de graça
    TELA_CANSADO      // Tela de descanso (onde entra a foto do Prof. Muriel e o timer)
} EstadoMonitoria;

// ============================================================================
// 1. FUNÇÕES AUXILIARES
// ============================================================================

// Função que divide uma linha do CSV em colunas, ignorando vírgulas que estão dentro de aspas ""
void fatiar_csv_avancado(char *linha, char colunas[9][400]) {
    int i = 0, col = 0, char_idx = 0;
    int dentro_de_aspas = 0; // Controle para saber se a vírgula faz parte do texto ou separa colunas

    // Limpa a matriz de colunas antes de começar a preencher
    for (int c = 0; c < 9; c++) colunas[c][0] = '\0';

    // Percorre a linha caractere por caractere
    while (linha[i] != '\0' && col < 9) {
        if (linha[i] == '"') {
            dentro_de_aspas = !dentro_de_aspas; // Se achou aspas, inverte o estado (entrou ou saiu delas)
        } else if (linha[i] == ',' && !dentro_de_aspas) {
            // Se achou uma vírgula FORA das aspas, significa que mudou de coluna
            colunas[col][char_idx] = '\0'; // Finaliza a string da coluna atual
            col++;                         // Avança para a próxima coluna
            char_idx = 0;                  // Reseta o índice de caracteres da nova coluna
        } else if (linha[i] != '\r' && linha[i] != '\n') {
            // Se for um caractere válido, copia para a coluna atual
            colunas[col][char_idx] = linha[i];
            char_idx++;
        }
        i++;
    }
    colunas[col][char_idx] = '\0'; // Finaliza a última string
}

// Função mágica para quebrar o texto em várias linhas automaticamente se ele for maior que a largura permitida
void DrawTextWrapped(const char *text, int x, int y, int maxWidth, int fontSize, int lineSpacing, Color color) {
    char buffer[1024];
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char linhaAtual[1024] = "";
    char palavra[256];
    int posBuffer = 0;
    int len = (int)strlen(buffer);
    int curY = y;

    while (posBuffer < len) {
        int wIdx = 0;
        // Pega a próxima palavra isolada do texto
        while (posBuffer < len && buffer[posBuffer] != ' ') {
            if (wIdx < (int)sizeof(palavra) - 1) palavra[wIdx++] = buffer[posBuffer];
            posBuffer++;
        }
        palavra[wIdx] = '\0';
        if (buffer[posBuffer] == ' ') posBuffer++;

        // Tenta encaixar a palavra na linha atual
        char tentativa[1024];
        if (linhaAtual[0] == '\0') {
            snprintf(tentativa, sizeof(tentativa), "%s", palavra);
        } else {
            snprintf(tentativa, sizeof(tentativa), "%s %s", linhaAtual, palavra);
        }

        int largura = MeasureText(tentativa, fontSize);
        // Se estourar a largura máxima, desenha o que já tem e pula para a linha de baixo
        if (largura > maxWidth && linhaAtual[0] != '\0') {
            DrawText(linhaAtual, x, curY, fontSize, color);
            curY += fontSize + lineSpacing; // Avança o Y (vertical)
            snprintf(linhaAtual, sizeof(linhaAtual), "%s", palavra);
        } else {
            snprintf(linhaAtual, sizeof(linhaAtual), "%s", tentativa);
        }
    }

    // Desenha o último pedaço de texto que sobrou no buffer
    if (linhaAtual[0] != '\0') {
        DrawText(linhaAtual, x, curY, fontSize, color);
    }
}

// Sorteia se o acerto de 2 questões vai render dinheiro ou se será "Trabalho Voluntário"
float finalizar_turno(void) {
    int rng = (rand() % 100) + 1; // Gera número de 1 a 100
    int chance_voluntario = 40;   // 40% de chance de trabalhar de graça

    if (rng <= chance_voluntario) {
        return 0.00f; // Azar: voluntário
    } else {
        return 7.00f; // Sorte: ganhou R$ 7,00
    }
}

// Carrega as perguntas do arquivo CSV para a memória ram (Array de structs Questao)
int carregar_questoes_csv(Questao *banco) {
    FILE *arquivo = fopen("perguntas_jogo.csv", "r");
    if (arquivo == NULL) return 0; // Se o arquivo não existir, fecha o módulo com segurança

    char linha[1500];
    fgets(linha, sizeof(linha), arquivo); // Pula a primeira linha (cabeçalho com títulos)

    char colunas[9][400];
    int i = 0;
    // Lê linha por linha até o final do arquivo ou até encher o limite máximo do banco
    while (fgets(linha, sizeof(linha), arquivo) && i < TOTAL_BANCO) {
        fatiar_csv_avancado(linha, colunas);
        banco[i].id = atoi(colunas[0]);
        strcpy(banco[i].nivel, colunas[1]);
        strcpy(banco[i].theme, colunas[2]);
        strcpy(banco[i].enunciado, colunas[3]);
        strcpy(banco[i].altA, colunas[4]);
        strcpy(banco[i].altB, colunas[5]);
        strcpy(banco[i].altC, colunas[6]);
        strcpy(banco[i].altD, colunas[7]);
        strcpy(banco[i].correta, colunas[8]);
        i++;
    }
    fclose(arquivo);
    return i; // Retorna a quantidade de questões carregadas com sucesso
}

// ============================================================================
// 2. FUNÇÃO PRINCIPAL DO MÓDULO
// ============================================================================
void ExecutarModuloMonitoria(float *saldo_jogador) {
    // Carrega os arquivos de imagem (texturas) para a memória de vídeo
    Texture2D monitoria = LoadTexture("assets/monitoria.png");
    Texture2D telaPerguntas = LoadTexture("assets/tela_perguntas.png");
    Texture2D telaDescanso = LoadTexture("assets/tela_descanso_monitoria.png"); // Imagem customizada com o balão

    Questao banco_questoes[TOTAL_BANCO];
    int total_carregado = carregar_questoes_csv(banco_questoes);

    // Se falhar ao carregar o CSV, descarrega as texturas e fecha a função para o jogo não dar crash
    if (total_carregado == 0) {
        UnloadTexture(monitoria);
        UnloadTexture(telaPerguntas);
        UnloadTexture(telaDescanso);
        return;
    }

    EstadoMonitoria estadoAtual = TELA_SALA_AULA; // Começa na sala de aula

    // Controle de tempo para a mecânica de fadiga/cansaço
    static double tempoFimUltimoTurno = -1000.0; // Valor inicial baixo garante liberação imediata no primeiro clique
    const double TEMPO_ESPERA = 60.0;            // Tempo de descanso obrigatório em segundos

    // Definição das Hitboxes (áreas clicáveis do mouse) calibradas para a tela de 1280x720
    Rectangle areaMeninoVerde = { 796, 309, 178, 281 }; // Coordenadas do aluno na sala

    // Coordenadas dos botões das alternativas A, B, C e D dentro do "Terminal" azul
    Rectangle botaoA = { 213, 405, 515, 90 };
    Rectangle botaoB = { 744, 405, 515, 90 };
    Rectangle botaoC = { 213, 505, 515, 90 };
    Rectangle botaoD = { 744, 505, 515, 90 };

    int idx_q1 = 0, idx_q2 = 0;
    int perguntaAtual = 1;
    int questaoSorteadaIdx = 0;
    int respostasCorretas = 0;
    char textoResultado[160] = "";

    // Loop principal da tela de monitoria (continua rodando até fechar a janela ou apertar ESC)
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition(); // Captura a posição X e Y do mouse a cada frame

        // Permite sair da monitoria ou voltar para a sala de aula usando a tecla ESC
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (estadoAtual == TELA_SALA_AULA) break; // Sai da monitoria e volta para o mapa principal do jogo
            else estadoAtual = TELA_SALA_AULA;
        }

        // --- MÁQUINA DE ESTADOS: ATUALIZAÇÃO DA LÓGICA ---
        switch (estadoAtual) {
            case TELA_SALA_AULA:
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Verifica se o jogador clicou em cima do boneco do aluno
                    if (CheckCollisionPointRec(mousePos, areaMeninoVerde)) {
                        double segundosPassados = GetTime() - tempoFimUltimoTurno;

                        // Se o cooldown de 60 segundos não acabou, barra o jogador e manda ele descansar
                        if (segundosPassados < TEMPO_ESPERA) {
                            estadoAtual = TELA_CANSADO;
                        } else {
                            // Sorteia duas questões aleatórias e garante que elas não sejam iguais
                            idx_q1 = rand() % total_carregado;
                            idx_q2 = rand() % total_carregado;
                            while (idx_q2 == idx_q1) idx_q2 = rand() % total_carregado;

                            // Inicializa as variáveis do turno de perguntas
                            perguntaAtual = 1;
                            questaoSorteadaIdx = idx_q1;
                            respostasCorretas = 0;
                            estadoAtual = TELA_VS_CODE; // Vai para a tela de resolver problemas
                        }
                    }
                }
                break;

            case TELA_CANSADO:
                // Se o tempo acabar enquanto o jogador olha a tela do Prof. Muriel, libera o acesso automaticamente
                if (GetTime() - tempoFimUltimoTurno >= TEMPO_ESPERA) {
                    estadoAtual = TELA_SALA_AULA;
                }
                break;

            case TELA_VS_CODE: {
                char respostaEscolhida[10] = "";

                // Captura a resposta via teclado (Teclas A, B, C ou D)
                if (IsKeyPressed(KEY_A)) strcpy(respostaEscolhida, "A");
                else if (IsKeyPressed(KEY_B)) strcpy(respostaEscolhida, "B");
                else if (IsKeyPressed(KEY_C)) strcpy(respostaEscolhida, "C");
                else if (IsKeyPressed(KEY_D)) strcpy(respostaEscolhida, "D");

                // Ou captura a resposta via cliques do mouse nos botões do terminal
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mousePos, botaoA)) strcpy(respostaEscolhida, "A");
                    else if (CheckCollisionPointRec(mousePos, botaoB)) strcpy(respostaEscolhida, "B");
                    else if (CheckCollisionPointRec(mousePos, botaoC)) strcpy(respostaEscolhida, "C");
                    else if (CheckCollisionPointRec(mousePos, botaoD)) strcpy(respostaEscolhida, "D");
                }

                // Se o jogador escolheu uma alternativa (por clique ou teclado)
                if (respostaEscolhida[0] != '\0') {
                    // Compara a resposta ignorando maiúsculas/minúsculas (stricmp)
                    if (stricmp(respostaEscolhida, banco_questoes[questaoSorteadaIdx].correta) == 0) {
                        respostasCorretas++;
                    }

                    // Se foi apenas a primeira pergunta, carrega a segunda
                    if (perguntaAtual == 1) {
                        perguntaAtual = 2;
                        questaoSorteadaIdx = idx_q2;
                    } else {
                        // Se respondeu a segunda, calcula os lucros do turno finalizado
                        float ganho = 0.00f;

                        if (respostasCorretas == 2) {
                            ganho = finalizar_turno(); // Sorteia o fator voluntário/pago
                            if (ganho == 0.00f) {
                                sprintf(textoResultado, "Turno Finalizado! 2/2 corretas.\nDeu sorte ruim: contabilizado como VOLUNTARIO (R$ 0,00).");
                            } else {
                                sprintf(textoResultado, "Turno Finalizado! 2/2 corretas.\nExplicacoes didaticas! Voce ganhou R$ %.2f.", ganho);
                            }
                        } else if (respostasCorretas == 1) {
                            ganho = 3.50f; // Acertou metade, ganha fixo metade do valor sem bônus
                            sprintf(textoResultado, "Turno Finalizado! 1/2 corretas.\nSem bonus, mas recebeu R$ %.2f.", ganho);
                        } else {
                            ganho = 0.00f; // Errou tudo, sai sem um tostão
                            sprintf(textoResultado, "Turno Finalizado! 0/2 corretas.\nNenhuma explicacao boa... sem pagamento.");
                        }

                        *saldo_jogador += ganho;      // Atualiza a carteira global do jogador
                        tempoFimUltimoTurno = GetTime(); // Salva a marca exata de tempo em que terminou o trabalho

                        estadoAtual = TELA_RESULTADO; // Avança para exibir o feedback
                    }
                }
                break;
            }

            case TELA_RESULTADO:
                // Sai da tela de resultados ao clicar ou apertar ENTER
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)) {
                    estadoAtual = TELA_SALA_AULA;
                }
                break;
        }

        // --- RENDERIZAÇÃO GRÁFICA (DESENHAR NA TELA) ---
        BeginDrawing();
            ClearBackground(BLACK); // Limpa o rastro do frame anterior

            if (estadoAtual == TELA_SALA_AULA) {
                DrawTexture(monitoria, 0, 0, WHITE);
                DrawText("Clique no aluno para responder sua pergunta", 30, 30, 20, GREEN);
                DrawText(TextFormat("Saldo: R$ %.2f", *saldo_jogador), 30, 60, 20, GOLD);
            }
            else if (estadoAtual == TELA_CANSADO) {
                // Desenha a imagem do laboratório com o Prof. Muriel e o balão branco de fala
                DrawTexture(telaDescanso, 0, 0, WHITE);

                // Calcula os segundos regressivos baseados no tempo estipulado
                int segundosRestantes = (int)(TEMPO_ESPERA - (GetTime() - tempoFimUltimoTurno)) + 1;
                if (segundosRestantes < 0) segundosRestantes = 0;

                // Formata o texto do timer
                const char* textoTimer = TextFormat("Tempo restante: %d s", segundosRestantes);
                int fontSize = 17;

                // Coordenadas calibradas para centralizar o texto certinho no espaço vazio do balão branco
                int balaoCentroX = 535; 
                int balaoY = 75; 

                // Mede a largura dinâmica do texto em pixels para alinhar o centro perfeitamente
                int larguraTexto = MeasureText(textoTimer, fontSize);
                
                // Desenha o timer em PRETO (BLACK) para ficar legível por cima do fundo branco do balão
                DrawText(textoTimer, balaoCentroX - (larguraTexto / 2), balaoY, fontSize, BLACK);
            }
            else if (estadoAtual == TELA_VS_CODE) {
                DrawTexture(telaPerguntas, 0, 0, WHITE);

                Questao q = banco_questoes[questaoSorteadaIdx];

                // ===== DESENHA A ÁREA DO EDITOR DE CÓDIGO (Caixa de texto superior) =====
                int edX = 197, edY = 67, edW = 1076, edH = 276;
                DrawRectangle(edX, edY, edW, edH, (Color){30, 30, 30, 230});      
                DrawRectangle(edX, edY, edW, 28, (Color){45, 45, 45, 255});       
                DrawText("duvida_aluno.c", edX + 14, edY + 7, 14, LIGHTGRAY);

                DrawText("// EDITOR DE CODIGO - DUVIDA DO ALUNO:", edX + 18, edY + 45, 16, GRAY);
                DrawText(TextFormat("/* Tema: %s */", q.theme), edX + 18, edY + 75, 16, GREEN);
                DrawTextWrapped(TextFormat("char* duvida = \"%s\";", q.enunciado),
                                 edX + 18, edY + 115, edW - 36, 18, 8, RAYWHITE);

                // ===== DESENHA A ÁREA DO TERMINAL (Opções clicáveis abaixo) =====
                int termX = 197, termY = 377, termW = 1076, termH = 290;
                DrawRectangle(termX, termY, termW, termH, (Color){10, 10, 10, 230}); 
                DrawRectangle(termX, termY, termW, 26, (Color){45, 45, 45, 255});    
                DrawText("TERMINAL - selecione a resposta correta", termX + 14, termY + 6, 14, LIGHTGRAY);

                // Efeito Hover: Se o mouse colidir com o botão da alternativa, pinta de DARKGRAY, senão fica BLACK
                DrawRectangleRec(botaoA, CheckCollisionPointRec(mousePos, botaoA) ? DARKGRAY : BLACK);
                DrawTextWrapped(TextFormat("[A] %s", q.altA), (int)botaoA.x + 10, (int)botaoA.y + 5, (int)botaoA.width - 20, 15, 4, GREEN);

                DrawRectangleRec(botaoB, CheckCollisionPointRec(mousePos, botaoB) ? DARKGRAY : BLACK);
                DrawTextWrapped(TextFormat("[B] %s", q.altB), (int)botaoB.x + 10, (int)botaoB.y + 5, (int)botaoB.width - 20, 15, 4, GREEN);

                DrawRectangleRec(botaoC, CheckCollisionPointRec(mousePos, botaoC) ? DARKGRAY : BLACK);
                DrawTextWrapped(TextFormat("[C] %s", q.altC), (int)botaoC.x + 10, (int)botaoC.y + 5, (int)botaoC.width - 20, 15, 4, GREEN);

                DrawRectangleRec(botaoD, CheckCollisionPointRec(mousePos, botaoD) ? DARKGRAY : BLACK);
                DrawTextWrapped(TextFormat("[D] %s", q.altD), (int)botaoD.x + 10, (int)botaoD.y + 5, (int)botaoD.width - 20, 15, 4, GREEN);

                DrawText("Clique ou aperte [A, B, C, D] no teclado", termX + 18, termY + termH - 25, 14, LIGHTGRAY);
            }
            else if (estadoAtual == TELA_RESULTADO) {
                DrawTexture(telaPerguntas, 0, 0, WHITE);

                // Pop-up centralizado contendo o veredito financeiro do turno
                DrawRectangle(300, 230, 680, 220, BLACK);
                DrawRectangleLines(300, 230, 680, 220, GREEN);

                char copiaResultado[160];
                snprintf(copiaResultado, sizeof(copiaResultado), "%s", textoResultado);

                // Separa as frases pela quebra de linha '\n' obtida no cálculo anterior para renderizar organizadamente
                char *linha1 = strtok(copiaResultado, "\n");
                char *linha2 = strtok(NULL, "\n");

                if (linha1 != NULL) DrawTextWrapped(linha1, 330, 280, 620, 18, 6, WHITE);
                if (linha2 != NULL) DrawTextWrapped(linha2, 330, 310, 620, 18, 6, WHITE);

                DrawText(TextFormat("Saldo atual: R$ %.2f", *saldo_jogador), 330, 360, 18, GOLD);
                DrawText("Pressione ENTER ou Clique para voltar", 330, 410, 14, GRAY);
            }

        EndDrawing(); // Envia os comandos de desenho para a GPU processar a tela
    }

    // ============================================================================
    // 3. LIMPEZA DE MEMÓRIA (Garbage Collection Manual do C/Raylib)
    // ============================================================================
    // Quando o loop quebra e saímos da monitoria, deletamos os dados pesados das texturas da VRAM.
    // Isso previne estouro de memória (Memory Leak) se o jogador abrir a monitoria repetidas vezes.
    UnloadTexture(monitoria);
    UnloadTexture(telaPerguntas);
    UnloadTexture(telaDescanso); 
}