#ifndef TEXTURA_H
#define TEXTURA_H

int Tela_Home(Texture2D fundo, Music musica);
void Tela_Jogo(Texture2D fundo_dia, Texture2D fundo_noite, Music musica);
int Tela_Diltu(Music musica, float *saldo_jogador, bool deDia);
void Tela_Aposta(Music musica,float *saldo_jogador);
void Tela_Futebol(Music musica,float *saldo_jogador);
void Tela_Bixo(Music musica,float *saldo_jogador);

int Tela_Home(Texture2D fundo, Music musica_menu);

#endif