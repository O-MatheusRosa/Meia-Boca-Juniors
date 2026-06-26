//###########################################################//
//Bibliotecas
//###########################################################//
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>
 #include <time.h>
 #include "figurinha.h"
 #include "utilitarios.h"
 #include "raylib.h"
 #include "textura.h"

//###########################################################//
//Fnc q inicializa o album
//###########################################################//
void inicializa_Album(Album *album){
   album->quantidade_atual = 0;
   album->quantidade_maxima = 10;

   album->figurinhas = (Dados_Figurinha*) malloc(album->quantidade_maxima * sizeof(Dados_Figurinha));

   if (album->figurinhas == NULL){
      printf(VERMELHO "\nErro ao alocar a quantidade maxima de figurinhas, sinto muito\n" RESET);
      Som_Erro();
      exit(1);
   }//final do if
}//fim da fnc


//###########################################################//
//Main
//###########################################################//
int main(void){

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%   PARTE GRAFICA   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    InitWindow(1280,720,"Meia Boca Juniors");//inicializa a tela
    InitAudioDevice();//inicializa o audio
    SetTargetFPS(60);//seta o fps

    SetExitKey(KEY_X);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            //inicia as texturas do menu, e o som
        Image imagem_gigante = LoadImage("assets/fundopika (1).png");

        ImageResize(&imagem_gigante, 1280, 720);

        Texture2D fundo = LoadTextureFromImage(imagem_gigante);

        Music musica_menu = LoadMusicStream("assets/musica1.mp3");
        SetMusicVolume(musica_menu,0.4f);



        PlayMusicStream(musica_menu);        
        bool noMenu = true;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            
int acao = Tela_Home(fundo,musica_menu);

//-------------------------------------incializa o album---------------------------------------------------
Album meu_album;
meu_album.figurinhas = NULL;
meu_album.quantidade_atual = 0;
meu_album.quantidade_maxima = 0;

Album catalago_geral;
catalago_geral.figurinhas = NULL;
catalago_geral.quantidade_atual = 0;
catalago_geral.quantidade_maxima = 0;

inicializa_Album(&meu_album);
inicializa_Album(&catalago_geral);

Carrega_Csv(&catalago_geral,"figurinhas2026.csv");
Sanitiza_Nome(&catalago_geral);
    Sanitiza_Secao(&catalago_geral);
//--------------------------------------------------------------------------------------------------------

                                    Limpar_Tela();
     
//-----------------------------------carrega o album--------------------------------------------------------
if (Carrega_Bin(&meu_album,"save.dat")){
    printf(VERDE "\n\n>> Arquivo encontrado! Album carregado do HD.\n" RESET);
}else{
    printf("\n\n>> Primeiro uso. Album zerado\n\n");
}//if else
//carrega o arq bin do hd, e se nao tiver, cria um
Sanitiza_Nome(&meu_album);
Sanitiza_Secao(&meu_album);
//-----------------------------------------------------------------------------------------------------------

srand(time(NULL));

//-----------------------------------------ordena��o em ordem alfabetica-------------------------------------
qsort(meu_album.figurinhas,meu_album.quantidade_atual,sizeof(Dados_Figurinha),Ordena_lista_Bin);
printf("\n\n>> Sistema: Banco de dados organizado em ordem alfabetica!\n\n");
//-----------------------------------------------------------------------------------------------------------

if(acao == 1){

    Image imagem_dia = LoadImage("assets/mapinha.png"); 
    ImageResize(&imagem_dia, 1280, 720);
    Texture2D fundo_dia = LoadTextureFromImage(imagem_dia);
    UnloadImage(imagem_dia); 

    Image imagem_noite = LoadImage("assets/mapa_noite (2).png"); 
    ImageResize(&imagem_noite, 1280, 720);
    Texture2D fundo_noite = LoadTextureFromImage(imagem_noite);
    UnloadImage(imagem_noite); 

    StopMusicStream(musica_menu); 

    Music musica_cidade = LoadMusicStream("assets/cidade.mp3"); 
    PlayMusicStream(musica_cidade);
    SetMusicVolume(musica_cidade,0.2f);

    Tela_Jogo(fundo_dia, fundo_noite, musica_cidade, &meu_album, &catalago_geral);

    UnloadTexture(fundo_dia);
    UnloadTexture(fundo_noite);
    UnloadMusicStream(musica_cidade); 

} else {
    printf("\nFechou a janela, saindo.....\n");
}


//Teste_AbrirPacotinho(&meu_album, &catalago_geral);



//------------------------------------------menu-------------------------------------------------------------
int op;

/*
do{
    
    printf(AZUL "\n========================================\n" RESET);
    printf("           ALBUM DA COPA 2026\n");
    printf(AZUL "========================================\n" RESET);
        printf("1. Listar todas as figurinhas\n");
        printf("2. Abrir um pacotinho\n");
        printf("3. Pesquisar figurinha especifica\n");
        printf("4. Alterar dados de uma figurinha\n");
        printf("5. Excluir uma figurinha\n");
        printf("6. Incluir uma nova figurinha\n");
        printf("0. Sair (e salvar o progresso!)\n");
        printf("========================================\n");
        printf(AMARELO "Escolha uma opcao: " RESET);
        //op��es
        
        setbuf(stdin, NULL);
        scanf("%d",&op);
        Som_Menu();
        
        
        switch (op){
            case 1:
                printf("\n>> Opcao 1:");
                
                Mostra_Album(&meu_album);
        break;     
//----------------------------------------------------------------------------------------------------------
        case 2:
            printf("\n\n>> Abrindo um pacotinho...\n\n"); 
            
            Abre_Pacotinho(&catalago_geral, &meu_album);
        break; 
//----------------------------------------------------------------------------------------------------------  
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
        }//case 3
//-----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
            case 4:
                printf("\n>> Opcao 4: \n");

                Alterar_Figurinha(&meu_album);
            break;
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
            case 5:
                printf("\n>> Opcao 5:\n");

                Exclui_Figurinha(&meu_album);
            break;
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
            case 6:
                printf("\n>> Opcao 6:\n");

                Adiciona_Figurinha(&catalago_geral);
            break;
//-----------------------------------------------------------------------------------------------------------
            case 0:
                printf("\n>> Salvando e saindo do programa. At� mais\n");
            break;
//-----------------------------------------------------------------------------------------------------------
        default:
             printf(VERMELHO "\n>> [ERRO] Op��o inv�lida! Tenta de novo.\n" RESET);
             Som_Erro();
    }//switch
    
   } while (op != 0);
//----------------------------------------------------------------------------------------------------------
*/
    if (Salva_Bin(&meu_album, "save.dat")) {
        printf("\n>> Jogo salvo no HD!\n");
    } else {
        printf(VERMELHO "\n>> [!ERRO!] Algo deu errado, o jogo nao foi salvo!\n" RESET);
        Som_Erro();
    }//else
    printf(">> Finalizando jogo\n"); 

    UnloadTexture(fundo);
    UnloadMusicStream(musica_menu);
    CloseAudioDevice();
    CloseWindow();

    free(catalago_geral.figurinhas);
    free(meu_album.figurinhas);
    return 0;
 }//final da main