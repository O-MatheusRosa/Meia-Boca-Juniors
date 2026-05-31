#ifndef UTILITARIOS_H
#define UTILITARIOS_H

// ===== Cores ANSI =====
#define RESET        "\033[0m"
#define VERMELHO     "\033[1;31m"
#define VERDE        "\033[1;32m"
#define AMARELO      "\033[1;33m"
#define AZUL         "\033[1;34m"
#define CIANO        "\033[1;36m"
#define BRANCO       "\033[1;37m"
#define FUNDO_AZUL   "\033[44m"
#define FUNDO_VERDE  "\033[42m"
#define FUNDO_VERMELHO "\033[41m"
#define NEGRITO      "\033[1m"

// ===== Funcoes =====
void Som_Sucesso();
void Som_Erro();
void Som_Menu();
void Som_Alerta();
void Som_Saida();
void Limpar_Tela();
void Delay(int ms);
void Animacao_Loading();

#endif