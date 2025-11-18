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
#include <stdbool.h> /**< Biblioteca que implementa o tipo booleano.*/

#include "background.h" /**< Arquivo .h pessoal que guarda as informações do Background.*/
// ------------------------------------

// Definição de macros utilizadas no código
// ------------------------------------
#define ALTURA 1080
#define LARGURA 1920
#define MOVESPEED 3.0f
#define MOVESPEED_ESPECIAL 5.0f
#define COR_BRANCA (tpCor){200, 200, 200}
#define COR_PRETA (tpCor){60, 60, 60}
#define COR_VERMELHA (tpCor){188, 113, 94}
#define COR_MARROM_ESCURO (tpCor){150, 115, 90}
#define COR_MARROM_MEDIO (tpCor){170, 135, 115}
#define COR_MARROM (tpCor){177, 155, 132}
#define COR_BEGE (tpCor){220, 195, 185}
#define COR_VERDE (tpCor){80, 120, 55}
#define COR_VERDE_CLARO (tpCor){132, 172, 102}
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

// ------------------------------------

/**
 * @struct Limite
 * @brief Uma estrutura que guarda os limites de inclinação das partes do Groot
 */
typedef struct Limite {

    float min;
    float max;
} tpLimite;

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
void teclado (unsigned char tecla, GLint x, GLint y);
void tecladoEspecial(int tecla, GLint x, GLint y);
void ligarPartes(void);
void idle(void);
void initTextura(void);
void desenharBackground(void);
void deletaTextura(void);
float restringirMov(float angulo, tpLimite limite);
tpPonto2D calcularPosicaoAtual(tpPonto2D ponto, tpPonto2D centroRotacao, float anguloGraus);

//-----------------------------------------------------------------------------
// Variáveis Globais
// -----------------
/**
* Abaixo, temos variáveis de suporte para animações.
*
*/
// PERNAS
static GLfloat anguloQuadril = 0.0f;
static GLfloat anguloCoxaDireita = 0.0f;
static GLfloat anguloCoxaEsquerda = 0.0f;
static GLfloat anguloPantDireita = 0.0f;
static GLfloat anguloPantEsquerda = 0.0f;
static GLfloat anguloPataDireita = 0.0f;
static GLfloat anguloPataEsquerda = 0.0f;

// BRAÇOS
static GLfloat anguloOmbroDireito = 0.0f;
static GLfloat anguloOmbroEsquerdo = 0.0f;
static GLfloat anguloBracoDireito = 0.0f;
static GLfloat anguloBracoEsquerdo = 0.0f;
static GLfloat anguloAntebracoDireito = 0.0f;
static GLfloat anguloAntebracoEsquerdo = 0.0f;
static GLfloat anguloMaoDireita = 0.0f;
static GLfloat anguloMaoEsquerda = 0.0f;

// TRONCO / CABEÇA
static GLfloat anguloCabeca = 0.0f;
static GLfloat anguloTronco = 0.0f;

// ESCALA E POSIÇÃO
float xGroot = 0.0f;
float yGroot = -1.0f;
float escalaGroot = 1.0f;

// ESTADO
bool esq = false;
bool dir = false;
bool animando = false;

bool acenando = false;
unsigned char etapaAceno = 0;

bool chutando = false;
unsigned char etapaChute = 0;

// LIMITES DE MOVIMENTO
tpLimite limOmbroEsq = {-182.0f, 114.0f};
tpLimite limOmbroDir = {-114.0f, 182.0f};

tpLimite limAntebracoEsq = {-8.0f, 172.0f};
tpLimite limAntebracoDir = {-172.0f, 8.0f};

tpLimite limMaoEsq = {-90.0f, 106.0f};
tpLimite limMaoDir = {-106.0f, 90.0f};

tpLimite limCoxaEsq = {-88.0f, 38.0f};
tpLimite limCoxaDir = {-38.f, 88.0f};

tpLimite limPantEsq = {-12.0f, 126.0f};
tpLimite limPantDir = {-126.0f, 12.0f};

tpLimite limPataEsq = {-10.0f, 16.0f};
tpLimite limPataDir = {-16.0f, 10.0f};

tpLimite limQuadril = {-16.0f, 16.0f};
tpLimite limTronco = {-10.0f, 10.0f};
tpLimite limCabeca = {-15.0f, 15.0f};

tpLimite limXGroot = {-23.0f, 23.0f};
tpLimite limYGroot = {-7.5f, 0.5f};
tpLimite limEscalaGroot = {0.7f, 2.3f};

// PARA BACKGROUND
GLuint texturaID;

/**
* Abaixo, temos uma sequencia de vetores de pontos2D seguidos de um int, um float e uma cor que,
* respectivamente, estes guardam os pontos de determinadas partes do corpo do Groot, o número de pontos
* daquela parte, o seu ponto pivô e cor principal.
*
*/
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
tpPonto2D pivoCabeca = {0.0f, 4.5f};
tpCor corCabeca = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosTronco[] = {

    {0.0f, 3.5f},  // L_2 (Reta)
    {1.0f, 3.5f},  // K_2 (Reta)
    {2.75f, 2.25f},  // A (Reta)
    {2.0f, 1.0f},  // V_3 (Reta)
    {2.0f, -3.5f},  // B (Reta)
    {0.0f, -3.5f},  // P_3 (Reta)
    {-2.0f, -3.5f},  // PB (Reta)
    {-2.0f, 1.0f},  // PV_3 (Reta)
    {-2.75f, 2.25f},  // PA (Reta)
    {-1.0f, 3.5f},  // PK_2 (Reta)
    {0.0f, 3.5f}  // PL_2 (Reta)

};
int numPontosTronco = 11;
tpPonto2D pivoTronco = {0.0f, 0.0f};
tpCor corTronco = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosQuadril[] = {

    {-2.0f, -3.0f},
    {-2.0f, -4.5f},
    {-1.0f, -4.5f},
    {1.0f, -4.5f},
    {2.0f, -4.5f},
    {2.0f, -3.0f}
};
int numPontosQuadril = 6;
tpPonto2D pivoQuadril = {0.0f, -3.65f};
tpCor corQuadril = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosOmbroEsquerdo[] = {

    {-2.2f, 2.5f},  // M_2 (Reta)
    {-2.75f, 2.325f},  // B_3 (Reta)
    {-3.1f, 2.0f},  // C_3 (Reta)
    {-3.25f, 1.60f},  // D_3 (Reta)
    {-3.25f, 1.20f},  // A (Reta)
    {-2.675f, 1.20f},
    {-2.0f, 1.20f},  // B (Reta)
    {-2.1f, 1.85f}
};
int numPontosOmbroEsquerdo = 8;
tpPonto2D pivoOmbroEsquerdo = {-2.55f, 1.9f};
tpCor corOmbroEsquerdo = COR_MARROM_ESCURO;

