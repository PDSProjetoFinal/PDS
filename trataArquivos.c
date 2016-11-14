#include <stdio.h>
#include <stdlib.h>
/*
*  Abre o arquivo para leitura e gravacao (r+)
*  Em modo binario (b)
*  Se o arquivo nao existir, sera criado
*/
FILE * abrirLeituraGravacao(char nomeArquivo[80]) {
    FILE * arq = NULL;
    if((arq = fopen(nomeArquivo, "rb+")) == NULL) {
        printf("\n O arquivo '%s' nao pode ser aberto!!\n", nomeArquivo);
        getch();
        exit(1);
    }
    return arq;
}

/*
* Fecha o arquivo que estiver aberto
* Esta funcao deve ser chamada para evitar que fique aberta a conexao entre o programa e o arquivo em disco
*/
void fecharArquivo(FILE * arq) {
    fclose(arq);
}

