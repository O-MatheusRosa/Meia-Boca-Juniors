#ifndef TEXTURA_H
#define TEXTURA_H
#include "raylib.h"
#include "figurinha.h"
#include <time.h> 

typedef struct {
    int    qntAlbum;
    float  precoAlbum;
    float  precoPacote;
    float  precoComprarFigurinhas; // valor que a padaria paga por cada figurinha vendida
    char FigurinhasNoBalcao[5][15]; // codigo tipo "BRA01", ajuste o 15 se o codigo for maio  // numeros das 5 figurinhas disponiveis para troca
    int    slotUsado[5];           // 1 = slot ja trocado (indisponivel)
    time_t ultimaAtualizacao;      // momento da ultima atualizacao do balcao
} BancaPadaria;

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
void Tela_Jogo(Texture2D fundo_dia, Texture2D fundo_noite, Music musica, Album *meu_album, Album *catalogo_geral);
int Tela_Diltu(Music musica, float *saldo_jogador, bool deDia);
void Tela_Aposta(Music musica,float *saldo_jogador);
void Tela_Futebol(Music musica,float *saldo_jogador);
void Tela_Bixo(Music musica,float *saldo_jogador);


void Animacao_AbrirPacotinho(Album *meu_album, Album *catalogo_geral);


void ExecutarModuloMonitoria(float *saldo_jogador, bool deDia);
void fatiar_csv_avancado(char *linha, char colunas[9][400]);
int carregar_questoes_csv(Questao *banco);

int Tela_Home(Texture2D fundo, Music musica_menu);

void Entra_Padaria(BancaPadaria *padaria, Album *catalogo_geral, float *saldo_jogador, int *tem_album, int *pacotes_bolso, int *figurinhas_bolso);





#endif