//-----------------------------------------------------------------------------

tpPonto2D pontosOmbroDireito[] = {

    {2.2f, 2.5f},  // M_2 (Reta)
    {2.75f, 2.325f},  // B_3 (Reta)
    {3.1f, 2.0f},  // C_3 (Reta)
    {3.25f, 1.60f},  // D_3 (Reta)
    {3.25f, 1.20f},  // A (Reta)
    {2.675f, 1.20f},
    {2.0f, 1.20f},  // B (Reta)
    {2.1f, 1.85f}
};
int numPontosOmbroDireito = 8;
tpPonto2D pivoOmbroDireito = {2.55f, 1.9f};
tpCor corOmbroDireito = COR_MARROM_ESCURO;

//-----------------------------------------------------------------------------

tpPonto2D pontosBracoDireito[] = {

    {3.25f, 1.10f},  // E_3 (Reta)
    {3.45f, -1.45f},  // A (Reta)
    {2.2f, -1.25f},  // B (Reta)
    {2.0f, 1.10f},  // C (Reta)
};
int numPontosBracoDireito = 4;
tpPonto2D pivoBracoDireito = {2.625f, 1.00f};
tpCor corBracoDireito = COR_MARROM_ESCURO;

//-----------------------------------------------------------------------------

tpPonto2D pontosBracoEsquerdo[] = {

    {-3.25f, 1.20f},  // E_3 (Reta)
    {-3.45f, -1.45f},  // A (Reta)
    {-2.2f, -1.25f},  // B (Reta)
    {-2.0f, 1.20f}  // C (Reta)
};
int numPontosBracoEsquerdo = 4;
tpPonto2D pivoBracoEsquerdo = {-2.625f, 0.50f};
tpCor corBracoEsquerdo = COR_MARROM_ESCURO;

//-----------------------------------------------------------------------------

tpPonto2D pontosAntebracoDireito[] = {

    {3.45f, -1.60f},  // F_3 (Reta)
    {3.35f, -2.5f},
    {3.25f, -3.85f},  // G_3 (Reta)
    {2.8f, -4.0f},  // A_4 (Reta)
    {2.2f, -1.25f},  // B_4 (Reta)
    {2.825f, -0.25f}
};
int numPontosAntebracoDireito = 6;
tpPonto2D pivoAntebracoDireito = {2.825f, -1.25f};
tpCor corAntebracoDireito = COR_BEGE;

//-----------------------------------------------------------------------------

tpPonto2D pontosAntebracoEsquerdo[] = {

    {-3.45f, -1.60f},  // F_3 (Reta)
    {-3.35f, -2.5f},
    {-3.25f, -3.85f},  // G_3 (Reta)
    {-2.8f, -4.0f},  // A_4 (Reta)
    {-2.2f, -1.25f},  // B_4 (Reta)
    {-2.825f, -0.25f}
};
int numPontosAntebracoEsquerdo = 6;
tpPonto2D pivoAntebracoEsquerdo = {-2.825f, -1.25f};
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
tpPonto2D pivoMaoDireita = {3.0f, -4.0f};
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
tpPonto2D pivoMaoEsquerda = {-3.0f, -3.75f};
tpCor corMaoEsquerda = COR_BEGE;

//-----------------------------------------------------------------------------

tpPonto2D pontosCoxaDireita[] = {
    {0.15f, -4.3f},  // O_3 (Reta)
    {0.4f, -4.6f},  // N_3 (Reta)
    {0.4f, -5.25f},
    {0.4f, -7.0f},  // D (Reta)
    {2.0f, -7.0f},  // A (Reta)
    {2.0f, -4.0f},  // K_3 (Reta)
    {2.0f, -3.0f},  // B (Reta)
    {0.65f, -3.4f},  // C (Reta, animar aqui)
};
int numPontosCoxaDireita = 8;
tpPonto2D pivoCoxaDireita = {1.02f, -3.0f};
tpCor corCoxaDireita = COR_MARROM_ESCURO;

//-----------------------------------------------------------------------------

tpPonto2D pontosCoxaEsquerda[] = {
    {-0.15f, -4.3f},  // O_3 (Reta)
    {-0.4f, -4.6f},  // N_3 (Reta)
    {-0.4f, -5.25f},
    {-0.4f, -7.0f},  // D (Reta)
    {-2.0f, -7.0f},  // A (Reta)
    {-2.0f, -4.0f},  // K_3 (Reta)
    {-2.0f, -3.0f},  // B (Reta)
    {-0.65f, -3.4f},  // C (Reta)
};
int numPontosCoxaEsquerda = 8;
tpPonto2D pivoCoxaEsquerda = {-1.02f, -3.0f};
tpCor corCoxaEsquerda = COR_MARROM_ESCURO;

//-----------------------------------------------------------------------------

tpPonto2D pontosPantDireita[] = {

    {2.0f, -5.5f},  // A (Reta)
    {0.4f, -7.0f},  // B (Reta)
    {0.4f, -8.4f},  // D (Reta)
    {2.0f, -8.4f},  // C (Reta)
};
int numPontosPantDireita = 4;
tpPonto2D pivoPantDireita = {1.2f, -7.0f};
tpCor corPantDireita = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosPantEsquerda[] = {

    {-2.0f, -5.5f},  // A (Reta)
    {-0.4f, -7.0f},  // B (Reta)
    {-0.4f, -8.4f},  // D (Reta)
    {-2.0f, -8.4f},  // C (Reta)
};
int numPontosPantEsquerda = 4;
tpPonto2D pivoPantEsquerda = {-1.2f, -7.0f};
tpCor corPantEsquerda = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosPataDireita[] = {

    {2.0f, -8.25f},  // L_3 (Reta)
    {0.4f, -8.0f},  // R_3 (Reta)
    {0.25f, -10.0f},  // T_3 (Reta)
    {2.3f, -10.0f},  // M_3 (Reta)
};
int numPontosPataDireita = 4;
tpPonto2D pivoPataDireita = {1.2f, -9.225f};
tpCor corPataDireita = COR_MARROM;

