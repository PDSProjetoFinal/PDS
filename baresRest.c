#include <stdio.h>
#include <stdlib.h>
#define VERDADEIRO 1
#define FALSO 0

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

typedef struct Bares {
    char nomeEstabelecimento[25];
    char Endereco[50];
    int Telefone;
    double Horario;
    char Tipodecomida[25];
    double Gastomedio ;
    struct BareseRestaurantes *proximo;
} tBareseRestaurantes;

FILE * arquivoBares;
long int    posicao;

// Funcoes externas
FILE * abrirLeituraGravacao(char nomeArquivo[80]);
void fecharArquivo(FILE * arq);


/*
 *  Imprime o cabecalho de Bares/Restaurantes
 */
void imprimeCabecalhoBares() {
    printf("\n");
    printf("|-------------------------------|\n");
    printf("|  GUOL - Bares/Restaurantes    |\n");
    printf("|-------------------------------|\n");
    printf("\n");
}

/*
 *  Exibe as opções do menu de Estabelecimentos
 *  e permite ao usuario informar a opcao desejada
 */


int menuBares() {
    int opcao;
    LIMPA_TELA;
    imprimeCabecalhoBares();
    printf("\nEscolha sua opcao\n");
    printf("1 - Cadastrar Bares/Restaurantes\n");
    printf("2 - Pesquisar Bares/Restaurantes\n");
    printf("3 - Listar Bares/Restaurantes\n");
    printf("4 - Alterar Bares/Restaurantes\n");
    printf("5 - Excluir Bares/Restaurantes\n");
    printf("0 - Voltar ao menu anterior\n");
    printf("\nInforme o numero da operacao desejada: ");
    LIMPA_BUFFER;
    scanf("%d", &opcao);
    return opcao;
}

/*
 *  Exibe as opções do menu de Bares/Restaurantes
 *  e permite ao usuario informar a opcao desejada
 */


 int menuListarBares() {
    int opcao;
    LIMPA_TELA;
    imprimeCabecalhoBares();
    printf("Escolha sua opcao\n");
    printf("1 - Listagem por ordem alfabetica\n");
    printf("0 - Voltar ao menu anterior\n");
    printf("\nInforme o numero da operacao desejada: ");
    LIMPA_BUFFER;
    scanf("%d", &opcao);
    return opcao;
}

/*
*  Retorna o primeiro ponteiro cujo Bar eh menor que O BAR informada
*  Retorna NULL se O BAR informada for menor que a primeira da lista
*/

tBareseRestaurantes * baresAnterior(tBareseRestaurantes* lista, tBareseRestaurantes* novo, int ordem) {
    tBareseRestaurantes * aux = NULL;
    while (lista != NULL) {
        if (ordem == 1) { // Alfabetica
            if (strcmp(lista->nomeEstabelecimento, novo->nomeEstabelecimento) > 0) {
                return aux;
            }
        } else {         // Valor de variacao
            if (lista->Gastomedio > novo->Gastomedio) {
                return aux;
            }
        }
        aux = lista;
        lista = lista->proximo;
    }
    return aux;
}

/*
 *  Valida os valores
 */
int validaBares(tBareseRestaurantes *bares) {
    int erros=0;
    if (strcmp(bares->nomeEstabelecimento, "") == 0) {
        printf("O nome do Bar/Restaurante deve ser informado\n"); erros++; }
    if (bares->nomeEstabelecimento[0] < 65 || bares->nomeEstabelecimento[0] > 122 ) {
        printf("O nome do Bar/Restaurante deve iniciar com uma letra: %d\n", bares->nomeEstabelecimento[0]); erros++; }
    if (bares->Tipodecomida[0] < 65 || bares->Tipodecomida[0] > 122 ) {
        printf("O nome do tipo de comida deve iniciar com uma letra: %d\n", bares->Tipodecomida[0]); erros++; }

    if (erros > 0) {
        return FALSO;
    } else {
        return VERDADEIRO;
    }
}


/*
 *  Pesquisa se o estabelecimento informado já existe
 */
tBareseRestaurantes * existebar(tBareseRestaurantes *pesquisar) {
    tBareseRestaurantes * bares = (tBareseRestaurantes *) malloc(sizeof(tBareseRestaurantes));

    // Volta ao inicio do arquivo
    rewind(arquivoBares);
    posicao = 0;

    // Percorre o arquivo ate o final
    while(!feof(arquivoBares)) {
        // Le um registro do arquivoBares e armazena na variavel bar
        fread(bares, sizeof(tBareseRestaurantes), 1, arquivoBares);
        posicao = posicao + sizeof(tBareseRestaurantes);
        if (!feof(arquivoBares)) {
            if (strcmp(bares->nomeEstabelecimento, pesquisar->nomeEstabelecimento) == 0) {
                return bares;
            }
        }
    }
    return NULL;
}

