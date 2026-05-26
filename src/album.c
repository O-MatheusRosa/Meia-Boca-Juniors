//###########################################################//
//Bibliotecas
//###########################################################//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figurinha.h"

#ifndef _WIN32
    #include <strings.h>
    #define stricmp strcasecmp
#endif


//###########################################################//
//Funcao que vai meio q limpar o nome, pq ele tem os espaços, o \n, tem tudo, entao limpamos ele
//###########################################################//
void Sanitiza_Nome(Album *album){
    for (int i = 0; i < album->quantidade_atual; i++){
        char *nome = album->figurinhas[i].nome_Jogador;

        nome[strcspn(nome,"\r\n")] = '\0';

        int len = strlen(nome);

        while (len > 0 && nome[len - 1] == ' '){
            nome[len - 1] = '\0';
            len--;
        }//while q repete enquanto há caractere espaço sobrando  

        while(nome[0] == ' '){
           int j = 0;

           while (nome[j + 1] != 0){
                nome[j] = nome[j + 1];
                j++;
           }//while

           nome[j] = '\0';
        }//final do if     
    }//final do for 
}//funcao


//###########################################################//
//Funcao que vai printar o nome dos jogadores
//###########################################################//
void Mostra_Album(Album *album){

    if (album->quantidade_atual == 0){
        printf(">> O album ta vazio! Vai abrir pacotinho!\n");
        return;
    }//caso n tenha figurinha

    printf("\n=== LISTA DE FIGURINHAS ===\n");
    
    for (int i = 0; i < album->quantidade_atual; i++){
        printf("\nNome: %-20s || Codigo: %7s || Raridade: %10s\n",album->figurinhas[i].nome_Jogador,album->figurinhas[i].codigo,album->figurinhas[i].raridade);
    }//for

    printf("===========================\n");
}//primeira funcao

//###########################################################//
//Função que vai ordenar a lista em binario
//###########################################################//
int Ordena_lista_Bin(const void *a, const void *b){

   Dados_Figurinha *figurinhaA = (Dados_Figurinha*) a;
   Dados_Figurinha *figurinhaB = (Dados_Figurinha*) b;

   return stricmp(figurinhaA->nome_Jogador, figurinhaB->nome_Jogador);
}//segunda função



//###########################################################//
//Funcao que vai buscar o nome do jogador
//###########################################################//
int Procura_Jogador(Album *album,char *nome_buscado){

    nome_buscado[strcspn(nome_buscado,"\r\n")] = '\0';

    for (int i = 0; i < album->quantidade_atual; i++){
        if (stricmp(nome_buscado,album->figurinhas[i].nome_Jogador) == 0){// o i ali no str i cpm é pra ignorar o case sensivite
            return i;
        }//if q compara o nome digitado e ve se tem o nome do jogador de vdd
    }//final do for q repete ate ter fig

    return -1;
}//enesima fnc