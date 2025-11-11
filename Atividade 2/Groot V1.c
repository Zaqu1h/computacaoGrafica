/**
 * @file Groot.c
 * @brief Implementação de um programa que desenha o Groot (personagem da Marvel
 * numa janela através, principalmente da biblioteca GLUT (OpenGL Utility Toolkit).
 *
 * Este arquivo contém a lógica para realizar a síntese de um personagem definido
 * por diversos pontos em um espaço bidimensional, onde esses pontos são usados de
 * referência para delimitar partes do corpo do personagem (o Groot, especificamente)
 * ou para construir outras características deste por meio da GLUT.
 *
 * @author Isaque Passos
 */

// Inclusão de Bibliotecas Essenciais
// ------------------------------------
#include <GL/glut.h> /**< Biblioteca principal. Fornece inúmeras funções para se trabalhar com OpenGL.*/
#include <math.h> /**< Biblioteca matemática, Útil para manipular angulações, proporções e etc.*/
// ------------------------------------

// Definição de macros de cores utilizadas no código
// ------------------------------------
#define COR_BRANCA (tpCor){200, 200, 200}
#define COR_PRETA (tpCor){60, 60, 60}
#define COR_VERMELHA (tpCor){188, 113, 94}
#define COR_MARROM_ESCURO (tpCor){140, 110, 80}
#define COR_MARROM_MEDIO (tpCor){170, 135, 115}
#define COR_MARROM (tpCor){177, 155, 132}
#define COR_BEGE (tpCor){220, 195, 185}
#define COR_VERDE (tpCor){80, 120, 55}
#define COR_COR_VERDE_CLARO (tpCor){132, 172, 102}
#define COR_CONTORNO (tpCor){110, 90, 60}
// ------------------------------------

// Definição de macros para conversão de graus e radianos
// ------------------------------------
#define _G * (180.0 / M_PI)
#define _R * (M_PI / 180.0)
// ------------------------------------

// Definição de Estruturas e Tipos
// ------------------------------------

/**
 * @struct Ponto2D
 * @brief Uma representação de um ponto num espaço bidimensional.
 *
 * Contém as variáveis x e y, que, respectivamente, guarda a posição da abcissa
 * das ordenadas em um plano. Float pois estamos tratando de números reais.
 */
typedef struct Ponto2D{

    float x;
    float y;
} tpPonto2D;

// ------------------------------------

/**
 * @struct Cor
 * @brief Uma estrutura que guarda componentes de cor RGB.
 *
 * Cada componente (r, g, b) é um `unsigned char`, logo
 * armazena um valor inteiro sem sinal de 0 a 255.
 */
typedef struct Cor {

    unsigned char r;/**< Componente de cor Vermelha (Red). */
    unsigned char g; /**< Componente de cor COR_VERDE (Green). */
    unsigned char b; /**< Componente de cor Azul (Blue). */
} tpCor;

//-----------------------------------------------------------------------------
// Protótipos das Funções
// ------------------------
void definirCor(unsigned char r, unsigned char g, unsigned char b);
void desenharColorP(tpPonto2D pontos[], int numPontos, tpCor cor);
void desenharPG(tpPonto2D pontos[], int numPontos, tpCor cor1, tpCor cor2, float inicioGradiente, float fimGradiente);
void desenharOutline(tpPonto2D pontos[], int numPontos, tpCor cor);
void desenharElipse(float centroX, float centroY, float raioX, float raioY, int segmentos, tpCor cor, float anguloInclinacao);
void desenharLinhaReta(float x1, float y1, float x2, float y2, tpCor cor, float espessura);
void desenharLinhaCurva(float centroX, float centroY, float largura, float altura, int segmentos, tpCor cor);
void desenharFeaturesRosto(void);
void desenharGroot(void);
void display(void);
void remodelar(int largura, int altura);

