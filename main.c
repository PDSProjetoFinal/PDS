#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

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


// Declaracao de funcoes externas
void mainCambio();
void mainFilmes();
void mainBaresRest();


/*
*  Exibe as opções do menu principal
*/
char menuPrincipal() {
    char opcao;
    LIMPA_TELA;
    printf("|----------------------------|\n");
    printf("|  GUOL - Guia de recursos   |\n");
    printf("|----------------------------|\n");
    printf("\nEscolha sua opcao\n");
    printf("1 - Cambio\n");
    printf("2 - Bares e restaurantes\n");
    printf("3 - Filmes em cartaz\n");
    printf("0 - Sair\n");
    printf("\nInforme o numero da operacao desejada: ");
    LIMPA_BUFFER;
    opcao = getche();
    //scanf("%d", &opcao);
    return opcao;
}

int main() {
    char opcao;
    do {
        opcao = menuPrincipal();
        switch (opcao){
            case '1':
                mainCambio();
                break;
            case '2':
                mainBaresRest();
                break;
            case '3':
                mainFilmes();
                break;
            case '0':
                exit(0);
            default:
                printf("\n\nOpcao inexistente!\n");
                printf("\nPressione uma tecla para tentar novamente.\n");
                LIMPA_BUFFER;
                getche();
        }
    } while (1);
    return 0;
}
