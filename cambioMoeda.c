#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

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

typedef struct MOEDA {
    char nomeMoeda[25];
    double valorCompra;
    double valorVenda;
    double valorVariacao;
    double pcVariacao;
    double valorMinimo;
    double valorMaximo;
    int    icExcluido;
    struct MOEDA *proximo;
} tMoeda;

FILE * arquivoCambio;
long int    posicao;

// Funcoes externas
FILE * abrirLeituraGravacao(char nomeArquivo[80]);
void fecharArquivo(FILE * arq);


/*
 *  Imprime o cabecalho do cambio
 */
void imprimeCabecalhoCambio() {
    printf("\n");
    printf("|-------------------------------|\n");
    printf("|  GUOL - CAMBIO                |\n");
    printf("|-------------------------------|\n");
    printf("\n");
}

/*
 *  Exibe as opções do menu de cambio
 *  e permite ao usuario informar a opcao desejada
 */
int menuCambio() {
    int opcao;
    LIMPA_TELA;
    imprimeCabecalhoCambio();
    printf("Escolha sua opcao\n");
    printf("1 - Cadastrar moeda\n");
    printf("2 - Pesquisar moeda\n");
    printf("3 - Listar moedas\n");
    printf("4 - Alterar moeda\n");
    printf("5 - Excluir moeda\n");
    printf("0 - Voltar ao menu anterior\n");
    printf("\nInforme o numero da operacao desejada: ");
    LIMPA_BUFFER;
    opcao = getche();
    //scanf("%d", &opcao);
    return opcao;
}

/*
 *  Exibe as opções do menu de cambio
 *  e permite ao usuario informar a opcao desejada
 */
int menuListarMoeda() {
    int opcao;
    LIMPA_TELA;
    imprimeCabecalhoCambio();
    printf("Escolha sua opcao\n");
    printf("1 - Listagem por ordem alfabetica\n");
    printf("2 - Listagem pela variacao\n");
    printf("0 - Voltar ao menu anterior\n");
    printf("\nInforme o numero da operacao desejada: ");
    LIMPA_BUFFER;
    opcao = getche();
    //scanf("%d", &opcao);
    return opcao;
}