//-----------------------------------------------------------------------------

tpPonto2D pontosPataEsquerda[] = {

    {-2.0f, -8.25f},  // L_3 (Reta)
    {-0.4f, -8.0f},  // R_3 (Reta)
    {-0.25f, -10.0f},  // T_3 (Reta)
    {-2.3f, -10.0f},  // M_3 (Reta)
};
int numPontosPataEsquerda = 4;
tpPonto2D pivoPataEsquerda = {-1.2f, -9.225f};
tpCor corPataEsquerda = COR_MARROM;
//-----------------------------------------------------------------------------

tpPonto2D pontosMaoAbertaEsquerda[] = {

    {-3.25f, -3.70f},  // N
    {-2.87f, -3.70f},  // O
    {-2.67f, -3.90f},  // P
    {-2.60f, -4.50f},  // Q
    {-2.60f, -4.85f},  // R
    {-2.82f, -5.47f},  // S
    {-3.32f, -5.50f}, // T
    {-3.50f, -4.00f},  // U
    {-3.60f, -4.35f},  // V
    {-3.34f, -4.65f}, // W
    {-3.32f, -4.40f} // X
};
int numPontosMaoAbertaEsquerda = 11;
//-----------------------------------------------------------------------------

tpPonto2D pontosMaoAbertaDireita[] = {

    {3.25f, -3.70f},  // N
    {2.87f, -3.70f},  // O
    {2.67f, -3.90f},  // P
    {2.60f, -4.50f},  // Q
    {2.60f, -4.85f},  // R
    {2.82f, -5.47f},  // S
    {3.32f, -5.50f}, // T
    {3.50f, -4.00f},  // U
    {3.60f, -4.35f},  // V
    {3.34f, -4.65f}, // W
    {3.32f, -4.40f} // X
};
int numPontosMaoAbertaDireita = 11;
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
    desenharElipse(-1.30f, 10.25f, 0.35f, 0.15f, 120, COR_VERDE_CLARO, 45);

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

    ligarPartes();

// TRONCO -----------------------------------------------------------------------------
    glPushMatrix();
        glTranslatef(pivoTronco.x, pivoTronco.y, 0);
        glRotatef(anguloTronco, 0, 0, 1);
        glTranslatef(-pivoTronco.x, -pivoTronco.y, 0);
        desenharPG(pontosTronco, numPontosTronco, corTronco, COR_MARROM_ESCURO, 0.5f, 0.7f);

// QUADRIL -----------------------------------------------------------------------------
        glPushMatrix();
            glTranslatef(pivoQuadril.x, pivoQuadril.y, 0);
            glRotatef(anguloQuadril, 0, 0, 1);
            glTranslatef(-pivoQuadril.x, -pivoQuadril.y, 0);
            desenharPG(pontosQuadril, numPontosQuadril, COR_MARROM_ESCURO, corQuadril, 0.25f, 0.75f);

// COXA ESQUERDA -----------------------------------------------------------------------------
            glPushMatrix();
                glTranslatef(pivoCoxaEsquerda.x, pivoCoxaEsquerda.y, 0);
                glRotatef(anguloCoxaEsquerda, 0, 0, 1);
                glTranslatef(-pivoCoxaEsquerda.x, -pivoCoxaEsquerda.y, 0);
                desenharColorP(pontosCoxaEsquerda, numPontosCoxaEsquerda, corCoxaEsquerda);

// PANTURRILHA ESQUERDA -----------------------------------------------------------------------------
                glPushMatrix();
                    glTranslatef(pivoPantEsquerda.x, pivoPantEsquerda.y, 0);
                    glRotatef(anguloPantEsquerda, 0, 0, 1);
                    glTranslatef(-pivoPantEsquerda.x, -pivoPantEsquerda.y, 0);
                    desenharPG(pontosPantEsquerda, numPontosPantEsquerda, corPantEsquerda, COR_MARROM_ESCURO, 0.1f, 0.4f);

// PATA ESQUERDA -----------------------------------------------------------------------------
                    glPushMatrix();
                        glTranslatef(pivoPataEsquerda.x, pivoPataEsquerda.y, 0);
                        glRotatef(anguloPataEsquerda, 0, 0, 1);
                        glTranslatef(-pivoPataEsquerda.x, -pivoPataEsquerda.y, 0);
                        desenharPG(pontosPataEsquerda, numPontosPataEsquerda, COR_BEGE, corPataEsquerda, 0.1f, 0.40f);
                    glPopMatrix();

                glPopMatrix(); // POP PANTURRILHA ESQUERDA
            glPopMatrix(); // POP COXA ESQUERDA

// COXA DIREITA -----------------------------------------------------------------------------
            glPushMatrix();
                glTranslatef(pivoCoxaDireita.x, pivoCoxaDireita.y, 0);
                glRotatef(anguloCoxaDireita, 0, 0, 1);
                glTranslatef(-pivoCoxaDireita.x, -pivoCoxaDireita.y, 0);
                desenharColorP(pontosCoxaDireita, numPontosCoxaDireita, corCoxaDireita);

// PANTURRILHA DIREITA -----------------------------------------------------------------------------
                glPushMatrix();
                    glTranslatef(pivoPantDireita.x, pivoPantDireita.y, 0);
                    glRotatef(anguloPantDireita, 0, 0, 1);
                    glTranslatef(-pivoPantDireita.x, -pivoPantDireita.y, 0);
                    desenharPG(pontosPantDireita, numPontosPantDireita, corPantDireita, COR_MARROM_ESCURO, 0.1f, 0.4f);

// PATA DIREITA -----------------------------------------------------------------------------
                    glPushMatrix();
                        glTranslatef(pivoPataDireita.x, pivoPataDireita.y, 0);
                        glRotatef(anguloPataDireita, 0, 0, 1);
                        glTranslatef(-pivoPataDireita.x, -pivoPataDireita.y, 0);
                        desenharPG(pontosPataDireita, numPontosPataDireita, COR_BEGE, corPataDireita, 0.1f, 0.40f);
                    glPopMatrix();

                glPopMatrix(); // POP PANTURRILHA DIREITA
            glPopMatrix(); // POP COXA DIREITA

        glPopMatrix(); // POP QUADRIL

