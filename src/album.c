//###########################################################//
//Bibliotecas
//###########################################################//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figurinha.h"
#include "utilitarios.h"

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
        if (album->figurinhas[i].colada == 1){
            printf("\n[ X ] Nome: %-20s || Codigo: %7s || Raridade: %10s || Repetidas: %d\n",album->figurinhas[i].nome_Jogador,album->figurinhas[i].codigo,album->figurinhas[i].raridade,album->figurinhas[i].quantidade_repetidas);
        }else{
            printf("\n[   ] Nome: %-20s || Codigo: %7s || Raridade: %10s || Repetidas: %d\n",album->figurinhas[i].nome_Jogador,album->figurinhas[i].codigo,album->figurinhas[i].raridade,album->figurinhas[i].quantidade_repetidas);
        }//else caso n tenha colado        
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

void Abre_Pacotinho(Album *catalogo, Album *meu_album){

   Animacao_Loading();

   if (catalogo->quantidade_atual == 0) {
        printf(VERMELHO "\n[ERRO] O catalogo esta vazio (CSV nao carregou)! Nao tem como sortear.\n" RESET);
        return; 
    }//caso n abra

    for (int i = 0; i < 7; i++){
        int sorteado = rand() % catalogo->quantidade_atual;

        char *nome_sorteado = catalogo->figurinhas[sorteado].nome_Jogador;
        int posicao = Procura_Jogador(meu_album,nome_sorteado);

        if (posicao != -1){
            meu_album->figurinhas[posicao].quantidade_repetidas++;
            printf(">> Repetida: %s, ja tenho %d dela.....\n",meu_album->figurinhas[posicao].nome_Jogador,meu_album->figurinhas[posicao].quantidade_repetidas);
        }else{

        if (meu_album->quantidade_atual >= meu_album->quantidade_maxima){
            
            if (meu_album->quantidade_maxima == 0) {
                meu_album->quantidade_maxima = 10;
            } else {
                meu_album->quantidade_maxima *= 2;
            }//else

            Dados_Figurinha *temp = realloc(meu_album->figurinhas, meu_album->quantidade_maxima * sizeof(Dados_Figurinha));
            
            if (temp == NULL){
                printf(VERMELHO "\n[ERRO] Memoria cheia! Nao deu pra expandir o album.\n" RESET);
                return; // Sai em seguranca
            }
            meu_album->figurinhas = temp;
        }//if de verificação se encheu ou nao a memoria 

                meu_album->figurinhas[meu_album->quantidade_atual] = catalogo->figurinhas[sorteado];
                
                meu_album->figurinhas[meu_album->quantidade_atual].colada = 0;
                meu_album->figurinhas[meu_album->quantidade_atual].quantidade_repetidas = 0;
                
                printf(AZUL "\n>> Nova figurinha: " RESET);
                printf(CIANO " %s\n" RESET, meu_album->figurinhas[meu_album->quantidade_atual].nome_Jogador);
                Som_Sucesso();
        
                        setbuf(stdin,NULL);
                        int escolha;
                        printf(VERMELHO ">> Colar no album? (1-sim/0-nao): " RESET);
                        scanf("%d",&escolha);

                        printf("\n");

                        if (escolha == 1){
                             meu_album->figurinhas[meu_album->quantidade_atual].colada = 1;
                        }else{
                            printf(AMARELO "OK, figurinha indo pro bolo....\n\n" RESET);
                        }//else
                meu_album->quantidade_atual++;   
        }//else 
    }//for   
}//funcao

void Alterar_Figurinha(Album *meu_album){

    char nome[51];

    setbuf(stdin,NULL);
    printf("\n>> Digite o nome do jogador: \n");
    fgets(nome,50,stdin);
    nome[strcspn(nome,"\n")] = '\0';

    int posicao = Procura_Jogador(meu_album, nome);

    if (posicao == -1){
        printf("\nvoce ainda nao achou esse jogador!\n");
    }else{
        printf("\n>> ENCONTRADA: %s <<\n",meu_album->figurinhas[posicao].nome_Jogador);
        printf("\n>> QUANTIDADE: %d <<\n",meu_album->figurinhas[posicao].quantidade_repetidas);

        if (meu_album->figurinhas[posicao].colada == 1) {
            printf("Status: [ X ] JA COLADA!\n");
            printf(">> Voce nao pode colar uma figurinha que ja esta colada...\n");
        } else {
            printf("Situacao: [   ] NAO COLADA.\n");
            
            int escolha;
            printf("\nDeseja colar essa figurinha no album? (1-Sim / 0-Nao): ");
            scanf("%d", &escolha);
            
            if (escolha == 1) {
                meu_album->figurinhas[posicao].colada = 1;
                printf("\n>> FOI! A cartinha do %s foi colada no album!\n", meu_album->figurinhas[posicao].nome_Jogador);
            } else {
                printf("\n>> Operacao cancelada. A cartinha continua solta por ai.\n");
            }//else de colada ou nao
        }//quer colar?
    }//else de procura de figurinhas
}//funcao

