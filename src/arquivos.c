//###########################################################//
//Bibliotecas
//###########################################################//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figurinha.h"

//###########################################################//
//Funcao que abre o arquivo no hd, no caso a leitura apenas por enqnt
//###########################################################//
int Carrega_Csv(Album *album, const char *nome_arquivo){ //const char, pra apenas leitura

    FILE *arquivo;

    char leitura_linha[301];

    arquivo = fopen(nome_arquivo,"r");

    if (arquivo == NULL){
        printf("\nErro ao abrir o arquivo!\n");
        return 0;
    }//caso de erro ao abrir o arquivo

    fgets(leitura_linha, sizeof(leitura_linha),arquivo);
    //aqui ele pega o cabeçalho, pq ele n me ajuda muito

    while (fgets(leitura_linha, sizeof(leitura_linha), arquivo) != NULL) { 
        
        Dados_Figurinha pedaco_temporario;
        char *pedaco;

       
        pedaco = strtok(leitura_linha, ";,"); 
        if (pedaco == NULL) {
            continue;
        }
        strcpy(pedaco_temporario.codigo, pedaco);

        pedaco = strtok(NULL, ";,");
        if (pedaco != NULL) strcpy(pedaco_temporario.nome_Jogador, pedaco);

        pedaco = strtok(NULL, ";,");
        if (pedaco != NULL) strcpy(pedaco_temporario.secao, pedaco);

        pedaco = strtok(NULL, ";,");
        if (pedaco != NULL) strcpy(pedaco_temporario.grupo, pedaco);

        pedaco = strtok(NULL, ";,\r\n");
        if (pedaco != NULL) strcpy(pedaco_temporario.raridade, pedaco);

        pedaco_temporario.colada = 0;
        pedaco_temporario.quantidade_repetidas = 0;  
        
        if (album->quantidade_atual == album->quantidade_maxima){
            album->quantidade_maxima *= 2; 

            album->figurinhas = (Dados_Figurinha*) realloc(album->figurinhas, album->quantidade_maxima * sizeof(Dados_Figurinha)); 

            if (album->figurinhas == NULL){
                printf("\nErro ao realocar a memoria referente ao album->figurinhas\n");
                return 0;
            }//ultimo if           
        }//realloc
        album->figurinhas[album->quantidade_atual] = pedaco_temporario;
        album->quantidade_atual++;
    }//while

    fclose(arquivo);
    return 1;
}//final da funcao

//###########################################################//
//Funcao que vai da memoria ram pro arquivo em binario
//###########################################################//
int Salva_Bin(Album *album, const char *nome_arquivo){

   FILE *arquivo;

   arquivo = fopen(nome_arquivo,"wb");

   if (arquivo == NULL){
    return 0;
   }//caso o arquivo nao abra

   fwrite(&album->quantidade_atual, sizeof(int),1,arquivo);

   fwrite(album->figurinhas,sizeof(Dados_Figurinha), album->quantidade_atual,arquivo);

   fclose(arquivo);

    return 1;
}//funcao que converte pra bin

//###########################################################//
//Funcao que sai do arquivo em binario e vai pra memoria ram, caminho contrario da fnc Salva_Bin
//###########################################################//
int Carrega_Bin(Album * album, const char *nome_arquivo){
    FILE *arquivo;

    arquivo = fopen(nome_arquivo,"rb");

    if (arquivo == NULL){
        return 0;
    }//caso nao consiga carregar do hd pra ram

    fread(&album->quantidade_atual,sizeof(int),1,arquivo);

    album->figurinhas = (Dados_Figurinha*) malloc(album->quantidade_atual * sizeof(Dados_Figurinha));

    fread(album->figurinhas,sizeof(Dados_Figurinha),album->quantidade_atual,arquivo);
    
    if (album->quantidade_atual == 0) {
        album->quantidade_maxima = 10; 
    } else {
        album->quantidade_maxima = album->quantidade_atual;
    }

    fclose(arquivo);

    return 1;
}//final da funcao que abre o arquivo