/*
 * Cadastra um novo valor de moeda
 */
void cadastrarBareserestaurantes() {

    // Aloca espaco de memoria e inicia os valores
    tBareseRestaurantes * bares = (tBareseRestaurantes *) malloc(sizeof(tBareseRestaurantes));
    strcpy(bares->nomeEstabelecimento, "                        ");
    bares->proximo = NULL;

    // Imprime o cabecalho
    LIMPA_TELA;
    imprimeCabecalhoBares();
    printf("Cadastrar Bares/Restaurantes\n");

    //Le os dados do Bar
    printf("\nNome do Bar/Restaurante: ");
    LIMPA_BUFFER;
    scanf("%[a-z A-Z]s", bares->nomeEstabelecimento);
    printf("\nEndereço: ");
    scanf("%s", &bares->Endereco);
    printf("\nTelefoe: ");
    scanf("%d", &bares->Telefone);
    printf("\nHorario: ");
    scanf("%f", &bares->Horario);
    printf("\nTipo de comida: ");
    scanf("%c", &bares->Tipodecomida);
    printf("\nGasto medio: ");
    scanf("%f", &bares->Gastomedio);
    LIMPA_BUFFER;

     // Efetua as validacoes e salva
    if (validaBares(bares)) {

        if (existebar(bares) != NULL) {
            // BAR/RESTAURANTE ja existe e nao pode ser cadastrada novamente
            printf("\nBar/Restaurante %s ja existe. Nao foi possivel incluir.\n", bares->nomeEstabelecimento);

        } else {
            // Gravar no arquivo
            fseek(arquivoBares, 0L, SEEK_END); // Vai ao final do arquivo
            if (fwrite(bares, sizeof(tBareseRestaurantes), 1, arquivoBares) != 1) {  // Grava e verifica se houve erro
                printf("\nErro ao incluir registro\n");
                exit(1);
            };

            // Mensagem de sucesso
            printf("\nBares/Restauranes '%s' incluido\n", bares->nomeEstabelecimento);
        }
    } else {
        printf("\nNao foi possivel incluir o Bar/Restaurante %s \n", bares->nomeEstabelecimento);
    }
    system("pause");
}

/*
 * Alterar informaçoes dos bares
 */