//-----------------------------------------------------------------------------
// Variáveis Globais
// -----------------
/**
* Abaixo, temos uma sequencia de vetores de pontos2D seguidos de um int, respectivamente,
* estes guardam os pontos de determinadas partes do corpo do Groot e o número de pontos
* daquela parte. */
tpPonto2D pontosCabeca[] = {

    {3.0f, 8.0f},  // J
    {2.5f, 4.5f},  // H
    {2.0f, 3.5f},  // B
    {1.5f, 3.0f},  // D
    {0.5f, 2.7f},  // F
    {-0.5f, 2.7f},  // E
    {-1.5f, 3.0f},  // C
    {-2.0f, 3.5f},  // A
    {-2.5f, 4.5f},  // G
    {-3.0f, 8.0f},  // I
    {-3.25f, 8.55f},  // R
    {-3.28f, 9.07f},  // T
    {-3.18f, 9.36f},  // U
    {-2.97f, 9.58f},  // V
    {-2.726f, 9.66f},  // W
    {-2.5f, 9.6f},  // Z
    {-2.25f, 9.20f},  // A_1
    {-2.25f, 9.7f},  // B_1
    {-2.2f, 9.88f},  // C_1
    {-2.0f, 10.0f},  // D_1
    {-1.8f, 10.0f},  // E_1
    {-1.66f, 9.98f},  // F_1
    {-1.50f, 9.84f},  // G_1
    {-1.14, 10.275f},  // I_1
    {-0.99f, 10.37f},  // J_1
    {-0.5f, 10.5f},  // N_1
    {-0.32f, 10.55f},  // O_1
    {-0.118f, 10.56f},  // P_1
    {0.0f, 10.5f},  // Q_1
    {0.0f, 10.0f},  // D_2
    {0.5f, 10.5f},  // E_2
    {1.0f, 10.5f},  // F_2
    {1.238f, 10.236f},  // G_2
    {1.33f, 10.0f},  // H_2
    {1.345f, 9.50f},  // I_2
    {1.5f, 10.0f},  // J_2
    {1.648f, 10.11f},  // N_2
    {1.837f, 10.1f},  // O_2
    {2.0f, 10.0f},  // P_2
    {2.1f, 9.9f},  // Q_2
    {2.11f, 9.395f},  // R_2
    {2.46f, 9.9f},  // A_3
    {2.70f, 9.928f},  // Z_2
    {2.918f, 9.75f},  // W_2
    {3.0f, 9.29f},  // V_2
    {3.158f, 9.20f},  // U_2
    {3.323f, 9.06f},  // T_2
    {3.33f, 8.82f}  // S_2
};
int numPontosCabeca = 48;
tpCor corCabeca = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosTronco[] = {

    {0.0f, 3.5f},  // L_2 (Reta)
    {1.0f, 3.5f},  // K_2 (Reta)
    {2.75f, 2.25f},  // A (Reta)
    {2.0f, 1.0f},  // V_3 (Reta)
    {2.0f, -4.0f},  // K_3 (Reta)
    {2.0f, -4.3f},  // B (Reta)
    {0.5f, -4.3f},  // C (Reta)
    {0.15f, -4.3f},  // O_3 (Reta)
    {0.0f, -4.3f},  // P_3 (Reta)
    {-0.15f, -4.3f},  // PO_3 (Reta)
    {-0.5f, -4.3f},  // PC (Reta)
    {-2.0f, -4.3f},  // PB (Reta)
    {-2.0f, -4.0f},  // PK_3 (Reta)
    {-2.0f, 1.0f},  // PV_3 (Reta)
    {-2.75f, 2.25f},  // PA (Reta)
    {-1.0f, 3.5f},  // PK_2 (Reta)
    {0.0f, 3.5f}  // PL_2 (Reta)
};
int numPontosTronco = 17;
tpCor corTronco = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosOmbroEsquerdo[] = {

    {-2.2f, 2.5f},  // M_2 (Reta)
    {-2.75f, 2.325f},  // B_3 (Reta)
    {-3.1f, 2.0f},  // C_3 (Reta)
    {-3.25f, 1.60f},  // D_3 (Reta)
    {-3.25f, 1.25f},  // A (Reta)
    {-2.0f, 1.25f},  // B (Reta)
};
int numPontosOmbroEsquerdo = 6;
tpCor corOmbroEsquerdo = COR_MARROM_ESCURO;

