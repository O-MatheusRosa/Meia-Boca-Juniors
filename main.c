//###########################################################//
//Bibliotecas
//###########################################################//
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>
 #include "figurinha.h"

//###########################################################//
//Fnc q inicializa o album
//###########################################################//
void inicializa_Album(Album *album){
   album->quantidade_atual = 0;
   album->quantidade_maxima = 10;

   album->vetor = (Dados_Figurinha*) malloc(album->quantidade_maxima * sizeof(Dados_Figurinha));

   if (album->vetor == NULL){
      printf("\nErro ao abrir o arquivo, sinto muito\n");
      exit(1);
   }//final do if
}//fim da fnc


//###########################################################//
//Main
//###########################################################//
 int main(){

   Album meu_album;

   inicializa_Album(&meu_album);

   int op;

   do{
     
       printf("\n===================================\n");
       printf("      ALBUM DA COPA 2026\n");
       printf("===================================\n");
       printf("1. Carregar figurinhas do CSV\n");
       printf("2. Mostrar total de figurinhas\n");
       printf("3. Abrir pacotinho\n");
       printf("4. Sair\n");
       printf("Escolha uma opcao: ");
       //opções

       setbuf(stdin, NULL);
       scanf("%d",&op);


       switch (op){
         case 1:
        if (Carrega_Csv(&meu_album,"figurinhas2026.csv")){
            printf("\nArquivo Carregado!\n");
        }else{
            printf("\nErro ao carregar o arquivo\n");
        }//if else de abertura do csv de fig
        
         break; 
               
           case 2:
               printf("\n>> No momento o album tem %d figurinhas.\n", meu_album.quantidade_atual);
               break;

           case 3:
               printf("\n>> Abrindo um pacotinho...\n");
               break;

           case 4:
               printf("\n>> Salvando e saindo do programa. Ate mais\n");
               break;

           default:
               printf("\n>> [ERRO] Opcao invalida! Tenta de novo.\n");
       }//switch
      
   } while (op != 4);
   
    return 0;
 }//final da main

 