// OMBRO ESQUERDO -----------------------------------------------------------------------------
        glPushMatrix();
            glTranslatef(pivoOmbroEsquerdo.x, pivoOmbroEsquerdo.y, 0);
            glRotatef(anguloOmbroEsquerdo, 0, 0, 1);
            glTranslatef(-pivoOmbroEsquerdo.x, -pivoOmbroEsquerdo.y, 0);
            desenharColorP(pontosOmbroEsquerdo, numPontosOmbroEsquerdo, corOmbroEsquerdo);

// BRAÇO ESQUERDO -----------------------------------------------------------------------------
            glPushMatrix();
                glTranslatef(pivoBracoEsquerdo.x, pivoBracoEsquerdo.y, 0);
                glRotatef(anguloBracoEsquerdo, 0, 0, 1);
                glTranslatef(-pivoBracoEsquerdo.x, -pivoBracoEsquerdo.y, 0);
                desenharPG(pontosBracoEsquerdo, numPontosBracoEsquerdo, COR_BEGE, corBracoEsquerdo, 0.1f, 1.0f);

// ANTEBRAÇO ESQUERDO -----------------------------------------------------------------------------
                glPushMatrix();
                    glTranslatef(pivoAntebracoEsquerdo.x, pivoAntebracoEsquerdo.y, 0);
                    glRotatef(anguloAntebracoEsquerdo, 0, 0, 1);
                    glTranslatef(-pivoAntebracoEsquerdo.x, -pivoAntebracoEsquerdo.y, 0);
                    desenharColorP(pontosAntebracoEsquerdo, numPontosAntebracoEsquerdo, corAntebracoEsquerdo);

// MÃO ESQUERDA -----------------------------------------------------------------------------
                    glPushMatrix();

                    if(anguloAntebracoEsquerdo > -30){

                        glTranslatef(pivoMaoEsquerda.x, pivoMaoEsquerda.y, 0);
                        glRotatef(anguloMaoEsquerda, 0, 0, 1);
                        glTranslatef(-pivoMaoEsquerda.x, -pivoMaoEsquerda.y, 0);
                        desenharColorP(pontosMaoEsquerda, numPontosMaoEsquerda, corMaoEsquerda);
                    }
                    else{

                        glTranslatef(pivoMaoEsquerda.x, pivoMaoEsquerda.y, 0);
                        glRotatef(anguloMaoEsquerda, 0, 0, 1);
                        glTranslatef(-pivoMaoEsquerda.x, -pivoMaoEsquerda.y, 0);
                        desenharColorP(pontosMaoAbertaEsquerda, numPontosMaoAbertaEsquerda, corMaoEsquerda);
                    }
                    glPopMatrix();

                glPopMatrix(); // ANTEBRAÇO ESQUERDA
            glPopMatrix(); // BRAÇO ESQUERDO
        glPopMatrix(); // OMBRO ESQUERDO

// OMBRO DIREITO -----------------------------------------------------------------------------
        glPushMatrix();
            glTranslatef(pivoOmbroDireito.x, pivoOmbroDireito.y, 0);
            glRotatef(anguloOmbroDireito, 0, 0, 1);
            glTranslatef(-pivoOmbroDireito.x, -pivoOmbroDireito.y, 0);
            desenharColorP(pontosOmbroDireito, numPontosOmbroDireito, corOmbroDireito);

// BRAÇO DIREITO -----------------------------------------------------------------------------
            glPushMatrix();
                glTranslatef(pivoBracoDireito.x, pivoBracoDireito.y, 0);
                glRotatef(anguloBracoDireito, 0, 0, 1);
                glTranslatef(-pivoBracoDireito.x, -pivoBracoDireito.y, 0);
                desenharPG(pontosBracoDireito, numPontosBracoDireito, COR_BEGE, corBracoDireito, 0.1f, 1.0f);

// ANTEBRACO DIREITO -----------------------------------------------------------------------------
                glPushMatrix();
                    glTranslatef(pivoAntebracoDireito.x, pivoAntebracoDireito.y, 0);
                    glRotatef(anguloAntebracoDireito, 0, 0, 1);
                    glTranslatef(-pivoAntebracoDireito.x, -pivoAntebracoDireito.y, 0);
                    desenharColorP(pontosAntebracoDireito, numPontosAntebracoDireito, corAntebracoDireito);

// MÃO DIREITA -----------------------------------------------------------------------------
                    glPushMatrix();

                    if(anguloAntebracoDireito < 30){

                        glTranslatef(pivoMaoDireita.x, pivoMaoDireita.y, 0);
                        glRotatef(anguloMaoDireita, 0, 0, 1);
                        glTranslatef(-pivoMaoDireita.x, -pivoMaoDireita.y, 0);
                        desenharColorP(pontosMaoDireita, numPontosMaoDireita, corMaoDireita);
                    }
                    else{

                        glTranslatef(pivoMaoDireita.x, pivoMaoDireita.y, 0);
                        glRotatef(anguloMaoDireita, 0, 0, 1);
                        glTranslatef(-pivoMaoDireita.x, -pivoMaoDireita.y, 0);
                        desenharColorP(pontosMaoAbertaDireita, numPontosMaoAbertaDireita, corMaoDireita);
                    }
                    glPopMatrix();

                glPopMatrix(); // ANTEBRAÇO DIREITO
            glPopMatrix(); // BRAÇO DIREITO
        glPopMatrix(); // OMBRO DIREITO

// CABEÇA -----------------------------------------------------------------------------
        glPushMatrix();
            glTranslatef(pivoCabeca.x, pivoCabeca.y, 0);
            glRotatef(anguloCabeca, 0, 0, 1);
            glTranslatef(-pivoCabeca.x, -pivoCabeca.y, 0);
            desenharPG(pontosCabeca, numPontosCabeca, corCabeca, COR_VERDE, 0.70f, 0.9f);
            desenharFeaturesRosto();
        glPopMatrix();

    glPopMatrix(); // POP TRONCO

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
/**
 * @brief Calcula a nova posição de um ponto após rotação em torno de um centro.
 *
 * @param ponto Ponto original que será rotacionado.
 * @param centroRotacao Ponto que serve como pivô da rotação.
 * @param anguloGraus Ângulo da rotação em graus.
 * @return tpPonto2D Novo ponto resultante da rotação.
 */