//-----------------------------------------------------------------------------

tpPonto2D pontosOmbroDireito[] = {

    {2.2f, 2.5f},  // M_2 (Reta)
    {2.75f, 2.325f},  // B_3 (Reta)
    {3.1f, 2.0f},  // C_3 (Reta)
    {3.25f, 1.60f},  // D_3 (Reta)
    {3.25f, 1.25f},  // A (Reta)
    {2.0f, 1.25f},  // B (Reta)
};
int numPontosOmbroDireito = 6;
tpCor corOmbroDireito = COR_MARROM_ESCURO;

//-----------------------------------------------------------------------------

tpPonto2D pontosBracoDireito[] = {

    {3.25f, 1.35f},  // E_3 (Reta)
    {3.45f, -1.45f},  // A (Reta)
    {2.2f, -1.25f},  // B (Reta)
    {2.0f, 1.35f},  // C (Reta)
};
int numPontosBracoDireito = 4;
tpCor corBracoDireito = COR_MARROM_ESCURO;

//-----------------------------------------------------------------------------

tpPonto2D pontosBracoEsquerdo[] = {

    {-3.25f, 1.35f},  // E_3 (Reta)
    {-3.45f, -1.45f},  // A (Reta)
    {-2.2f, -1.25f},  // B (Reta)
    {-2.0f, 1.35f},  // C (Reta)
};
int numPontosBracoEsquerdo = 4;
tpCor corBracoEsquerdo = COR_MARROM_ESCURO;

//-----------------------------------------------------------------------------

tpPonto2D pontosAntebracoDireito[] = {

    {3.45f, -1.25f},  // F_3 (Reta)
    {3.25f, -3.85f},  // G_3 (Reta)
    {2.8f, -4.0f},  // A_4 (Reta)
    {2.2f, -1.25f},  // B_4 (Reta)
};
int numPontosAntebracoDireito = 4;
tpCor corAntebracoDireito = COR_BEGE;

//-----------------------------------------------------------------------------

tpPonto2D pontosAntebracoEsquerdo[] = {

    {-3.45f, -1.25f},  // F_3 (Reta)
    {-3.25f, -3.85f},  // G_3 (Reta)
    {-2.8f, -4.0f},  // A_4 (Reta)
    {-2.2f, -1.25f},  // B_4 (Reta)
};
int numPontosAntebracoEsquerdo = 4;
tpCor corAntebracoEsquerdo = COR_BEGE;

//-----------------------------------------------------------------------------

tpPonto2D pontosMaoDireita[] = {

    {3.25f, -3.7f},  // G_3 (Reta)
    {3.6, -5.0f},  // H_3 (Reta)
    {3.2f, -5.8f},  // I_3 (Reta)
    {3.0f, -5.7f},  // J_3 (Reta)
    {3.15f, -5.0f},  // W_3 (Reta)
    {2.85f, -4.56f},  // C_4 (Reta)
    {2.8f, -5.0f},  // D_4 (Reta)
    {2.60f, -4.80f},  // Z_3 (Reta)
    {2.75f, -3.80f},  // A_4 (Reta)
};
int numPontosMaoDireita = 9;
tpCor corMaoDireita = COR_BEGE;

//-----------------------------------------------------------------------------

tpPonto2D pontosMaoEsquerda[] = {

    {-3.25f, -3.7f},  // G_3 (Reta)
    {-3.6, -5.0f},  // H_3 (Reta)
    {-3.2f, -5.8f},  // I_3 (Reta)
    {-3.0f, -5.7f},  // J_3 (Reta)
    {-3.15f, -5.0f},  // W_3 (Reta)
    {-2.85f, -4.56f},  // C_4 (Reta)
    {-2.8f, -5.0f},  // D_4 (Reta)
    {-2.60f, -4.80f},  // Z_3 (Reta)
    {-2.75f, -3.80f},  // A_4 (Reta)
};
int numPontosMaoEsquerda = 9;
tpCor corMaoEsquerda = COR_BEGE;

