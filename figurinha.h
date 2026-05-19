#ifndef FIGURINHA_H
#define FIGURINHA_H

//###########################################################//
//Estrutura
//###########################################################//
typedef struct{
   
   char codigo[15]; //codigo da figurinha
   char titulo[60]; //Nome do jogador
   char secao[60];  //categoria do jogador ou pg, ainda tem q ver
   char grupo[60];  //grupo q o jogador esta, tipo o Brasil é gp B,logo o jogador tbm
   char tipo[30];   //Raridade

   //controle de fluxo das fig

   int colada;               //pra ser mais facil, 1 ta colado, 0 nao ta
   int quantidade_repetidas; //quantas da mesma o jogador tem

}Dados_Figurinha;

//****************************//
//estruct do album
//****************************//
typedef struct{

    //aloca dinamicamente o album

    Dados_Figurinha *vetor; 
    int quantidade_atual;
    int quantidade_maxima;

}Album;

int Carrega_Csv(Album *album, const char *nome_arquivo); //chama da função referente ao arquivos.c


#endif