#ifndef FIGURINHA_H
#define FIGURINHA_H

//###########################################################//
//Estrutura
//###########################################################//
typedef struct{
   
   char codigo[15]; //codigo da figurinha
   char nome_Jogador[60]; //titulo
   char secao[60];  //categoria do jogador ou pg, ainda tem q ver
   char grupo[60];  //grupo q o jogador esta, tipo o Brasil é gp B,logo o jogador tbm
   char raridade[30];   //tipo

   //controle de fluxo das fig

   int colada;               //pra ser mais facil, 1 ta colado, 0 nao ta
   int quantidade_repetidas; //quantas da mesma o jogador tem

}Dados_Figurinha;

//****************************//
//estruct do album
//****************************//
typedef struct{

    //aloca dinamicamente o album
    Dados_Figurinha *figurinhas; 

    int quantidade_atual;
    int quantidade_maxima;

}Album;

int Salva_Bin(Album *album, const char *nome_arquivo);//carrega da ram pro hd, ##case 0 da main

int Carrega_Bin(Album * album, const char *nome_arquivo);//carrega do hd pra ram

int Carrega_Csv(Album *album, const char *nome_arquivo); //chama da função referente ao carregamento do csv

void Mostra_Album(Album *album);//preciso nem explicar ne

int Procura_Jogador(Album *album,char *nome_buscado); // procura o jogador, ##case 3 da main

int Ordena_lista_Bin(const void *a, const void *b); 

void Sanitiza_Nome(Album *album);

void Abre_Pacotinho(Album *catalogo, Album *meu_album);

void Alterar_Figurinha(Album *meu_album);

void Exclui_Figurinha(Album *meu_album);

void Adiciona_Figurinha(Album *catalogo);
#endif