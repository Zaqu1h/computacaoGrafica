/**
 * @file chroma_key.c
 * @brief Implementação de um programa de Chroma Key para imagens PPM.
 *
 * Este arquivo contém a lógica para realizar a sobreposição de imagens
 * baseada em uma cor chave (Chroma Key). Ele lê duas imagens no formato PPM (P3),
 * uma de primeiro plano (foreground) com um fundo de cor sólida e outra de
 * fundo (background), e gera uma nova imagem combinada.
 *
 * A técnica funciona substituindo pixels na imagem de primeiro plano que são
 * "próximos" a uma cor chave pelos pixels correspondentes da imagem de fundo.
 *
 * @author Társis Barreto
 * @author Isaque Passos
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------

typedef struct Pixel
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
} tpPixel;

//-----------------------------------------------------------------------------

char infoB[4], infoF[4];
unsigned char maxValB, maxValF;
short int nLinB, nLinF, nColB, nColF;

FILE *arqFore, *arqBack, *arqSaida;

unsigned char chaveR, chaveG, chaveB;
int distancia, tolerancia;
short int provR, provG, provB, provTol;

tpPixel *back1D, *fore1D;
tpPixel **back2D, **fore2D;

//-----------------------------------------------------------------------------

void abrirArquivos(int argc, char *argv[]);
void lerCabecalhos(void);
void validarDados(void);
void alocarImagens(void);
void guardaImagens(void);
void criarImagem(void);
void liberaAlocacoes(void);

//-----------------------------------------------------------------------------

/**
 * @brief Abre os arquivos e processa os argumentos da linha de comando.
 */
void abrirArquivos(int argc, char *argv[])
{
    if (argc < 8){

        printf("Instr. de uso: <prog> <imgForeground> <imgBackground> <imgSaida> <chaveR> <chaveG> <chaveB> <tolerancia>\n\n");
        exit(0);
    }

    arqFore = fopen(argv[1], "r");
    arqBack = fopen(argv[2], "r");

    if (arqFore == NULL || arqBack == NULL){

        printf("Erro ao abrir algum dos arquivos\n");
        exit(1);
    }

    arqSaida = fopen(argv[3], "w");

    if (arqSaida == NULL){

        printf("Erro ao criar arquivo de saida\n");
        exit(1);
    }

    provR = atoi(argv[4]);
    provG = atoi(argv[5]);
    provB = atoi(argv[6]);
    provTol = atoi(argv[7]);
}

//-----------------------------------------------------------------------------

/**
 * @brief Lê os cabeçalhos das imagens PPM.
 */
void lerCabecalhos(void)
{
    fscanf(arqBack, "%s\n", infoB);
    fscanf(arqBack, "%hd %hd\n", &nColB, &nLinB);
    fscanf(arqBack, "%hhu\n", &maxValB);

    fscanf(arqFore, "%s\n", infoF);
    fscanf(arqFore, "%hd %hd\n", &nColF, &nLinF);
    fscanf(arqFore, "%hhu\n", &maxValF);
}

//-----------------------------------------------------------------------------

/**
 * @brief Valida todos os dados de entrada para garantir a consistência.
 */
void validarDados()
{
    if (provR > 255 || provR < 0 || provG > 255 || provG < 0 || provB > 255 || provB < 0){

        printf("Insira apenas valores entre 0 e 255 para RGB\n");
        exit(1);
    }

    chaveR = provR;
    chaveG = provG;
    chaveB = provB;

    if (provTol > 441) tolerancia = 441;
    else if (provTol < 0) tolerancia = 0;
    else tolerancia = provTol;

    if (strcmp(infoB, infoF) != 0){

        printf("Ambos arquivos devem ter o formato P3.\n");
        exit(1);
    }

    if (nColF > nColB || nLinF > nLinB){

        printf("O arquivo foreground precisa ser menor que o background.\n");
        exit(1);
    }

    if (maxValB != maxValF){

        printf("Os arquivos tem maxima intensidade diferente.\n");
        exit(1);
    }
}

//-----------------------------------------------------------------------------

/**
 * @brief Aloca a memória necessária para armazenar as duas imagens.
 */