tpPonto2D calcularPosicaoAtual(tpPonto2D ponto, tpPonto2D centroRotacao, float anguloGraus) {

    float anguloRad = anguloGraus _R;
    float cosA = cos(anguloRad);
    float sinA = sin(anguloRad);

    float xRel = ponto.x - centroRotacao.x;
    float yRel = ponto.y - 0.2f - centroRotacao.y;

    float xRot = xRel * cosA - yRel * sinA;
    float yRot = xRel * sinA + yRel * cosA;

    float xFinal = xRot + centroRotacao.x;
    float yFinal = yRot + centroRotacao.y;

    return (tpPonto2D){xFinal, yFinal};
}

//-----------------------------------------------------------------------------
/**
 * @brief Atualiza a ligação entre todas as partes do corpo do Groot após rotação.
 *
 * Esta função ajusta os pontos das partes conectadas, garantindo que
 * quando uma articulação se move, as partes dependentes acompanhem
 * corretamente a animação.
 */
void ligarPartes(void){

    // TRONCO + QUADRIL
    pontosTronco[4] = calcularPosicaoAtual(pontosQuadril[5], pivoQuadril, anguloQuadril);
    pontosTronco[6] = calcularPosicaoAtual(pontosQuadril[0], pivoQuadril, anguloQuadril);

    // TRONCO + OMBROS
    pontosTronco[9] = calcularPosicaoAtual(pontosOmbroEsquerdo[0], pivoOmbroEsquerdo, anguloOmbroEsquerdo);
    pontosTronco[7] = calcularPosicaoAtual(pontosOmbroEsquerdo[7], pivoOmbroEsquerdo, anguloOmbroEsquerdo);

    pontosTronco[1] = calcularPosicaoAtual(pontosOmbroDireito[0], pivoOmbroDireito, anguloOmbroDireito);
    pontosTronco[3] = calcularPosicaoAtual(pontosOmbroDireito[7], pivoOmbroDireito, anguloOmbroDireito);

    // QUADRIL + COXAS
    pontosQuadril[1] = calcularPosicaoAtual(pontosCoxaEsquerda[5], pivoCoxaEsquerda, anguloCoxaEsquerda);
    pontosQuadril[2] = calcularPosicaoAtual(pontosCoxaEsquerda[2], pivoCoxaEsquerda, anguloCoxaEsquerda);

    pontosQuadril[4] = calcularPosicaoAtual(pontosCoxaDireita[6], pivoCoxaDireita, anguloCoxaDireita);
    pontosQuadril[3] = calcularPosicaoAtual(pontosCoxaDireita[2], pivoCoxaDireita, anguloCoxaDireita);

    // OMBROS + BRAÇOS
    pontosOmbroDireito[4] = calcularPosicaoAtual(pontosBracoDireito[0], pivoBracoDireito, anguloBracoDireito);
    pontosOmbroDireito[5] = calcularPosicaoAtual(pontosBracoDireito[3], pivoBracoDireito, anguloBracoDireito);

    pontosOmbroEsquerdo[4] = calcularPosicaoAtual(pontosBracoEsquerdo[0], pivoBracoEsquerdo, anguloBracoEsquerdo);
    pontosOmbroEsquerdo[5] = calcularPosicaoAtual(pontosBracoEsquerdo[3], pivoBracoEsquerdo, anguloBracoEsquerdo);

    // BRAÇOS + ANTEBRAÇOS
    pontosBracoDireito[1] = calcularPosicaoAtual(anguloAntebracoDireito <= -34 ? pontosAntebracoDireito[5] : pontosAntebracoDireito[0], pivoAntebracoDireito, anguloAntebracoDireito);

    pontosBracoEsquerdo[1] = calcularPosicaoAtual(anguloAntebracoEsquerdo >= 34 ? pontosAntebracoEsquerdo[5] : pontosAntebracoEsquerdo[0], pivoAntebracoEsquerdo, anguloAntebracoEsquerdo);

    // ANTEBRAÇOS + MÃOS
    pontosAntebracoDireito[1] = calcularPosicaoAtual(pontosMaoDireita[0], pivoMaoDireita, anguloMaoDireita);

    pontosAntebracoEsquerdo[1] = calcularPosicaoAtual(pontosMaoEsquerda[8], pivoMaoEsquerda, anguloMaoEsquerda);

    // COXAS + PANTURRILHA
    pontosCoxaDireita[3] = calcularPosicaoAtual(pontosPantDireita[1], pivoPantDireita, anguloPantDireita);
    pontosCoxaDireita[4] = calcularPosicaoAtual(pontosPantDireita[0], pivoPantDireita, anguloPantDireita);

    pontosCoxaEsquerda[3] = calcularPosicaoAtual(pontosPantEsquerda[1], pivoPantEsquerda, anguloPantEsquerda);
    pontosCoxaEsquerda[4] = calcularPosicaoAtual(pontosPantEsquerda[0], pivoPantEsquerda, anguloPantEsquerda);

    // PANTURRILHA + PÉS
    pontosPantDireita[2] = calcularPosicaoAtual(pontosPataDireita[1], pivoPataDireita, anguloPataDireita);
    pontosPantDireita[3] = calcularPosicaoAtual(pontosPataDireita[0], pivoPataDireita, anguloPataDireita);

    pontosPantEsquerda[2] = calcularPosicaoAtual(pontosPataEsquerda[1], pivoPataEsquerda, anguloPataEsquerda);
    pontosPantEsquerda[3] = calcularPosicaoAtual(pontosPataEsquerda[0], pivoPataEsquerda, anguloPataEsquerda);
}

//-----------------------------------------------------------------------------
/**
 * @brief Limita um ponto/angulo dentro de um intervalo permitido.
 *
 * Garante que uma articulação não ultrapasse seus limites mecânicos,
 * evitando animações anatomicamente impossíveis, além de ser utilizada
 * para regular escalas e movimentações no plano XY.
 *
 * @param angulo Ângulo/Ponto atual que será verificado.
 * @param limite Estrutura contendo mínimo e máximo permitidos.
 * @return float Ângulo/Ponto corrigido, se necessário.
 */
float restringirMov(float angulo, tpLimite limite){

    if(angulo > limite.max) return limite.max;
    if(angulo < limite.min) return limite.min;
    return angulo;
}

//-----------------------------------------------------------------------------
/**
 * @brief Contém as operações para animar Groot chutando.
 */
