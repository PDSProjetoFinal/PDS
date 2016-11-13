#include <stdio.h>
#include <stdlib.h>
//Se o sistema for Windows adiciona determinada biblioteca, e definindo comandos de limpar e esperar
#ifdef WIN32
    #include <windows.h>
    #define LIMPA_TELA system("cls")
    #define LIMPA_BUFFER fflush(stdin)
//Senão for Windows (ex.: Linux)
#else
    #include <unistd.h>
    #define LIMPA_TELA system("/usr/bin/clear")
    #define LIMPA_BUFFER __fpurge(stdin)
#endif

int menuFilmes() {
    int opcao;
    LIMPA_TELA;
    printf("|-------------------------------|\n");
    printf("|  GUOL - FILMES                |\n");
    printf("|-------------------------------|\n");
    printf("\nEscolha sua opcao\n");
    printf("1 - Incluir ...\n");
    printf("2 - Alterar ...\n");
    printf("3 - Excluir ...\n");
    printf("4 - Consultar ...\n");
    printf("5 - ...\n");
    printf("0 - Voltar ao menu anterior\n");
    printf("\nInforme o numero da operacao desejada: ");
    LIMPA_BUFFER;
    opcao = getche();
    //scanf("%d", &opcao);
    return opcao;
}

void mainFilmes() {
    int opcao;
    do {
        opcao = menuFilmes();
        switch (opcao){
            case '1':
                ///O que fazer;
                break;
            case '2':
                /// O que fazer;
                break;
            case '3':
                /// O que fazer;
                break;
            case '0':
                return;
                break;
            default:
                printf("\n\nOpcao inexistente!\n");
                printf("\nPressione uma tecla para tentar novamente.\n");
                LIMPA_BUFFER;
                getche();
        }
    } while (1);

    return;
}
