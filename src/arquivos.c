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

    while (fgets(leitura_linha, sizeof(leitura_linha),arquivo) != NULL){ //ele pega linha enqt tiver
        
        Dados_Figurinha pedaco_temporario;
        char *pedaco;

        pedaco = strtok(leitura_linha,",");
        strcpy(pedaco_temporario.codigo,pedaco);
        //pega o pedaço, que no caso é o codigo da fig, tipo BRA001

        pedaco = strtok(NULL,",");
        strcpy(pedaco_temporario.nome_Jogador,pedaco);
        //pedaço referente ao nome do jogador

        pedaco = strtok(NULL,",");
        strcpy(pedaco_temporario.secao,pedaco);
        //referente a categoria

        pedaco = strtok(NULL,",");
        strcpy(pedaco_temporario.grupo,pedaco);
        //referente ao grupo

        pedaco = strtok(NULL,",\n");
        strcpy(pedaco_temporario.raridade,pedaco);
        //tipo

        pedaco_temporario.colada = 0;
        pedaco_temporario.quantidade_repetidas = 0;  
        
        if (album->quantidade_atual == album->quantidade_maxima){
            album->quantidade_maxima *= 2; // se estiver lotado a qntd maxima, dobra o tamanho maximo

            album->figurinhas = (Dados_Figurinha*) realloc(album->figurinhas, album->quantidade_maxima * sizeof(Dados_Figurinha)); // da um realloc do tamanho do vetor

            if (album->figurinhas == NULL){
                printf("\nErro ao realocar a memória referente ao album->figurinhas\n");
                return 0;
            }//um if pra avisar se deu erro na realocação do tamanho do vetor            
        }//if
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

    fclose(arquivo);

    return 1;
}//final da funcao que abre o arquivo