void animarChute(void) {

    if (!chutando) return;

    // PERNA DIREITA -----------------------------------------------------------------------------
    if (dir) {

        if (etapaChute == 0) {

            anguloCoxaDireita += MOVESPEED_ESPECIAL;
            anguloPantDireita -= MOVESPEED_ESPECIAL;
            if (anguloCoxaDireita >= 45.0f) etapaChute = 1;
        }

        else if (etapaChute == 1) {

            anguloPantDireita += MOVESPEED_ESPECIAL;
            if (anguloPantDireita >= 10.0f) etapaChute = 2;
        }

        else if (etapaChute == 2) {

            static int direcao = 1;
            static int ciclo = 0;

            anguloPantDireita += direcao * MOVESPEED_ESPECIAL;

            if (anguloPantDireita >= 10.0f) direcao = -1;
            if (anguloPantDireita <= -30.0f) {

                direcao = 1;
                ciclo++;
            }

            if (ciclo == 1) {

                etapaChute = 3;
                ciclo = 0;
            }
        }

        else if (etapaChute == 3) {

            bool terminou = true;

            if (anguloCoxaDireita > 0) {

                anguloCoxaDireita -= MOVESPEED_ESPECIAL;
                if (anguloCoxaDireita < 0) anguloCoxaDireita = 0;
                terminou = false;
            }

            if (anguloPantDireita < 0) {

                anguloPantDireita += MOVESPEED_ESPECIAL;
                if (anguloPantDireita > 0) anguloPantDireita = 0;
                terminou = false;
            }

            if (terminou) {

                dir = false;
                chutando = false;
                animando = false;
                etapaChute = 0;
            }
        }
    }

    // PERNA ESQUERDA -----------------------------------------------------------------------------
    if (esq) {

        if (etapaChute == 0) {

            anguloCoxaEsquerda -= MOVESPEED_ESPECIAL;
            anguloPantEsquerda += MOVESPEED_ESPECIAL;
            if (anguloCoxaEsquerda <= -45.0f) etapaChute = 1;
        }

        else if (etapaChute == 1) {

            anguloPantEsquerda += MOVESPEED_ESPECIAL;
            if (anguloPantEsquerda >= 30.0f) etapaChute = 2;
        }

        else if (etapaChute == 2) {

            static int direcao = 1;
            static int ciclo = 0;

            anguloPantEsquerda -= direcao * MOVESPEED_ESPECIAL;

            if (anguloPantEsquerda <= -20.0f) direcao = -1;
            if (anguloPantEsquerda >= 30.0f) {

                direcao = 1;
                ciclo++;
            }

            if (ciclo == 5) {

                etapaChute = 3;
                ciclo = 0;
            }
        }

        else if (etapaChute == 3) {

            bool terminou = true;

            if (anguloCoxaEsquerda < 0) {

                anguloCoxaEsquerda += MOVESPEED_ESPECIAL;
                if (anguloCoxaEsquerda > 0) anguloCoxaEsquerda = 0;
                terminou = false;
            }

            if (anguloPantEsquerda > 0) {

                anguloPantEsquerda -= MOVESPEED_ESPECIAL;
                if (anguloPantEsquerda < 0) anguloPantEsquerda = 0;
                terminou = false;
            }

            if (terminou) {

                esq = false;
                chutando = false;
                animando = false;
                etapaChute = 0;
            }
        }
    }
}

//-----------------------------------------------------------------------------

/**
 * @brief Contém as operações para animar Groot acenando.
 */
void animarAceno(void) {

    if (!acenando) return;

    // BRAÇO DIREITO -----------------------------------------------------------------------------
    if (dir) {

        if (etapaAceno == 0) {

            anguloOmbroDireito += MOVESPEED_ESPECIAL;
            anguloCabeca += 0.5f;
            if (anguloOmbroDireito >= 90) etapaAceno = 1;
        }

        else if (etapaAceno == 1){

            anguloAntebracoDireito += MOVESPEED_ESPECIAL;
            if (anguloAntebracoDireito > 79.0f) etapaAceno = 2;
        }

        else if (etapaAceno == 2) {

            static int direcao = 1;
            static int ciclo = 0;

            anguloAntebracoDireito += direcao * MOVESPEED_ESPECIAL;

            if (anguloAntebracoDireito >= 120.0f) direcao = -1;
            if (anguloAntebracoDireito <= 40.0f) {

                direcao = 1;
                ciclo++;
            }

            if (ciclo == 3){

                etapaAceno = 3;
                ciclo = 0;
            }
        }

        else if (etapaAceno == 3) {

            bool terminou = true;

            if (anguloOmbroDireito > 0) {
                anguloCabeca -= 0.5f;
                anguloOmbroDireito -= MOVESPEED_ESPECIAL;
                if (anguloOmbroDireito < 0) anguloOmbroDireito = 0;
                terminou = false;
            }

            if (anguloAntebracoDireito > 0) {

                anguloAntebracoDireito -= MOVESPEED_ESPECIAL;

                if (anguloAntebracoDireito < 0) anguloAntebracoDireito = 0;

                terminou = false;
            }

            if (terminou) {

                dir = false;
                acenando = false;
                animando = false;
                etapaAceno = 0;
            }
        }
    }

    // BRAÇO ESQUERDO -----------------------------------------------------------------------------
    else if (esq) {

        if (etapaAceno == 0) {

            anguloOmbroEsquerdo -= MOVESPEED_ESPECIAL;
            anguloCabeca -= 0.5f;
            if (anguloOmbroEsquerdo <= -90) etapaAceno = 1;
        }

        else if (etapaAceno == 1){

            anguloAntebracoEsquerdo -= MOVESPEED_ESPECIAL;
            if (anguloAntebracoEsquerdo < -80.0f) etapaAceno = 2;
        }

        else if (etapaAceno == 2) {

            static int direcao = 1;
            static int ciclo = 0;

            anguloAntebracoEsquerdo -= direcao * MOVESPEED_ESPECIAL;

            if (anguloAntebracoEsquerdo <= -120.0f) direcao = -1;
            if (anguloAntebracoEsquerdo >= -40.0f) {
                direcao = 1;
                ciclo++;
            }

            if (ciclo == 3){

                etapaAceno = 3;
                ciclo = 0;
            }
        }

        else if (etapaAceno == 3) {

            bool terminou = true;

            if (anguloOmbroEsquerdo < 0) {
                anguloCabeca += 0.5f;
                anguloOmbroEsquerdo += MOVESPEED_ESPECIAL;
                if (anguloOmbroEsquerdo > 0) anguloOmbroEsquerdo = 0;
                terminou = false;
            }

            if (anguloAntebracoEsquerdo < 0) {
                anguloAntebracoEsquerdo += MOVESPEED_ESPECIAL;
                if (anguloAntebracoEsquerdo > 0) anguloAntebracoEsquerdo = 0;
                terminou = false;
            }

            if (terminou) {
                esq = false;
                acenando = false;
                animando = false;
                etapaAceno = 0;
            }
        }
    }
}