//-----------------------------------------------------------------------------

tpPonto2D pontosCoxaDireita[] = {
    {0.15f, -4.3f},  // O_3 (Reta)
    {0.4f, -4.6f},  // N_3 (Reta)
    {0.4f, -7.0f},  // D (Reta)
    {2.0f, -7.0f},  // A (Reta)
    {2.0f, -4.0f},  // K_3 (Reta)
    {1.4f, -3.4f},  // B (Reta)
    {0.65f, -3.4f},  // C (Reta, animar aqui)
};
int numPontosCoxaDireita = 7;
tpCor corCoxaDireita = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosCoxaEsquerda[] = {
    {-0.15f, -4.3f},  // O_3 (Reta)
    {-0.4f, -4.6f},  // N_3 (Reta)
    {-0.4f, -7.0f},  // D (Reta)
    {-2.0f, -7.0f},  // A (Reta)
    {-2.0f, -4.0f},  // K_3 (Reta)
    {-1.4f, -3.4f},  // B (Reta)
    {-0.65f, -3.4f},  // C (Reta)
};
int numPontosCoxaEsquerda = 7;
tpCor corCoxaEsquerda = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosPantDireita[] = {

    {2.0f, -5.0f},  // A (Reta)
    {1.2, -5.0f},
    {0.4f, -5.0f},  // B (Reta)
    {0.4f, -9.5f},  // D (Reta)
    {2.0f, -9.5f},  // C (Reta)
};
int numPontosPantDireita = 5;
tpCor corPantDireita = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosPantEsquerda[] = {

    {-2.0f, -5.0f},  // A (Reta)
    {-1.2, -5.0f},
    {-0.4f, -5.0f},  // B (Reta)
    {-0.4f, -9.5f},  // D (Reta)
    {-2.0f, -9.5f},  // C (Reta)
};
int numPontosPantEsquerda = 5;
tpCor corPantEsquerda = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosPataDireita[] = {

    {2.0f, -9.35f},  // L_3 (Reta)
    {0.4f, -9.1f},  // R_3 (Reta)
    {0.25f, -10.0f},  // T_3 (Reta)
    {2.3f, -10.0f},  // M_3 (Reta)
};
int numPontosPataDireita = 4;
tpCor corPataDireita = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosPataEsquerda[] = {

    {-2.0f, -9.35f},  // L_3 (Reta)
    {-0.4f, -9.1f},  // R_3 (Reta)
    {-0.25f, -10.0f},  // T_3 (Reta)
    {-2.3f, -10.0f},  // M_3 (Reta)
};
int numPontosPataEsquerda = 4;
tpCor corPataEsquerda = COR_MARROM;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * @brief Define a cor atual para desenho no OpenGL.
 * @param r Componente COR_VERMELHA (0-255).
 * @param g Componente COR_VERDE (0-255).
 * @param b Componente azul (0-255).
 */
void definirCor(unsigned char r, unsigned char g, unsigned char b){

    glColor3ub(r, g, b);
}

//-----------------------------------------------------------------------------
/**
 * @brief Desenha um polígono preenchido com uma cor sólida.
 * @param pontos Array de pontos que definem o polígono.
 * @param numPontos Número de pontos no array.
 * @param cor Cor do polígono.
 */
void desenharColorP(tpPonto2D pontos[], int numPontos, tpCor cor){

    definirCor(cor.r, cor.g, cor.b);

    glBegin(GL_POLYGON);

    for (int i = 0; i < numPontos; i++){

        glVertex2f(pontos[i].x, pontos[i].y);
    }
    glEnd();
}

//-----------------------------------------------------------------------------
/**
 * @brief Desenha um polígono com gradiente vertical entre duas cores.
 * @param pontos Array de pontos que definem o polígono.
 * @param numPontos Número de pontos no array.
 * @param cor1 Cor inicial do gradiente (parte inferior).
 * @param cor2 Cor final do gradiente (parte superior).
 * @param inicioGradiente Posição relativa (0.0 á 1.0) onde o gradiente começa.
 * @param fimGradiente Posição relativa (0.0 á 1.0) onde o gradiente termina.
 */
