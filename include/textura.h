#ifndef TEXTURA_H
#define TEXTURA_H

#include "raylib.h"

// Definição da estrutura de questões do seu CSV
typedef struct {
    int id;
    char nivel[30];
    char theme[50];
    char enunciado[400];
    char altA[250];
    char altB[250];
    char altC[250];
    char altD[250];
    char correta[10];
} Questao;


int Tela_Home(Texture2D fundo, Music musica);
void Tela_Jogo(Texture2D fundo_dia, Texture2D fundo_noite, Music musica);
int Tela_Diltu(Music musica, float *saldo_jogador, bool deDia);
void Tela_Aposta(Music musica,float *saldo_jogador);
void Tela_Futebol(Music musica,float *saldo_jogador);
void Tela_Bixo(Music musica,float *saldo_jogador);


void ExecutarModuloMonitoria(float *saldo_jogador);
void fatiar_csv_avancado(char *linha, char colunas[9][400]);
int carregar_questoes_csv(Questao *banco);

int Tela_Home(Texture2D fundo, Music musica_menu);


#endif