//-----------------------------------------------------------------------------
/**
 * @brief Função de callback do OpenGL responsável por redesenhar a cena.
 *
 * Limpa o buffer, desenha fundo, posiciona Groot e atualiza a saída
 * gráfica com double buffering.
 */
void display(void) {

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(255, 255, 255);

    desenharBackground();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3ub(255, 255, 255);

    glPushMatrix();

        glTranslatef(xGroot, yGroot, 0.0f);
        glScalef(escalaGroot, escalaGroot, 1.0f);
        desenharGroot();
    glPopMatrix();

    glutSwapBuffers();
}

//-----------------------------------------------------------------------------
/**
 * @brief Callback de teclado para controlar articulações manualmente.
 *
 * @param tecla Tecla pressionada.
 * @param x Posição X do mouse no momento da tecla (GLUT exige).
 * @param y Posição Y do mouse no momento da tecla (GLUT exige).
 */
void teclado (unsigned char tecla, GLint x, GLint y){

    if(animando) return;

    switch (tecla) {

        case 'q': anguloOmbroDireito += MOVESPEED;
                  anguloOmbroDireito = restringirMov(anguloOmbroDireito, limOmbroDir);
                  break;
        case 'w': anguloOmbroDireito -= MOVESPEED;
                  anguloOmbroDireito = restringirMov(anguloOmbroDireito, limOmbroDir);
                  break;
        case 'e': anguloAntebracoDireito += MOVESPEED;
                  anguloAntebracoDireito = restringirMov(anguloAntebracoDireito, limAntebracoDir);
                  break;
        case 'r': anguloAntebracoDireito -= MOVESPEED;
                  anguloAntebracoDireito = restringirMov(anguloAntebracoDireito, limAntebracoDir);
                  break;
        case 't': anguloMaoDireita += MOVESPEED;
                  anguloMaoDireita = restringirMov(anguloMaoDireita, limMaoDir);
                  break;
        case 'y': anguloMaoDireita -= MOVESPEED;
                  anguloMaoDireita = restringirMov(anguloMaoDireita, limMaoDir);
                  break;

        case 'u': anguloOmbroEsquerdo += MOVESPEED;
                  anguloOmbroEsquerdo = restringirMov(anguloOmbroEsquerdo, limOmbroEsq);
                  break;
        case 'i': anguloOmbroEsquerdo -= MOVESPEED;
                  anguloOmbroEsquerdo = restringirMov(anguloOmbroEsquerdo, limOmbroEsq);
                  break;
        case 'o': anguloAntebracoEsquerdo += MOVESPEED;
                  anguloAntebracoEsquerdo = restringirMov(anguloAntebracoEsquerdo, limAntebracoEsq);
                  break;
        case 'p': anguloAntebracoEsquerdo -= MOVESPEED;
                  anguloAntebracoEsquerdo = restringirMov(anguloAntebracoEsquerdo, limAntebracoEsq);
                  break;
        case 'a': anguloMaoEsquerda += MOVESPEED;
                  anguloMaoEsquerda = restringirMov(anguloMaoEsquerda, limMaoEsq);
                  break;
        case 's': anguloMaoEsquerda -= MOVESPEED;
                  anguloMaoEsquerda = restringirMov(anguloMaoEsquerda, limMaoEsq);
                  break;

        case 'd': anguloCoxaDireita += MOVESPEED;
                  anguloCoxaDireita = restringirMov(anguloCoxaDireita, limCoxaDir);
                  break;
        case 'f': anguloCoxaDireita -= MOVESPEED;
                  anguloCoxaDireita = restringirMov(anguloCoxaDireita, limCoxaDir);
                  break;
        case 'g': anguloPantDireita += MOVESPEED;
                  anguloPantDireita = restringirMov(anguloPantDireita, limPantDir);
                  break;
        case 'h': anguloPantDireita -= MOVESPEED;
                  anguloPantDireita = restringirMov(anguloPantDireita, limPantDir);
                  break;
        case 'j': anguloPataDireita += MOVESPEED;
                  anguloPataDireita = restringirMov(anguloPataDireita, limPataDir);
                  break;
        case 'k': anguloPataDireita -= MOVESPEED;
                  anguloPataDireita = restringirMov(anguloPataDireita, limPataDir);
                  break;

        case 'l': anguloCoxaEsquerda += MOVESPEED;
                  anguloCoxaEsquerda = restringirMov(anguloCoxaEsquerda, limCoxaEsq);
                  break;
        case 'z': anguloCoxaEsquerda -= MOVESPEED;
                  anguloCoxaEsquerda = restringirMov(anguloCoxaEsquerda, limCoxaEsq);
                  break;
        case 'x': anguloPantEsquerda += MOVESPEED;
                  anguloPantEsquerda = restringirMov(anguloPantEsquerda, limPantEsq);
                  break;
        case 'c': anguloPantEsquerda -= MOVESPEED;
                  anguloPantEsquerda = restringirMov(anguloPantEsquerda, limPantEsq);
                  break;
        case 'v': anguloPataEsquerda += MOVESPEED;
                  anguloPataEsquerda = restringirMov(anguloPataEsquerda, limPataEsq);
                  break;
        case 'b': anguloPataEsquerda -= MOVESPEED;
                  anguloPataEsquerda = restringirMov(anguloPataEsquerda, limPataEsq);
                  break;

        case 'n': anguloQuadril += MOVESPEED;
                  anguloQuadril = restringirMov(anguloQuadril, limQuadril);
                  break;
        case 'm': anguloQuadril -= MOVESPEED;
                  anguloQuadril = restringirMov(anguloQuadril, limQuadril);
                  break;

        case '1': anguloTronco += MOVESPEED;
                  anguloTronco = restringirMov(anguloTronco, limTronco);
                  break;
        case '2': anguloTronco -= MOVESPEED;
                  anguloTronco = restringirMov(anguloTronco, limTronco);
                  break;

        case '3': anguloCabeca += MOVESPEED;
                  anguloCabeca = restringirMov(anguloCabeca, limCabeca);
                  break;
        case '4': anguloCabeca -= MOVESPEED;
                  anguloCabeca = restringirMov(anguloCabeca, limCabeca);
                  break;

        case '0': anguloOmbroDireito = anguloOmbroEsquerdo = 0;
                  anguloBracoDireito = anguloBracoEsquerdo = 0;
                  anguloAntebracoDireito = anguloAntebracoEsquerdo = 0;
                  anguloMaoDireita = anguloMaoEsquerda = 0;
                  anguloCoxaDireita = anguloCoxaEsquerda = 0;
                  anguloPantDireita = anguloPantEsquerda = 0;
                  anguloPataDireita = anguloPataEsquerda = 0;
                  anguloQuadril = 0;
                  anguloTronco = 0;
                  anguloCabeca = 0;
                  break;

        case 27: exit(0); break;
        default: break;
    }

    glutPostRedisplay();
}

