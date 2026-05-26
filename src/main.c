//###########################################################//
//Bibliotecas
//###########################################################//
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>
 #include <locale.h>

#ifdef _WIN32
    #include <windows.h>
#endif

 #include "figurinha.h"



//###########################################################//
//Fnc q inicializa o album
//###########################################################//
void inicializa_Album(Album *album){
   album->quantidade_atual = 0;
   album->quantidade_maxima = 10;

   album->figurinhas = (Dados_Figurinha*) malloc(album->quantidade_maxima * sizeof(Dados_Figurinha));

   if (album->figurinhas == NULL){
      printf("\nErro ao alocar a quantidade maxima de figurinhas, sinto muito\n");
      exit(1);
   }//final do if
}//fim da fnc


//###########################################################//
//Main
//###########################################################//
 int main(){

     #ifdef _WIN32
        SetConsoleOutputCP(1252); 
     #endif

        setlocale(LC_ALL, "Portuguese"); 
        
    // Avisa o windows pra ler os acentos
     
     Album meu_album;
     
     inicializa_Album(&meu_album);

     
     if (Carrega_Bin(&meu_album,"data/album.bin")){
         printf(">> Arquivo encontrado! Album carregado do HD.\n");
     }else{
        printf(">> Primeiro uso. Subindo o csv...\n");

        Carrega_Csv(&meu_album,"data/figurinhas2026.csv");
     }//if else
     //carrega o arq bin do hd, e se nao tiver, cria um

     Sanitiza_Nome(&meu_album);

   int op;

   qsort(meu_album.figurinhas,meu_album.quantidade_atual,sizeof(Dados_Figurinha),Ordena_lista_Bin);
   printf(">> Sistema: Banco de dados organizado em ordem alfabetica!\n\n");

   do{
     
        printf("\n========================================\n");
        printf("         ALBUM DA COPA 2026\n");
        printf("========================================\n");
        printf("1. Listar todas as figurinhas\n");
        printf("2. Abrir um pacotinho\n");
        printf("3. Pesquisar figurinha especifica\n");
        printf("4. Alterar dados de uma figurinha\n");
        printf("5. Excluir uma figurinha\n");
        printf("0. Sair (e salvar o progresso!)\n");
        printf("========================================\n");
        printf("Escolha uma opcao: ");
       //opções

       setbuf(stdin, NULL);
       scanf("%d",&op);


       switch (op){
        case 1:
            printf("\n>> No momento o album tem %d figurinhas. Aqui sao elas:\n", meu_album.quantidade_atual);

            Mostra_Album(&meu_album);
        break;
        
        case 2:
            printf("\n>> Abrindo um pacotinho...\n");  
        break; 
        
        case 3:
        {
            char nome[51];

            setbuf(stdin,NULL);
            printf("\n>> Digite o nome do jogador:...\n");
            fgets(nome,50,stdin);
            nome[strcspn(nome,"\n")] = '\0';

            int posicao = Procura_Jogador(&meu_album, nome);

            if (posicao != -1){
                printf("\n>> FIGURINHA ENCONTRADA! <<\n");
                printf("Numero: %s\n", meu_album.figurinhas[posicao].codigo);
                printf("Nome: %s\n", meu_album.figurinhas[posicao].nome_Jogador);
                printf("Categoria: %s\n", meu_album.figurinhas[posicao].secao);
                printf("Grupo: %s\n", meu_album.figurinhas[posicao].grupo);
                printf("Raridade: %s\n", meu_album.figurinhas[posicao].raridade);
            }else{
                    printf("\n>> Vish... Jogador '%s' nao encontrado no album.\n", nome);
            }//procura com base no return, se for != -1 tem, se nao o jogador nao esta
        break;
        }
        case 4:
            printf("\n>> Em desenvolvimento\n");
        break;
        
        case 0:
            printf("\n>> Salvando e saindo do programa. Até mais\n");
        
            if (Salva_Bin(&meu_album, "data/album.bin")){
                printf("\n>> Jogo salvo no HD!\n");
            }else{
                printf("\n>> [!ERRO!] Algo deu errado, o jogo nao foi salvo!\n");
            }//else
        
            printf(">> Finalizando jogo\n");
        break;

        default:
        printf("\n>> [ERRO] Opção inválida! Tenta de novo.\n");
    }//switch
    
   } while (op != 0);
   
    return 0;
 }//final da main