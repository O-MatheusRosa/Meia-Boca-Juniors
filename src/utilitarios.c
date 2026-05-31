#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

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

void Som_Sucesso() {
#ifdef _WIN32
    Beep(1500, 120);
#else
    printf("\a");
#endif
}

void Som_Erro() {
#ifdef _WIN32
    Beep(400, 300);
#else
    printf("\a");
#endif
}

void Som_Menu() {
#ifdef _WIN32
    Beep(700, 100);
    Beep(900, 100);
#else
    printf("\a");
#endif
}

void Som_Alerta() {
#ifdef _WIN32
    Beep(1000, 200);
#else
    printf("\a");
#endif
}

void Som_Saida() {
#ifdef _WIN32
    Beep(600, 150);
    Beep(400, 150);
#else
    printf("\a");
#endif
}

void Limpar_Tela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Delay(int ms){
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif
}

void Animacao_Loading() {
    printf("\n"); 
    
    for (int i = 0; i <= 10; i++) {
        
        printf("\r" AMARELO ">> Rasgando o pacote [" RESET);
        
        for (int j = 0; j < 10; j++) {
            if (j < i) {
                printf(VERDE "=" RESET); // Parte já carregada
            } else if (j == i) {
                printf(VERDE ">" RESET); // A ponta da flecha
            } else {
                printf(" "); // Parte vazia
            }
        }
        
        printf(AMARELO "] %d%%" RESET, i * 10);
        
        fflush(stdout); 
        
        Delay(150); 
    }
    printf("\n\n"); // Desce a linha quando terminar de carregar
}