//-----------------------------------------------------------------------------
/**
 * @brief Callback de teclado especial (setas, teclas de função, etcetera).
 *
 * A função só executa se nenhuma animação automática estiver ocorrendo,
 * impedindo conflito entre animação e controle manual.
 *
 * @param tecla Código da tecla especial pressionada.
 * @param x Posição X do mouse
 * @param y Posição Y do mouse
 */
void tecladoEspecial(int tecla, GLint x, GLint y){

    if (animando) return;

        switch(tecla){

            case GLUT_KEY_F1: anguloOmbroEsquerdo = 0;
                              anguloAntebracoEsquerdo = 0;
                              anguloBracoEsquerdo = 0;
                              anguloMaoEsquerda = 0;
                              anguloTronco = 0;
                              anguloCabeca = 0;
                              etapaAceno = 0;
                              esq = true;
                              acenando = true;
                              animando = true;
                              break;

            case GLUT_KEY_F2: anguloOmbroDireito = 0;
                              anguloAntebracoDireito = 0;
                              anguloBracoDireito = 0;
                              anguloMaoDireita = 0;
                              anguloTronco = 0;
                              anguloCabeca = 0;
                              etapaAceno = 0;
                              dir = true;
                              acenando = true;
                              animando = true;
                              break;

            case GLUT_KEY_F3: anguloQuadril = 0;
                              anguloCoxaEsquerda = 0;
                              anguloPantEsquerda = 0;
                              anguloPataEsquerda = 0;
                              anguloTronco = 0;
                              etapaChute = 0;
                              esq = true;
                              chutando = true;
                              animando = true;
                              break;

            case GLUT_KEY_F4: anguloQuadril = 0;
                              anguloCoxaDireita = 0;
                              anguloPantDireita = 0;
                              anguloPataDireita = 0;
                              anguloTronco = 0;
                              etapaChute = 0;
                              dir = true;
                              chutando = true;
                              animando = true;
                              break;

            case GLUT_KEY_LEFT: xGroot -= 0.5f;

                                xGroot = restringirMov(xGroot, limXGroot);
                                break;

            case GLUT_KEY_RIGHT: xGroot += 0.5f;

                                 xGroot = restringirMov(xGroot, limXGroot);
                                 break;

            case GLUT_KEY_UP: escalaGroot -= 0.1f;
                              yGroot += 0.5f;

                              escalaGroot = restringirMov(escalaGroot, limEscalaGroot);
                              yGroot = restringirMov(yGroot, limYGroot);
                              break;

            case GLUT_KEY_DOWN: escalaGroot += 0.1f;
                                yGroot -= 0.5f;

                                escalaGroot = restringirMov(escalaGroot, limEscalaGroot);
                                yGroot = restringirMov(yGroot, limYGroot);
                                break;

            case GLUT_KEY_F12: escalaGroot = 1.0f;
                               xGroot = 0.0f;
                               yGroot = -1.0f;
                               break;

            case 27: exit(0); break;
            default: break;
        }
}

//-----------------------------------------------------------------------------
/**
 * @brief Inicializa e carrega a textura do background.
 *
 * - Habilita texturas 2D
 * - Cria o ID da textura
 * - Define filtros de renderização
 * - Envia a imagem lida para a GPU
 */
void initTextura(void) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &texturaID);
    glBindTexture(GL_TEXTURE_2D, texturaID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, backgroundLargura, backgroundAltura, 0, GL_BGR, GL_UNSIGNED_BYTE, backgroundV);
}

//-----------------------------------------------------------------------------
/**
 * @brief Renderiza o background cobrindo todo o viewport.
 *
 * - Liga a textura carregada previamente.
 * - Mantém proporção da imagem independente da janela.
 * - Desenha um retângulo (quad) com coordenadas e textura ajustadas.
 */
void desenharBackground(void) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaID);

    float zoom = 12.0f;
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);

    float proporcao = (float)viewport[2] / (float)viewport[3];

    glBegin(GL_QUADS);

        if (proporcao > 1.0f) {

            glTexCoord2f(0.0f, 0.0f); glVertex2f(-zoom * proporcao, -zoom);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(zoom * proporcao, -zoom);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(zoom * proporcao, zoom);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-zoom * proporcao, zoom);
        }
        else {

            glTexCoord2f(0.0f, 0.0f); glVertex2f(-zoom, -zoom / proporcao);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(zoom, -zoom / proporcao);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(zoom, zoom / proporcao);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-zoom, zoom / proporcao);
        }

    glEnd();
}

//-----------------------------------------------------------------------------
/**
 * @brief Libera memória da textura do background.
 *
 * Deve ser chamada ao fechar o programa
 * para evitar vazamento de memória na GPU.
 */
void deletaTextura(void) {

    glDeleteTextures(1, &texturaID);
}

//-----------------------------------------------------------------------------
/**
 * @brief Função de idle.
 *
 * Atualiza frames continuamente chamando:
 *  - `animarAceno()`
 *  - `animarChute()`
 * Depois, força o redesenho na tela.
 */
void idle(void) {

    animarAceno();
    animarChute();
    glutPostRedisplay();
}

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(LARGURA, ALTURA);
    glutCreateWindow("Groot v2");

    initTextura();

    tecladoEspecial(GLUT_KEY_F1, 0, 0);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(tecladoEspecial);
    glutReshapeFunc(remodelar);

    atexit(deletaTextura);

    glutMainLoop();
    return 0;
}