/*
*  Retorna o primeiro ponteiro cuja moeda eh menor que a moeda informada
*  Retorna NULL se a moeda informada for menor que a primeira da lista
*/
tMoeda * moedaAnterior(tMoeda* lista, tMoeda* novo, int ordem) {
    tMoeda * aux = NULL;
    while (lista != NULL) {
        if (ordem == 1) { // Alfabetica
            if (strcasecmp(lista->nomeMoeda, novo->nomeMoeda) > 0) {
                return aux;
            }
        } else {         // Valor de variacao
            if (lista->valorVariacao > novo->valorVariacao) {
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
int validaMoeda(tMoeda *moeda) {
    int erros=0;
    if (strcmp(moeda->nomeMoeda, "") == 0) {
        printf("O nome da moeda deve ser informado\n"); erros++; }
    if (moeda->nomeMoeda[0] < 65 || moeda->nomeMoeda[0] > 122 ) {
        printf("O nome da moeda deve iniciar com uma letra:\n"); erros++; }
    if (moeda->valorMinimo > moeda->valorMaximo) {
        printf("Valor minimo deve ser menor ou igual ao maximo\n"); erros++; }
    if (moeda->valorMinimo > moeda->valorCompra) {
        printf("Valor de compra deve ser maior que o valor minimo\n"); erros++; }
    if (moeda->valorMaximo < moeda->valorCompra) {
        printf("Valor de compra deve ser menor que o valor maximo\n"); erros++; }
    if (erros > 0) {
        return FALSO;
    } else {
        return VERDADEIRO;
    }
}

/*
 *  Pesquisa se a moeda informada já existe
 */
tMoeda * existeMoeda(tMoeda *pesquisar) {
    tMoeda * moeda = (tMoeda *) malloc(sizeof(tMoeda));

    // Volta ao inicio do arquivo
    rewind(arquivoCambio);
    posicao = 0;

    // Percorre o arquivo ate o final
    while(!feof(arquivoCambio)) {
        // Le um registro do arquivoCambio e armazena na variavel moeda
        fread(moeda, sizeof(tMoeda), 1, arquivoCambio);
        posicao = posicao + sizeof(tMoeda);
        if (!feof(arquivoCambio)) {
            if (moeda->icExcluido == FALSO && strcasecmp(moeda->nomeMoeda, pesquisar->nomeMoeda) == 0) {
                return moeda;
            }
        }
    }
    return NULL;
}

/*
 * Cadastra um novo valor de moeda
 */
void cadastrarMoeda() {

    // Aloca espaco de memoria e inicia os valores
    tMoeda * moeda = (tMoeda *) malloc(sizeof(tMoeda));
    strcpy(moeda->nomeMoeda, "                        ");
    moeda->proximo = NULL;
    moeda->icExcluido = FALSO;

    // Imprime o cabecalho
    LIMPA_TELA;
    imprimeCabecalhoCambio();
    printf("Cadastrar moedas\n");

    //Le os dados da moeda
    printf("\nNome da moeda: ");
    LIMPA_BUFFER;
    scanf("%[a-z A-Z]s", moeda->nomeMoeda);
    printf("\nVariacao: ");
    scanf("%lf", &moeda->valorVariacao);
    printf("\nPercentual de Variacao: ");
    scanf("%lf", &moeda->pcVariacao);
    printf("\nValor de compra: ");
    scanf("%lf", &moeda->valorCompra);
    printf("\nValor de venda: ");
    scanf("%lf", &moeda->valorVenda);
    printf("\nValor minimo: ");
    scanf("%lf", &moeda->valorMinimo);
    printf("\nValor maximo: ");
    scanf("%lf", &moeda->valorMaximo);
    LIMPA_BUFFER;

    // Efetua as validacoes e salva
    if (validaMoeda(moeda)) {

        if (existeMoeda(moeda) != NULL) {
            // Moeda ja existe e nao pode ser cadastrada novamente
            printf("\nMoeda %s ja existe. Nao foi possivel incluir.", moeda->nomeMoeda);

        } else {
            // Gravar no arquivo
            fseek(arquivoCambio, 0L, SEEK_END); // Vai ao final do arquivo
            if (fwrite(moeda, sizeof(tMoeda), 1, arquivoCambio) != 1) {  // Grava e verifica se houve erro
                printf("\nErro ao incluir registro\n");
                exit(1);
            };

            // Mensagem de sucesso
            printf("\nMoeda '%s' incluida\n", moeda->nomeMoeda);
        }
    } else {
        printf("\nNao foi possivel incluir a moeda %s", moeda->nomeMoeda);
    }
    LIMPA_BUFFER;
    printf("\n\nPressione enter para retornar");
    getchar();
}

/*
 * Alterar valores de moeda
 */
void alterarMoeda() {

    // Aloca espaco de memoria
    tMoeda * moeda = (tMoeda *) malloc(sizeof(tMoeda));

    // Imprime o cabecalho
    LIMPA_TELA;
    imprimeCabecalhoCambio();
    printf("Alterar moeda\n");

    //Le os dados da moeda
    printf("\nNome da moeda: ");
    LIMPA_BUFFER;
    scanf("%[a-z A-Z]s", moeda->nomeMoeda);

    moeda = existeMoeda(moeda);
    if (moeda == NULL) {
        // Moeda ainda nao existe
        printf("\nMoeda nao encontrada. Nao foi possivel alterar.");
        LIMPA_BUFFER;
        printf("\n\nPressione enter para retornar");
        getchar();
        return;
    }

    printf("\nVariacao (%11.2lf): ", moeda->valorVariacao);
    scanf("%lf", &moeda->valorVariacao);
    printf("\nPercentual de Variacao (%11.4lf): ", moeda->pcVariacao);
    scanf("%lf", &moeda->pcVariacao);
    printf("\nValor de compra (%11.2lf): ", moeda->valorCompra);
    scanf("%lf", &moeda->valorCompra);
    printf("\nValor de venda (%11.2lf): ", moeda->valorVenda);
    scanf("%lf", &moeda->valorVenda);
    printf("\nValor minimo (%11.2lf): ", moeda->valorMinimo);
    scanf("%lf", &moeda->valorMinimo);
    printf("\nValor maximo (%11.2lf): ", moeda->valorMaximo);
    scanf("%lf", &moeda->valorMaximo);
    LIMPA_BUFFER;

    // Efetua as validacoes e salva
    if (validaMoeda(moeda)) {

        // Posiciona o ponteiro para gravar
        fseek(arquivoCambio, posicao - sizeof(tMoeda), SEEK_SET );
        // Gravar no arquivo
        if (fwrite(moeda, sizeof(tMoeda), 1, arquivoCambio) != 1) {  // Grava e verifica se houve erro
            printf("\nErro ao alterar registro\n");
            exit(1);
        };

        // Mensagem de sucesso
        printf("\nMoeda '%s' alterada", moeda->nomeMoeda);
    } else {
        printf("\nNao foi possivel alterar a moeda %s ", moeda->nomeMoeda);
    }
    LIMPA_BUFFER;
    printf("\n\nPressione enter para retornar");
    getchar();
}

/*
 * Excluir moeda
 */
void excluirMoeda() {

    // Aloca espaco de memoria
    tMoeda * moeda = (tMoeda *) malloc(sizeof(tMoeda));

    // Imprime o cabecalho
    LIMPA_TELA;
    imprimeCabecalhoCambio();
    printf("Excluir moeda\n");

    //Le os dados da moeda
    printf("\nNome da moeda: ");
    LIMPA_BUFFER;
    scanf("%[a-z A-Z]s", moeda->nomeMoeda);

    moeda = existeMoeda(moeda);
    if (moeda == NULL) {
        // Moeda ainda nao existe
        printf("\nMoeda nao encontrada. Nao foi possivel excluir.\n");
        printf("\n\nPressione enter para retornar");
        getchar();
        return;
    }

    // Solicita confirmacao
    printf("\nConfirma a exclusao? (1 para confirmar, qualquer outro para nao confirmar): ");
    int confirmar;
    LIMPA_BUFFER;
    scanf("%d", &confirmar);

    // Efetua as validacoes e salva
    if (confirmar == 1) {

        // Marca o registro como excluido
        moeda->icExcluido = VERDADEIRO;

        // Posiciona o ponteiro para gravar
        fseek(arquivoCambio, posicao - sizeof(tMoeda), SEEK_SET );

        // Gravar no arquivo
        if (fwrite(moeda, sizeof(tMoeda), 1, arquivoCambio) != 1) {  // Grava e verifica se houve erro
            printf("\nErro ao alterar registro\n");
            exit(1);
        };

        // Mensagem de sucesso
        printf("\nMoeda '%s' excluida\n", moeda->nomeMoeda);
        printf("\n\nPressione enter para retornar");
        LIMPA_BUFFER;
        getchar();
    }
}

/*
 * Carrega as mooedas cadastradas
 */
tMoeda * carregarMoedaOrdem(int ordem) {

    tMoeda * inicio = NULL;
    tMoeda * aux    = NULL;

    // Volta ao inicio do arquivo
    rewind(arquivoCambio);

    // Percorre o arquivo ate o final
    while(!feof(arquivoCambio)) {
        // Le um registro do arquivoCambio e armazena na variavel moeda
        tMoeda * moeda = (tMoeda *) malloc(sizeof(tMoeda));
        fread(moeda, sizeof(tMoeda), 1, arquivoCambio);
        if (!feof(arquivoCambio)) {
            if (moeda->icExcluido == FALSO) {
                aux = moedaAnterior(inicio, moeda, ordem);
                if (aux == NULL) {
                    moeda->proximo = inicio;
                    inicio = moeda;
                } else {
                    moeda->proximo = aux->proximo;
                    aux->proximo= moeda;
                }
            } else {
                free(moeda);
            }
        }
    }

    return inicio;
}

/*
 * Pesquisa a moeda no arquivo, pelo nome
 */
tMoeda * carregarMoedaNome(char nomePesquisa[]) {

    tMoeda * moeda = (tMoeda *) malloc(sizeof(tMoeda));

    // Volta ao inicio do arquivo
    rewind(arquivoCambio);

    // Percorre o arquivo ate o final ou localizar a moeda
    while(!feof(arquivoCambio)) {
        // Le um registro do arquivoCambio e armazena na variavel moeda
        fread(moeda, sizeof(tMoeda), 1, arquivoCambio);
        if (!feof(arquivoCambio)) {
            if (moeda->icExcluido == FALSO && strcasecmp(moeda->nomeMoeda, nomePesquisa) == 0) {
                moeda->proximo = NULL; // O proximo deve ser nulo porque recuperou apenas uma moeda
                return moeda;
            }
        }
    }
    return NULL;
}

/*
 * Exibe os dados das moedas das lista
 */
void exibirMoeda(tMoeda * moeda, char titulo[40]) {

    // Imprime o cabecalho
    LIMPA_TELA;
    imprimeCabecalhoCambio();
    printf("%s\n\n", titulo);
    printf("%-25s %11s %11s %11s %11s %11s %11s\n", "Moeda", "Variacao", "% Variacao", "Compra", "Venda", "Minimo", "Maximo");

    // Se nao houver moeda para exibir
    if (moeda == NULL) {
        printf("\nNenhuma moeda para exibir\n");
    }

    // Exibe as moedas
    while(moeda != NULL) {
        printf("%-25s %11.2lf %11.4lf %11.2lf %11.2lf %11.2lf %11.2lf\n",
            moeda->nomeMoeda,
            moeda->valorVariacao,
            moeda->pcVariacao,
            moeda->valorCompra,
            moeda->valorVenda,
            moeda->valorMinimo,
            moeda->valorMaximo);
        moeda = moeda->proximo;
    }

    // Aguarda comando do usuario para retornar
    printf("\n\nPressione enter para retornar");
    LIMPA_BUFFER;
    getchar();
    return;
}

/*
 * Lista as mooedas cadastradas
 */
void listarMoedaOrdem(int ordem) {

    // Carrega as moedas em memoria
    tMoeda * moeda = carregarMoedaOrdem(ordem);

    // Configura o titulo
    char titulo[40];
    if (ordem ==1) {
        strcpy(titulo, "Moedas em ordem alfabetica");
    } else {
        strcpy(titulo, "Moedas em ordem de variacao");
    }

    // Imprime os dados
    exibirMoeda(moeda, titulo);

    // Libera a memoria
    free(moeda);
    return;
}

/*
 * Controla as opcoes de listar moeda
 */
void listarMoeda() {
    int opcao;
    do {
        opcao = menuListarMoeda();
        switch (opcao){
            case '1':
                listarMoedaOrdem(1);  // Alfabetica
                break;
            case '2':
                listarMoedaOrdem(2);  // Variacao
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
}

/*
 * Pesquisar mooeda pelo nome
 */
void pesquisarMoeda() {

    char nomePesquisa[25];

    // Imprime o cabecalho
    LIMPA_TELA;
    imprimeCabecalhoCambio();
    printf("Pesquisar moeda\n");

    // Le o nome da moeda
    printf("\nNome da moeda: ");
    LIMPA_BUFFER;
    scanf("%[a-z A-Z]s", nomePesquisa);

    // Pesquisa a moeda pelo nome
    tMoeda * moeda = carregarMoedaNome(nomePesquisa);

    // Imprime os dados da moeda
    exibirMoeda(moeda, "Moeda pesquisada");

    // Libera a memoria
    free(moeda);
    return;
}

/*
 *  Tratamento das opcoes gerais de cambio
 */
void mainCambio() {
    int opcao;
    arquivoCambio = abrirLeituraGravacao("dadosCambio.txt");
    do {
        opcao = menuCambio();
        switch (opcao){
            case '1':
                cadastrarMoeda();
                break;
            case '2':
                pesquisarMoeda();
                break;
            case '3':
                listarMoeda();
                break;
            case '4':
                alterarMoeda();
                break;
           case '5':
                excluirMoeda();
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

    fecharArquivo(arquivoCambio);
    return;
}

