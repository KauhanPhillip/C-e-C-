#include <stdio.h>
#include <string.h>

struct mouse {
    char marca[20];
    float preco;
    char tipo_conexao[15];
    char status;
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void geraarqtxt(FILE *);
void excluir(FILE *);

int main() {
    int op;
    FILE *arq;

    if ((arq = fopen("mouses.dat", "rb+")) == NULL) {
        if ((arq = fopen("mouses.dat", "wb+")) == NULL) {
            printf("Falha ao abrir o arquivo!\n");
            return 1;
        }
    }

    do {
        printf("\n======= LOJA DE MÓUSES ======= \n");
        printf("1. Cadastrar\n");
        printf("2. Consultar por marca\n");
        printf("3. Gerar arquivo\n");
        printf("4. Excluir registro\n");
        printf("5. Sair\n");
        printf("=========== Mouses:%d=\n", tamanho(arq));
        printf("Opcao:");
        scanf("%d", &op);

        switch (op) {
            case 1:
                cadastrar(arq);
                break;
            case 2:
                consultar(arq);
                break;
            case 3:
                geraarqtxt(arq);
                break;
            case 4:
                excluir(arq);
                break;
        }
    } while (op != 5);

    fclose(arq);
    return 0;
}

void cadastrar(FILE *arq) {
    struct mouse novoMouse;

    fflush(stdin);
    printf("Cadastrando novo mouse:\n");
    printf("Marca...........:");
    scanf("%s", novoMouse.marca);
    printf("Preço...........:");
    scanf("%f", &novoMouse.preco);
    printf("Tipo de Conexão.:");
    scanf("%s", novoMouse.tipo_conexao);
    novoMouse.status = ' ';

    printf("\nGravando...\n\n");
    fseek(arq, 0, SEEK_END);
    fwrite(&novoMouse, sizeof(struct mouse), 1, arq);
}

void consultar(FILE *arq) {
    struct mouse consultaMouse;
    char marca[20];

    printf("\nConsulta por marca\n");
    printf("Informe a Marca...:");
    scanf("%s", marca);

    fseek(arq, 0, SEEK_SET);
    while (fread(&consultaMouse, sizeof(struct mouse), 1, arq) == 1) {
        if (strcmp(consultaMouse.marca, marca) == 0) {
            printf("\nMarca........:%s", consultaMouse.marca);
            printf("\nPreço........:%.2f", consultaMouse.preco);
            printf("\nTipo de Conexão:%s\n", consultaMouse.tipo_conexao);
            return;
        }
    }

    printf("\nMouse não encontrado!\n");
}

void geraarqtxt(FILE *arq) {
    char nomearq[20];
    printf("Nome do arquivo texto:");
    scanf("%s", nomearq);
    strcat(nomearq, ".txt");
    FILE *arqtxt = fopen(nomearq, "w");

    if (!arqtxt) {
        printf("Não foi possível criar esse arquivo!\n");
        return;
    }

    fprintf(arqtxt, "Marca       Preço    Tipo de Conexão    Status\n");
    fprintf(arqtxt, "=============================================\n");

    fseek(arq, 0, SEEK_SET);
    struct mouse consultaMouse;

    while (fread(&consultaMouse, sizeof(struct mouse), 1, arq) == 1) {
        fprintf(arqtxt, "%-12s%-8.2f%-20s%c\n", consultaMouse.marca,
                consultaMouse.preco, consultaMouse.tipo_conexao, consultaMouse.status);
    }

    fprintf(arqtxt, "=============================================\n");
    fclose(arqtxt);
}

void excluir(FILE *arq) {
    struct mouse consultaMouse;
    char marca[20];

    printf("\nInforme a marca do mouse para excluir\n");
    scanf("%s", marca);

    fseek(arq, 0, SEEK_SET);
    while (fread(&consultaMouse, sizeof(struct mouse), 1, arq) == 1) {
        if (strcmp(consultaMouse.marca, marca) == 0) {
            printf("\nMarca........:%s", consultaMouse.marca);
            printf("\nPreço........:%.2f", consultaMouse.preco);
            printf("\nTipo de Conexão:%s\n", consultaMouse.tipo_conexao);
            printf("\nExcluindo...\n\n");
            struct mouse marcador = {0};
            fseek(arq, -sizeof(struct mouse), SEEK_CUR);
            fwrite(&marcador, sizeof(struct mouse), 1, arq);
            return;
        }
    }

    printf("Mouse não encontrado!\n");
}

int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(struct mouse);
}