void alocarImagens()
{
    short int i = 0;

    back1D = (tpPixel *)malloc(sizeof(tpPixel) * nLinB * nColB);
    fore1D = (tpPixel *)malloc(sizeof(tpPixel) * nLinF * nColF);

    if (back1D == NULL || fore1D == NULL){

        printf("Erro ao alocar.\n");
        exit(1);
    }

    back2D = (tpPixel **)malloc(sizeof(tpPixel *) * nLinB);
    fore2D = (tpPixel **)malloc(sizeof(tpPixel *) * nLinF);

    if (back2D == NULL || fore2D == NULL){

        printf("Erro ao alocar.\n");
        free(back1D);
        free(fore1D);
        back1D = fore1D = NULL;
        exit(1);
    }

    while (i < nLinB){

        back2D[i] = back1D + i * nColB;
        i++;
    }

    i = 0;

    while (i < nLinF){

        fore2D[i] = fore1D + i * nColF;
        i++;
    }
}

//-----------------------------------------------------------------------------

/**
 * @brief Lê os dados de pixel dos arquivos e os guarda na memória alocada.
 */
void guardaImagens()
{
    for (short int i = 0; i < nLinB; i++){
        for (short int j = 0; j < nColB; j++){

            fscanf(arqBack, "%hhu ", &back2D[i][j].R);
            fscanf(arqBack, "%hhu ", &back2D[i][j].G);
            fscanf(arqBack, "%hhu ", &back2D[i][j].B);
        }
    }

    for (short int i = 0; i < nLinF; i++){
        for (short int j = 0; j < nColF; j++){

            fscanf(arqFore, "%hhu ", &fore2D[i][j].R);
            fscanf(arqFore, "%hhu ", &fore2D[i][j].G);
            fscanf(arqFore, "%hhu ", &fore2D[i][j].B);
        }
    }

    if (fclose(arqBack) != 0 || fclose(arqFore) != 0){

        printf("Erro ao guardar imagens.\n");
        exit(1);
    }
}

//-----------------------------------------------------------------------------

/**
 * @brief Cria a imagem final aplicando o efeito Chroma Key.
 */
void criarImagem()
{
    unsigned char atualR, atualG, atualB;
    tolerancia = tolerancia * tolerancia;

    fprintf(arqSaida, "P3\n");
    fprintf(arqSaida, "%hd %hd\n", nColB, nLinB);
    fprintf(arqSaida, "%hhu\n", maxValB);

    for (short int i = 0; i < nLinB; i++){
        for (short int j = 0; j < nColB; j++){

            if (i < nLinF && j < nColF){

                atualR = fore2D[i][j].R;
                atualG = fore2D[i][j].G;
                atualB = fore2D[i][j].B;

                distancia = (atualR - chaveR) * (atualR - chaveR)
                          + (atualG - chaveG) * (atualG - chaveG)
                          + (atualB - chaveB) * (atualB - chaveB);

                if (distancia < tolerancia){

                    fprintf(arqSaida, "%hhu %hhu %hhu\n", back2D[i][j].R, back2D[i][j].G, back2D[i][j].B);
                }

                else if (distancia > tolerancia){

                    fprintf(arqSaida, "%hhu %hhu %hhu\n", fore2D[i][j].R, fore2D[i][j].G, fore2D[i][j].B);
                }

                else{

                    fprintf(arqSaida, "%hhu %hhu %hhu\n", ((back2D[i][j].R + fore2D[i][j].R) / 2),
                                                          ((back2D[i][j].G + fore2D[i][j].G) / 2),
                                                          ((back2D[i][j].B + fore2D[i][j].B) / 2));
                }
            }

            else{

                fprintf(arqSaida, "%hhu %hhu %hhu\n", back2D[i][j].R, back2D[i][j].G, back2D[i][j].B);
            }
        } // END_J
    } // END_I

    if (fclose(arqSaida) != 0){

        printf("Erro ao fechar arquivo de saida.\n");
        exit(0);
    }
}

//-----------------------------------------------------------------------------

/**
 * @brief Libera toda a memória que foi alocada dinamicamente com `malloc`.
 */
void liberaAlocacoes()
{
    free(back1D);
    free(fore1D);
    back1D = fore1D = NULL;

    free(back2D);
    free(fore2D);
    back2D = fore2D = NULL;
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    abrirArquivos(argc, argv);
    lerCabecalhos();
    validarDados();
    alocarImagens();
    guardaImagens();
    criarImagem();
    liberaAlocacoes();

    return 0;
}
