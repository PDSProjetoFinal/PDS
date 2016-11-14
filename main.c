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


// Declaracao de funcoes externas
void mainCambio();
void mainFilmes();
void mainBares();


/*
*  Exibe as opções do menu principal
*/
int menuPrincipal() {
    int opcao;
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
    scanf("%d", &opcao);
    return opcao;
}

int main() {
    int opcao;
    do {
        opcao = menuPrincipal();
        switch (opcao){
            case 1:
                mainCambio();
                break;
            case 2:
                mainBares();
                break;
            case 3:
                mainFilmes();
                break;
            case 0:
                exit(0);
            default:
                printf("\nAs opcoes disponiveis são 1, 2, 3 ou 0!\n\n");
                system("pause");
        }
    } while (1);
    return 0;
}