void desenharPG(tpPonto2D pontos[], int numPontos, tpCor cor1, tpCor cor2, float inicioGradiente, float fimGradiente) {

    float yMin = pontos[0].y, yMax = pontos[0].y;

    for (int i = 1; i < numPontos; i++) {

        if (pontos[i].y < yMin) yMin = pontos[i].y;
        if (pontos[i].y > yMax) yMax = pontos[i].y;
    }

    glBegin(GL_POLYGON);

    for (int i = 0; i < numPontos; i++) {

        float posicaoRelativa = (pontos[i].y - yMin) / (yMax - yMin);
        float fator;

        if (posicaoRelativa < inicioGradiente) {

            fator = 0.0f;
        } else if (posicaoRelativa > fimGradiente) {

            fator = 1.0f;
        } else {

            fator = (posicaoRelativa - inicioGradiente) / (fimGradiente - inicioGradiente);
        }

        unsigned char r = cor1.r + fator * (cor2.r - cor1.r);
        unsigned char g = cor1.g + fator * (cor2.g - cor1.g);
        unsigned char b = cor1.b + fator * (cor2.b - cor1.b);

        glColor3ub(r, g, b);
        glVertex2f(pontos[i].x, pontos[i].y);
    }

    glEnd();
}
//-----------------------------------------------------------------------------
/**
 * @brief Desenha o contorno de um polígono.
 * @param pontos Array de pontos que definem o polígono.
 * @param numPontos Número de pontos no array.
 * @param cor Cor do contorno.
 */
void desenharOutline(tpPonto2D pontos[], int numPontos, tpCor cor){

    definirCor(cor.r, cor.g, cor.b);


    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numPontos; i++){

        glVertex2f(pontos[i].x, pontos[i].y);
    }
    glEnd();
}

//-----------------------------------------------------------------------------
/**
 * @brief Desenha uma elipse preenchida.
 * @param centroX Coordenada X do centro da elipse.
 * @param centroY Coordenada Y do centro da elipse.
 * @param raioX Raio no eixo X da elipse.
 * @param raioY Raio no eixo Y da elipse.
 * @param segmentos Número de segmentos para aproximar a elipse.
 * @param cor Cor da elipse.
 * @param anguloInclinacao Ângulo de inclinação da elipse em graus.
 */
void desenharElipse(float centroX, float centroY, float raioX, float raioY, int segmentos, tpCor cor, float anguloInclinacao){

    anguloInclinacao = anguloInclinacao _R;

    definirCor(cor.r, cor.g, cor.b);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centroX, centroY);

    for (int i = 0; i <= segmentos; i++){

        float angulo = 2.0f * M_PI * i / segmentos;
        float x = cos(angulo) * raioX;
        float y = sin(angulo) * raioY;

        float xRot = x * cos(anguloInclinacao) - y * sin(anguloInclinacao);
        float yRot = x * sin(anguloInclinacao) + y * cos(anguloInclinacao);

        glVertex2f(centroX + xRot, centroY + yRot);
    }
    glEnd();
}

//-----------------------------------------------------------------------------
/**
 * @brief Desenha uma linha reta entre dois pontos.
 * @param x1 Coordenada X do ponto inicial.
 * @param y1 Coordenada Y do ponto inicial.
 * @param x2 Coordenada X do ponto final.
 * @param y2 Coordenada Y do ponto final.
 * @param cor Cor da linha.
 * @param espessura Espessura da linha em pixels.
 */
void desenharLinhaReta(float x1, float y1, float x2, float y2, tpCor cor, float espessura) {

    definirCor(cor.r, cor.g, cor.b);
    glLineWidth(espessura);

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    glLineWidth(1.0f);
}