void Exclui_Figurinha(Album *meu_album){
    char nome[51];

        setbuf(stdin,NULL);
        printf("\n>> Digite o nome do jogador: \n");
        fgets(nome,50,stdin);
        nome[strcspn(nome,"\n")] = '\0';

    int posicao = Procura_Jogador(meu_album, nome);

    if (posicao == -1){
        printf("\nvoce ainda nao achou esse jogador!\n");
    }else{
        for (int i = posicao; i < meu_album->quantidade_atual - 1; i++){
            meu_album->figurinhas[i] = meu_album->figurinhas[i+1];
        }//for
        
        meu_album->quantidade_atual--;

        printf("\nFIGURINHA EXCLUIDA!!!\n");
    }//else
}//funcao

void Adiciona_Figurinha(Album *catalogo){
    printf(CIANO "\n>> [ MODO ADMINISTRADOR ] CADASTRAR NOVA FIGURINHA GLOBAL <<\n" RESET);

    if (catalogo->quantidade_atual == catalogo->quantidade_maxima){
        catalogo->quantidade_maxima *=2;
        catalogo->figurinhas = realloc(catalogo->figurinhas, catalogo->quantidade_maxima * sizeof(Dados_Figurinha));

        if (catalogo->figurinhas == NULL){
            printf(VERMELHO "\n[ERRO FATAL] Falha ao expandir a memoria do catalogo!\n" RESET);
            return;
        }//caso n de o realloc
    }//final do if

    int pos = catalogo->quantidade_atual;
    setbuf(stdin, NULL);

    printf("Digite o Codigo (Ex: BRA-01): ");
    fgets(catalogo->figurinhas[pos].codigo, 10, stdin);
    catalogo->figurinhas[pos].codigo[strcspn(catalogo->figurinhas[pos].codigo, "\n")] = '\0';

    setbuf(stdin, NULL);
    printf("Digite o Nome do Jogador / Titulo: ");
    fgets(catalogo->figurinhas[pos].nome_Jogador, 50, stdin);
    catalogo->figurinhas[pos].nome_Jogador[strcspn(catalogo->figurinhas[pos].nome_Jogador, "\n")] = '\0';

    setbuf(stdin, NULL);
    printf("Digite a Secao (Ex: Brasil, Lendas): ");
    fgets(catalogo->figurinhas[pos].secao, 30, stdin);
    catalogo->figurinhas[pos].secao[strcspn(catalogo->figurinhas[pos].secao, "\n")] = '\0';

    setbuf(stdin, NULL);
    printf("Digite o Tipo/Raridade (Ex: Base, Ouro, Bordo): ");
    fgets(catalogo->figurinhas[pos].raridade, 20, stdin);
    catalogo->figurinhas[pos].raridade[strcspn(catalogo->figurinhas[pos].raridade, "\n")] = '\0';

    catalogo->figurinhas[pos].colada = 0;
    catalogo->figurinhas[pos].quantidade_repetidas = 0;

    catalogo->quantidade_atual++;

    FILE *arquivo = fopen("figurinhas2026.csv", "a"); // Ajuste o caminho
    
    if (arquivo != NULL) {
        fprintf(arquivo, "\n%s;%s;%s;%s", catalogo->figurinhas[pos].codigo, catalogo->figurinhas[pos].nome_Jogador, catalogo->figurinhas[pos].secao, catalogo->figurinhas[pos].raridade);
        fclose(arquivo);
    } else {
        printf(VERMELHO "\n[AVISO] A carta foi salva na RAM, mas houve erro ao abrir o arquivo .csv!\n" RESET);
    }//fim do else

    printf(VERDE "\n>> SUCESSO! %s foi adicionado(a) oficialmente ao universo do jogo!\n" RESET, catalogo->figurinhas[pos].nome_Jogador);
    Som_Sucesso(); // Beep da vitoria
}//final da fnc