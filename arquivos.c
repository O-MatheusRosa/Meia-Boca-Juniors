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
        strcpy(pedaco_temporario.titulo,pedaco);
        //pedaço referente ao nome do jogador

        pedaco = strtok(NULL,",");
        strcpy(pedaco_temporario.secao,pedaco);
        //referente a categoria

        pedaco = strtok(NULL,",");
        strcpy(pedaco_temporario.grupo,pedaco);
        //referente ao grupo

        pedaco = strtok(NULL,",\n");
        strcpy(pedaco_temporario.tipo,pedaco);
        //tipo

        pedaco_temporario.colada = 0;
        pedaco_temporario.quantidade_repetidas = 0;  
        
        if (album->quantidade_atual == album->quantidade_maxima){
            album->quantidade_maxima *= 2; // se estiver lotado a qntd maxima, sobra o tamanho maximo

            album->vetor = (Dados_Figurinha*) realloc(album->vetor, album->quantidade_maxima * sizeof(Dados_Figurinha)); // da um realloc do tamanho do vetor

            if (album->vetor == NULL){
                printf("\nErro ao realocar a memoria referente ao album->vetor\n");
                return 0;
            }//um if pra avisar se deu erro na realocação do tamanho do vetor            
        }//if
        album->vetor[album->quantidade_atual] = pedaco_temporario;
        album->quantidade_atual++;
    }//while

    fclose(arquivo);
    return 1;
}//final da funcao