//-----------------------------------------------------------------------------
/**
 * @brief Desenha uma linha curva (arco) usando função trigonométrica.
 * @param centroX Coordenada X do centro da curva.
 * @param centroY Coordenada Y do centro da curva.
 * @param largura Largura total da curva.
 * @param altura Altura total da curva.
 * @param segmentos Número de segmentos para suavizar a curva.
 * @param cor Cor da linha curva.
 */
void desenharLinhaCurva(float centroX, float centroY, float largura, float altura, int segmentos, tpCor cor){

    definirCor(cor.r, cor.g, cor.b);
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= segmentos; i++){

        float angulo = M_PI + (M_PI * i / segmentos);
        float x = centroX + cos(angulo) * largura;
        float y = centroY + sin(angulo) * altura;

        glVertex2f(x, y);
    }

    glEnd();
}

//-----------------------------------------------------------------------------
/**
 * @brief Desenha os elementos faciais do Groot.
 */
void desenharFeaturesRosto(void){

    // SOMBRA DOS OLHOS -----------------------------------------------------------------------------
    desenharElipse(1.45f, 5.8f, 0.6f, 0.8f, 128, COR_MARROM_MEDIO, 0);
    desenharElipse(-1.45f, 5.8f, 0.6f, 0.8f, 128, COR_MARROM_MEDIO, 0);

    // BOCHECHAS -----------------------------------------------------------------------------
    desenharElipse(-1.3f, 5.0f, 0.4f, 0.25f, 120, COR_VERMELHA, 0);
    desenharElipse(1.3f, 5.0f, 0.4f, 0.25f, 120, COR_VERMELHA, 0);

    // OLHOS -----------------------------------------------------------------------------
    desenharElipse(-1.3f, 5.7f, 0.3f, 0.6f, 120, COR_PRETA, 0);
    desenharElipse(1.3f, 5.7f, 0.3f, 0.6f, 120, COR_PRETA, 0);

    // OLHOS (BRILHO) -----------------------------------------------------------------------------
    desenharElipse(-1.3f, 5.95f, 0.20f, 0.35f, 120, COR_BRANCA, 0);
    desenharElipse(1.3f, 5.95f, 0.20f, 0.35f, 120, COR_BRANCA, 0);

    // FOLHAS -----------------------------------------------------------------------------
    desenharElipse(-1.7f, 10.25f, 0.4f, 0.2f, 120, COR_VERDE, -45);
    desenharElipse(-1.30f, 10.25f, 0.35f, 0.15f, 120, COR_COR_VERDE_CLARO, 45);

    // BOCA -----------------------------------------------------------------------------
    desenharLinhaCurva(0.0f, 4.20f, 0.5f, 0.2f, 24, COR_CONTORNO);

    // DETALHES (TOPO DA CABEÇA) -----------------------------------------------------------------------------
    desenharLinhaReta(1.35f, 9.5f, 0.0f, 10.5f, COR_CONTORNO, 2);
    desenharLinhaReta(1.5f, 10.0f, 1.35f, 9.5f, COR_CONTORNO, 2);
    desenharLinhaReta(-1.15f, 10.15f, -1.5f, 9.5f, COR_CONTORNO, 1);
    desenharLinhaReta(-2.25f, 9.2f, -1.5f, 9.5f, COR_CONTORNO, 2);
    desenharLinhaReta(-2.25f, 9.2f, -2.27f, 9.6f, COR_CONTORNO, 1);
    desenharLinhaReta(-2.25f, 9.2f, -2.725f, 9.66f, COR_CONTORNO, 2);
    desenharLinhaReta(-3.25f, 8.55f, -2.8f, 7.6f, COR_CONTORNO, 1);
    desenharLinhaReta(0.0f, 7.5f, 0.0f, 9.0f, COR_CONTORNO, 1);
    desenharLinhaReta(0.0f, 10.5f, 0.0f, 9.5f, COR_CONTORNO, 1);
    desenharLinhaReta(2.25f, 9.65f, 2.25f, 9.1f, COR_CONTORNO, 1);
    desenharLinhaReta(2.0f, 8.8f, 2.25f, 9.1f, COR_CONTORNO, 1);
    desenharLinhaReta(2.0f, 8.8f, 2.0f, 8.5f, COR_CONTORNO, 1);
    desenharLinhaReta(-2.0f, 9.3f, -2.0f, 8.5f, COR_CONTORNO, 1);
}