void alterarBarerestaurante() {

    // Aloca espaco de memoria
    tBareseRestaurantes * bares = (tBareseRestaurantes *) malloc(sizeof(tBareseRestaurantes));

    // Imprime o cabecalho
    LIMPA_TELA;
    imprimeCabecalhoBares();
    printf("Alterar Bar/Restaurante\n");

    //Le os dados do estabelecimento
    printf("\nNome do Estabelecimento: ");
    LIMPA_BUFFER;
    scanf("%[a-z A-Z]s", bares->nomeEstabelecimento);

    bares = existebar(bares);
    if (bares == NULL) {
        // Estabelecimento ainda nao existe
        printf("\nEstabelecimento nao encontrado. Nao foi possivel alterar.\n");
        system("pause");
        return;
    }

 printf("\nNome Estabelecimento (%s): ", bares->nomeEstabelecimento);
    scanf("%s", &bares->nomeEstabelecimento);
    printf("\nEndereço (%s): ", bares->Endereco);
    scanf("%s", &bares->Endereco);
    printf("\nTelefone (%d): ", bares->Telefone);
    scanf("%d", &bares->Telefone);
    printf("\nHorario (%f): ", bares->Horario);
    scanf("%f", &bares->Horario);
    printf("\nTipo de Comida (%s): ", bares->Tipodecomida);
    scanf("%s", &bares->Tipodecomida);
    printf("\nGasto Medio (%3f): ", bares->Gastomedio);
    scanf("%f", &bares->Gastomedio);
    LIMPA_BUFFER;

    // Efetua as validacoes e salva
    if (validaBares(bares)) {

        // Posiciona o ponteiro para gravar
        fseek(arquivoBares, posicao - sizeof(tBareseRestaurantes), SEEK_SET );
        // Gravar no arquivo
        if (fwrite(bares, sizeof(tBareseRestaurantes), 1, arquivoBares) != 1) {  // Grava e verifica se houve erro
            printf("\nErro ao alterar registro\n");
            exit(1);
        };

        // Mensagem de sucesso
        printf("\nBar/Restaurane '%s' alterada\n", bares->nomeEstabelecimento);
    } else {
        printf("\nNao foi possivel alterar o Estabelecimento %s \n", bares->nomeEstabelecimento);
    }
    system("pause");

/*
 * Carrega os Estabelecimentos cadastrados
 */
tBareseRestaurantes * carregarB(int ordem) {

    tBareseRestaurantes * inicio = NULL;
    tBareseRestaurantes * aux    = NULL;

    // Volta ao inicio do arquivo
    rewind(arquivoBares);

    // Percorre o arquivo ate o final
    while(!feof(arquivoBares)) {
        // Le um registro do arquivoBares e armazena na variavel bares
        tBareseRestaurantes * bares = (tBareseRestaurantes *) malloc(sizeof(tBareseRestaurantes));
        fread(bares, sizeof(tBareseRestaurantes), 1, arquivoBares);
        if (!feof(arquivoBares)) {
            aux = baresAnterior(inicio, bares, ordem);
            if (aux == NULL) {
                bares->proximo = inicio;
                inicio = bares;
            } else {
                bares->proximo = aux->proximo;
                aux->proximo= bares;
            }
        }
    }

    return inicio;
}

/*
 * Pesquisa o Estabelecimento no arquivo, pelo nome
 */
tBareseRestaurantes * carregarBar(char nomePesquisa[]) {

    tBareseRestaurantes * bares = (tBareseRestaurantes *) malloc(sizeof(tBareseRestaurantes));

    // Volta ao inicio do arquivo
    rewind(arquivoBares);

    // Percorre o arquivo ate o final ou localizar o Estabelecimento
    while(!feof(arquivoBares)) {
        // Le um registro do arquivobares e armazena na variavel bares
        fread(bares, sizeof(tBareseRestaurantes), 1, arquivoBares);
        if (!feof(arquivoBares)) {
            if (strcmp(bares->nomeEstabelecimento, nomePesquisa) == 0) {
                return bares;
            }
        }
    }
    return NULL;
}


/*
 * Exibe os dados dos estabelecimento das lista
 */

 void exibirBares(tBareseRestaurantes * bares, char titulo[40]) {

    // Imprime o cabecalho
    LIMPA_TELA;
    imprimeCabecalhoBares();
    printf("%s\n\n", titulo);
    printf("%-25s %11s %11s %11s %11s %11s %11s\n", "Estabelecimento", "Endereco", "% Endereco", "Telefone", "Horario", "Tipodecomida", "Gastomedio");

    // Se nao houver bares/restaurantes para exibir
    if (bares == NULL) {
        printf("\nNenhum Bar/Restaurante para exibir\n");
    }

    // Exibe Bares e Restaurantes
    while(bares != NULL) {
        printf("%s %s %d %f %s %f %\n",
            bares->nomeEstabelecimento,
            bares->Endereco,
            bares->Telefone,
            bares->Horario,
            bares->Tipodecomida,
            bares->Gastomedio);
            bares = bares->proximo;
    }

    // Aguarda comando do usuario para retornar
    printf("\n");
    system("pause");
    return;
}


/*
 * Lista os estabelecimentos cadastrados
 */
void listarBarOrdem(int ordem) {

    // Carrega os estabelecimentos em memoria
    tBareseRestaurantes * bares = carregarB(ordem);

    // Configura o titulo
    char titulo[40];
    if (ordem ==1) {
        strcpy(titulo, "Bares/Restaurantes em ordem alfabetica");
    }

    // Imprime os dados
    exibirBares(bares, titulo);

    // Libera a memoria
    free(bares);
    return;
}

/*
 * Controla as opcoes de listar moeda
 */
void listarBar() {
    int opcao;
    do {
        opcao = menuListarBares();
        switch (opcao){
            case 1:
                listarBarOrdem(1);
                break;
            case 2:
                listarBarOrdem(2);
                break;
            case 0:
                return;
                break;
            default:
                printf("\nOpcao inexistente!\n\n");
                system("pause");
        }
    } while (1);
}


/*
 * Pesquisar mooeda pelo nome
 */
void pesquisarBar() {

    char nomePesquisa[25];

    // Imprime o cabecalho
    LIMPA_TELA;
    imprimeCabecalhoBares();
    printf("Pesquisar Estabelecimento\n");

    // Le o nome do Estabelecimento
    printf("\nNome do estabelecimento: ");
    LIMPA_BUFFER;
    scanf("%[a-z A-Z]s", nomePesquisa);

    // Pesquisa a moeda pelo nome
    tBareseRestaurantes * bares = carregarB(nomePesquisa);

    // Imprime os dados da moeda
    exibirBares(bares, "Moeda pesquisada");

    // Libera a memoria
    free(bares);
    return;
}

/*
 *  Tratamento das opcoes gerais de cambio
 */


void mainBares() {
    int opcao;
    arquivoBares = abrirLeituraGravacao("dadosBares.txt");
    do {
        opcao = menuBares();
        switch (opcao){
            case 1:
                cadastrarBareserestaurantes();
                break;
            case 2:
                pesquisarBar();
                break;
            case 3:
                listarBar();
                break;
            case 4:
                alterarBarerestaurante();
            case 0:
                return;
                break;
            default:
                printf("\nOpcao inexistente!\n\n");
                system("pause");
        }
    }while (1);
    fecharArquivo(arquivoBares);


    return;
}

}






