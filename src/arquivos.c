//###########################################################//
//Bibliotecas
//###########################################################//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figurinha.h"
#include "utilitarios.h"

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
        
        Dados_Figurinha pedaco_temporario = {0};
        char *pedaco;

       
        pedaco = strtok(leitura_linha, ";,"); 
        if (pedaco == NULL) {
            continue;
        }
        strcpy(pedaco_temporario.codigo, pedaco);

        pedaco = strtok(NULL, ";,\r\n");
        if (pedaco != NULL) strcpy(pedaco_temporario.nome_Jogador, pedaco);

        pedaco = strtok(NULL, ";,\r\n");
        if (pedaco != NULL) strcpy(pedaco_temporario.secao, pedaco);

        pedaco = strtok(NULL, ";,\r\n");
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
//Funcao PRONTA pra receber um novo arquivo de figurinhas, no MESMO padrao
//do figurinhas2026.csv (codigo,titulo,secao,grupo,tipo).
//
//Serve pra quando sair uma atualizacao (ex: a "Update Edition" do album,
//com novas selecoes/figurinhas) ou qualquer outro arquivo extra que
//alguem queira somar ao catalogo geral do jogo.
//
//Ela so soma as figurinhas novas no catalogo que ja existe (nao
//apaga as que ja tinham sido carregadas antes), e ja sanitiza os nomes.
//
//Exemplo de uso (em qualquer lugar que tenha o catalago_geral):
//      Carrega_Novas_Figurinhas(&catalago_geral, "figurinhas_update.csv");
//###########################################################//
int Carrega_Novas_Figurinhas(Album *catalogo, const char *caminho_arquivo){

    int sucesso = Carrega_Csv(catalogo, caminho_arquivo);

    if (sucesso){
        Sanitiza_Nome(catalogo);
        printf(VERDE "\n>> Novo arquivo de figurinhas carregado com sucesso: %s\n" RESET, caminho_arquivo);
        printf(">> Catalogo agora tem %d figurinhas cadastradas no total.\n", catalogo->quantidade_atual);
    }else{
        printf(VERMELHO "\n>> [ERRO] Nao foi possivel carregar o arquivo: %s\n" RESET, caminho_arquivo);
        printf(VERMELHO ">> Confira se o arquivo existe e segue o padrao: codigo,titulo,secao,grupo,tipo\n" RESET);
    }//else

    return sucesso;
}//funcao
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

   if (album->quantidade_atual > 0) {
       fwrite(album->figurinhas, sizeof(Dados_Figurinha), album->quantidade_atual, arquivo);
   }//so salva se for maior q 0

   fclose(arquivo);

    return 1;
}//funcao que converte pra bin

//###########################################################//
//Funcao que sai do arquivo em binario e vai pra memoria ram
//###########################################################//
int Carrega_Bin(Album * album, const char *nome_arquivo){
    FILE *arquivo;

    arquivo = fopen(nome_arquivo,"rb");

    if (arquivo == NULL){
        return 0;
    }//caso nao consiga carregar do hd pra ram

    int qnt_lida = 0;
    //leitura do cabeçalho, jogo ele fora
    fread(&qnt_lida, sizeof(int), 1, arquivo);

    if (qnt_lida > 0){
        Dados_Figurinha *temp = (Dados_Figurinha*) realloc(album->figurinhas, qnt_lida * sizeof(Dados_Figurinha));
        
        if (temp != NULL) {
            album->figurinhas = temp; // Confirma o novo tamanho
            
            size_t lidos = fread(album->figurinhas, sizeof(Dados_Figurinha), qnt_lida, arquivo);
            album->quantidade_atual = lidos; 
            album->quantidade_maxima = lidos;
        }//if
    } else {
        
        album->quantidade_atual = 0;
        album->quantidade_maxima = 10;
    }//else
    
    fclose(arquivo);
    return 1;
}//final da funcao