//-----------------------------------------------------------------------------
/**
 * @brief Desenha o Groot completo com todas as suas partes.
 */
void desenharGroot(void){

    // PATAS ----------------------------------------------------------------------------
    desenharPG(pontosPataEsquerda, numPontosPataEsquerda, COR_BEGE, corPataEsquerda, 0.1f, 0.40f);
    desenharPG(pontosPataDireita, numPontosPataDireita, COR_BEGE, corPataEsquerda, 0.1f, 0.40f);

    // PANTURRILHAS -----------------------------------------------------------------------------
    desenharColorP(pontosPantEsquerda, numPontosPantEsquerda, corPantEsquerda);
    desenharColorP(pontosPantDireita, numPontosPantDireita, corPantDireita);

    // TRONCO -----------------------------------------------------------------------------
    desenharPG(pontosTronco, numPontosTronco, corTronco, COR_MARROM_ESCURO, 0.5f, 0.7f);

    // COXAS ----------------------------------------------------------------------------
    desenharPG(pontosCoxaEsquerda, numPontosCoxaEsquerda, corCoxaEsquerda, COR_MARROM_ESCURO, 0.5f, 1.0f);
    desenharPG(pontosCoxaDireita, numPontosCoxaDireita, corCoxaDireita, COR_MARROM_ESCURO, 0.5f, 1.0f);

    // OMBROS ----------------------------------------------------------------------------
    desenharColorP(pontosOmbroEsquerdo, numPontosOmbroEsquerdo, corOmbroEsquerdo);
    desenharColorP(pontosOmbroDireito, numPontosOmbroDireito, corOmbroDireito);

    // CABEÇA ----------------------------------------------------------------------------
    desenharPG(pontosCabeca, numPontosCabeca, corCabeca, COR_VERDE, 0.70f, 0.9f);

    // BRAÇOS ----------------------------------------------------------------------------
    desenharPG(pontosBracoEsquerdo, numPontosBracoEsquerdo, COR_BEGE, corBracoEsquerdo, 0.1f, 1.0f);
    desenharPG(pontosBracoDireito, numPontosBracoDireito, COR_BEGE, corBracoDireito, 0.1f, 1.0f);

    // ANTEBRAÇOS ----------------------------------------------------------------------------
    desenharColorP(pontosAntebracoEsquerdo, numPontosAntebracoEsquerdo, corAntebracoEsquerdo);
    desenharColorP(pontosAntebracoDireito, numPontosAntebracoDireito, corAntebracoDireito);

    // MÃOS ----------------------------------------------------------------------------
    desenharColorP(pontosMaoEsquerda, numPontosMaoEsquerda, corMaoEsquerda);
    desenharColorP(pontosMaoDireita, numPontosMaoDireita, corMaoDireita);

    //-----------------------------------------------------------------------------
    desenharFeaturesRosto();
}

//-----------------------------------------------------------------------------
/**
 * @brief Função de callback do OpenGL para redesenhar a cena.
 */
void display(void){

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    desenharGroot();

    glutSwapBuffers();
}

//-----------------------------------------------------------------------------
/**
 * @brief Função de callback do OpenGL para redimensionamento da janela.
 * @param largura Nova largura da janela em pixels.
 * @param altura Nova altura da janela em pixels.
 */
void remodelar(int largura, int altura){

    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float proporcao = (float)largura / (float)altura;
    float zoom = 12.0f;

    if (proporcao > 1.0f){

        gluOrtho2D(-zoom * proporcao, zoom * proporcao, -zoom, zoom);
    } else {

        gluOrtho2D(-zoom, zoom, -zoom / proporcao, zoom / proporcao);
    }

    glMatrixMode(GL_MODELVIEW);
}

//-----------------------------------------------------------------------------

int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(768, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Groot");

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(remodelar);

    glutMainLoop();

    return 0;
}
