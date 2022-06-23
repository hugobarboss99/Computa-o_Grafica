/*
Projeto do jogo space invaders computação grafica
Alunos: 
Hugo Barbosa Santana Silva - 20193002946
Thamiris Souza Madeira Ferreira - 20193008037
Marcos Alves de Castro Baker - 20193008019
*/
#include <gl/gl.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "SOIL.h"
#define tamanhoSprite 0.1f
#define tamanhoPontos 0.0700f
#define tamanhoTiro 0.05f

double direcaoAlien = 1;
double posicaoAlienX = 0; //
double posicaoAlienY = 0; //
int aliens [5][9]= {1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 1, 1, 1, 1
                   };
int AlienMorto = 0;
int IniciaJogo = 0;
int Pontos = 0;
char PontoS[5] = "0000";
int menorlinhavivadacoluna[9];
int ColisaoTex;

struct NAVE
{
    double xi;
    double yi;
    double x;
    double y;
    double status;
    double direcao;
    double posicao;
};

struct TIRO
{
    int status, pos, aperta, natela, colisao;
    double x, xi;
    double y, yi;
};

//======================//

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void DesenhaMenu();
void DesenhaCenario();
void DesenhaCenarioGameOver();
void DesenhaCenarioWin();
void DesenhaET();
void DesenhaNave();
void DesenhaTiro();
void DesenhaPontos();
void Reseta();
void DesenhaTiroA();

struct TIRO tiro_alien[3];

struct NAVE nave;
struct TIRO tiro;
//OpenGL e resolução;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdline,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    // Registro

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;

    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          800,
                          800,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    EnableOpenGL(hwnd, &hDC, &hRC);

    IniciaNave();
    tiro.status = 0;

    //Começo dos tiros dos aliens
    int i;
    for(i=0; i<3; i++)
    {
        tiro_alien[i].status = 0;
    }
    for(i=0; i<9; i++)
    {
        menorlinhavivadacoluna[i]=4;
    }
    while (!bQuit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            DesenhaMenu();
            if (IniciaJogo == 1)
            {
                DesenhaCenario(); //Cena do inicio
                if (nave.status > 0 && AlienMorto < 45)
                {
                    DesenhaPontos();
                    DesenhaET();
                    DesenhaTiro();
                    DesenhaNave();
                    DesenhaTiroA();
                }
                else if(nave.status > 0 && AlienMorto == 45)
                {
                    IniciaJogo = 2;
                }
                else
                {
                    IniciaJogo = 3;
                }
            }
            else if (IniciaJogo == 2)
            {
                DesenhaCenarioWin();
                ResetaJogo();
            }
            else if (IniciaJogo == 3)
            {
                DesenhaCenarioGameOver();
                nave.status = 1;
                ResetaJogo();
            }

            glPopMatrix();
            SwapBuffers(hDC);
            Sleep(10);
        }
    }

    DisableOpenGL(hwnd, hDC, hRC);

    DestroyWindow(hwnd);

    return msg.wParam;
}
//Controles

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case VK_LEFT:
            nave.direcao = -1;
            break;
        case VK_RIGHT:
            nave.direcao = 1;
            break;
        case VK_SPACE:
            tiro.aperta = 1;
            break;
        case '1':
        case VK_NUMPAD1:
            if (IniciaJogo == 3)
            {
                Pontos = 0;
                strcpy(PontoS, "0000");
            }
            IniciaJogo = 1;
        }
    }
    break;

    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_LEFT:
            nave.direcao = 0;
            break;
        case VK_RIGHT:
            nave.direcao = 0;
            break;
        case VK_SPACE:
            tiro.aperta = 0;
            break;
        }
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;

}

//OpenGL
void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    *hDC = GetDC(hwnd);

    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);

    carregaTexturas();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

GLuint AlienTex[3];
GLuint NaveTex;
GLuint CenarioTex;
GLuint CenarioGameOverTex;
GLuint CenarioVitoriaTex;
GLuint MenuTex;
GLuint MorteTex[3];
GLuint NumeroTex[10];

static void desenhaSprite(float coluna,float linha, GLuint tex);
static GLuint carregaArqTextura(char *str);

//Carrega as texturas
void carregaTexturas()
{
    int i;
    char str[50];

    for (i = 0; i < 3; i++)
    {
        sprintf(str,".//fotos//alien%d.png", i);
        AlienTex[i] = carregaArqTextura(str);
    }

    sprintf(str,".//fotos//nave.png");
    NaveTex = carregaArqTextura(str);

    sprintf(str,".//fotos//fundo.png");
    CenarioTex = carregaArqTextura(str);

    sprintf(str,".//fotos//gameoverfracassado.png");
    CenarioGameOverTex = carregaArqTextura(str);

    sprintf(str,".//fotos//vitoria.png");
    CenarioVitoriaTex = carregaArqTextura(str);

    sprintf(str,".//fotos//teladeboasvindas.png");
    MenuTex = carregaArqTextura(str);

    for (i = 0; i < 3; i++)
    {
        sprintf(str,".//fotos//morte%d.png", i);
        MorteTex[i] = carregaArqTextura(str);
    }

    for (i = 0; i < 10; i++)
    {
        sprintf(str, ".//fotos//numeros//numero%d.png", i);
        NumeroTex[i] = carregaArqTextura(str);
    }

    sprintf(str,".//fotos//Colisao.png");
    ColisaoTex = carregaArqTextura(str);
}

// Carrega textura opengl
static GLuint carregaArqTextura(char *str)
{
    GLuint tex = SOIL_load_OGL_texture
                 (
                     str,
                     SOIL_LOAD_AUTO,
                     SOIL_CREATE_NEW_ID,
                     SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
                     SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
                 );

    //Verificando erro durante carregamento
    if(0 == tex)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }

    return tex;
}

//Desenha textura dos aliens e da nave

void desenhaSprite(float coluna,float linha, GLuint tex)
{
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);
    glVertex2f(coluna, linha);
    glTexCoord2f(1.0f,1.0f);
    glVertex2f(coluna + tamanhoSprite, linha);
    glTexCoord2f(1.0f,0.0f);
    glVertex2f(coluna + tamanhoSprite, linha - tamanhoSprite);
    glTexCoord2f(0.0f,0.0f);
    glVertex2f(coluna, linha - tamanhoSprite);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

//Desenha a textura na tela inteira

void desenhaFullscreen(GLuint tex)
{
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);
    glVertex2f(-1,  1);
    glTexCoord2f(1.0f,1.0f);
    glVertex2f( 1,  1);
    glTexCoord2f(1.0f,0.0f);
    glVertex2f( 1, -1);
    glTexCoord2f(0.0f,0.0f);
    glVertex2f(-1, -1);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

//Desenha textura da pontuação

void desenhaNumero(float x, float y, GLuint tex)
{
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f,1.0f);
    glVertex2f(x + tamanhoPontos, y);
    glTexCoord2f(1.0f,0.0f);
    glVertex2f(x + tamanhoPontos, y - tamanhoPontos);
    glTexCoord2f(0.0f,0.0f);
    glVertex2f(x, y - tamanhoPontos);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void DesenhaCenario()
{
    desenhaFullscreen(CenarioTex);
}

void DesenhaCenarioGameOver()
{
    desenhaFullscreen(CenarioGameOverTex);
}

void DesenhaCenarioWin()
{
    desenhaFullscreen(CenarioVitoriaTex);
}

void DesenhaMenu()
{
    desenhaFullscreen(MenuTex);
}

void ResetaJogo()
{
    DesenhaPontos();
    IniciaNave();
    AlienMorto = 0;
    posicaoAlienX = 0;
    posicaoAlienY = 0;
    int i, j;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 9; j++)
        {
            aliens[i][j] = 1;
        }
    }
    for(i = 0; i < 3; i++)
    {
        tiro_alien[i].status = 0;
    }
    for(i = 0; i < 9; i++)
    {
        menorlinhavivadacoluna[i] = 4;
    }
}

void DesenhaPontos()
{
    int i, n;
    for (i = 0; i < 4; i++)
    {
        double posXPonto = -0.67;
        double posYPonto =  0.964;
        n = PontoS[i] - '0'; // Converte cada char da string para um inteiro equivalente
        desenhaNumero(posXPonto + (i * 0.0550), posYPonto, NumeroTex[n]);
    }
}

void IniciaNave()
{
    nave.xi = -0.05;
    nave.yi = -0.83;
    nave.direcao = 0;
    nave.posicao = 0;
    nave.status = 1;
}

void DesenhaNave()
{
    if(nave.direcao == 1 && nave.x + 0.1f < 0.97)
    {
        nave.posicao += 0.02;
    }
    if(nave.direcao == -1 && nave.xi + nave.posicao > -0.97)
    {
        nave.posicao -= 0.02;
    }

    nave.x = nave.xi + nave.posicao;
    nave.y = nave.yi;

    desenhaSprite(nave.x, nave.y, NaveTex);
}

void DesenhaTiro()
{
    if(tiro.natela == 0 && tiro.aperta == 1)
    {
        tiro.status = 1;
        tiro.pos = 0;
        tiro.natela = 1;
    }

    //Seta a posicao inicial do tiro
    if(tiro.pos == 0)
    {
        tiro.yi = nave.y;
        tiro.xi = nave.x + 0.045f;

        tiro.pos = 1;

        tiro.y = tiro.yi;
        tiro.x = tiro.xi;
    }

    tiro.y += 0.06f; //Movimentacao do tiro

    if(tiro.status == 1)
    {
        glPushMatrix();
        glBegin(GL_QUADS);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(tiro.x, tiro.y);
        glVertex2f((tiro.x + 0.01f),  tiro.y);
        glVertex2f((tiro.x + 0.01f), (tiro.y - 0.1f));
        glVertex2f(tiro.x,   (tiro.y - 0.1f));
        glEnd();
    }
    if(tiro.y > 1.0 )
    {
        //Desativa o tiro caso ele saia da tela
        tiro.natela = 0;
        tiro.status = 0;
        tiro.y = nave.y;
    }
}

void DesenhaET()
{
    // Se o alien está pro lado direito, o desloca pra direita
    if (direcaoAlien == 1)
    {
        posicaoAlienX += 0.004; // Valor irá interferir na velocidade em que o alien anda
    }

    // Se o alien está pro lado esquerdo, o desloca pra esquerda
    if (direcaoAlien == -1)
    {
        posicaoAlienX -= 0.004; // Valor irá interferir na velocidade em que o alien anda
    }

    float i, j;
    for(i = -1; i < 4; i++)
    {
        for(j = -4; j <= 4; j++)
        {
            int linha = i + 1, coluna = j + 4;

            double CoordsXDir =  0.05f + j / 5; // Lado direito  do quadrado
            double CoordsXEsq = -0.05f + j / 5; // Lado esquerdo do quadrado
            double CoordsYSup =  0.25f + i / 5; // Lado superior do quadrado
            double CoordsYInf =  0.15f + i / 5; // Lado inferior do quadrado

            double CoordsXDir_Atual = CoordsXDir + posicaoAlienX; //Nao e o alienX do ben10
            double CoordsXEsq_Atual = CoordsXEsq + posicaoAlienX;
            double CoordsYSup_Atual = CoordsYSup + posicaoAlienY;
            double CoordsYInf_Atual = CoordsYInf + posicaoAlienY;

            if(aliens[linha][coluna] == 1)  // Se o alien estiver vivo
            {
                // Desenha diferentes aliens de acordo com a linha, o alien glender vale mais pontos
                if (i == 3)
                {
                    desenhaSprite(CoordsXEsq_Atual, CoordsYSup_Atual, AlienTex[2]);
                }
                else if (i == 2)
                {
                    desenhaSprite(CoordsXEsq_Atual, CoordsYSup_Atual, AlienTex[1]);
                }
                else
                {
                    desenhaSprite(CoordsXEsq_Atual, CoordsYSup_Atual, AlienTex[0]);
                }

                // Verifica se o alien encostou no lado direito, invertendo sua direção e descendo
                if (CoordsXDir + posicaoAlienX >= 1)
                {
                    direcaoAlien = -1;
                    posicaoAlienY -= 0.01;
                }

                // Verifica se o Alien encostou no lado esquerdo, invertendo sua direção e descendo
                if (CoordsXEsq + posicaoAlienX <= -1)
                {
                    direcaoAlien = 1;
                    posicaoAlienY -= 0.01;
                }

                // Realiza a verificação da colisão
                if (tiro.status == 1 && ((tiro.x >= CoordsXEsq_Atual && tiro.x <= CoordsXDir_Atual && tiro.y <= CoordsYSup_Atual && tiro.y >= CoordsYInf_Atual)
                                         || (tiro.x + 0.02 >= CoordsXEsq_Atual && tiro.x + 0.02 <= CoordsXDir_Atual && tiro.y <= CoordsYSup_Atual && tiro.y >= CoordsYInf_Atual)))
                {
                    tiro.status = 0;
                    // Adição na pontuação, dependendo do Alien
                    if (i == 3) Pontos += 50;
                    else if ( i == 2) Pontos += 30;
                    else Pontos += 10;

                    // Transformação da pontuação inteira em string
                    if (Pontos < 100)
                    {
                        sprintf(PontoS, "00%d", Pontos);
                    }
                    if (Pontos >= 100 && Pontos < 1000)
                    {
                        sprintf(PontoS, "0%d", Pontos);
                    }
                    if (Pontos >= 1000)
                    {
                        sprintf(PontoS, "%d", Pontos);
                    }

                    aliens[linha][coluna] = 0;
                    AlienMorto++;
                }

                // Verifica se o alien chegou ao final do caminho, resultando em game over
                if (CoordsYInf_Atual <= -0.880)
                {
                    nave.status = 0;
                }

                //Pega a menor linha entre os invaders vivos daquela coluna
                if(linha < menorlinhavivadacoluna[coluna])
                {
                    menorlinhavivadacoluna[coluna] = linha;
                }
            }
            // Se o alien morrer, inicia a animação de morte
            else if (aliens[linha][coluna] <= 0 && aliens[linha][coluna] >= -27)
            {
                //Seta 4 como menor linha p/ que o calculo seja feito novamente na coluna do alien morto
                menorlinhavivadacoluna[coluna] = 4;

                if (aliens[linha][coluna] < 0 && aliens[linha][coluna] >= -9)
                {
                    desenhaSprite(CoordsXEsq_Atual, CoordsYSup_Atual, MorteTex[0]);
                }
                else if (aliens[linha][coluna] < -9 && aliens[linha][coluna] >= -18)
                {
                    desenhaSprite(CoordsXEsq_Atual, CoordsYSup_Atual, MorteTex[1]);
                }
                else if (aliens[linha][coluna] < -18 && aliens[linha][coluna] >= -27)
                {
                    desenhaSprite(CoordsXEsq_Atual, CoordsYSup_Atual, MorteTex[2]);
                }
                aliens[linha][coluna]--;
            }
        }
    }
}

void DesenhaTiroA()
{
    int idx;
    int coluna_count;

    idx = 0;
    for(coluna_count = 0; coluna_count < 9; coluna_count++)
    {
        idx++;
        if(idx > 2) idx = 0;

        int linha = menorlinhavivadacoluna[coluna_count];
        double x = -0.05f + (coluna_count - 4)/5.0 + posicaoAlienX;
        double y = 0.15f + (linha - 1)/5.0 + posicaoAlienY;

        tiro_alien[idx].y -= 0.0025f; //Movimentacao do tiro

        if(aliens[linha][coluna_count] == 1)
        {
            //Simula o tiro para os aliens vivos
            int random = rand() % 15000;
            if(random > 50*(idx) && random < 50*(idx+1))
            {
                tiro_alien[idx].aperta = 1;
                random = 20;
            }
            else
            {
                tiro_alien[idx].aperta = 0;
            }


            if(tiro_alien[idx].aperta == 1 && tiro_alien[idx].natela == 0)
            {
                //Caso o tiro esteja fora da tela
                tiro_alien[idx].aperta == 0;
                tiro_alien[idx].status = 1;
                tiro_alien[idx].pos = 0;
                tiro_alien[idx].natela = 1;
            }

            if(tiro_alien[idx].pos == 0)
            {
                //Seta a posicao inicial do tiro
                tiro_alien[idx].xi = x + 0.045f;
                tiro_alien[idx].yi = y;

                tiro_alien[idx].pos = 1;

                tiro_alien[idx].x = tiro_alien[idx].xi;
                tiro_alien[idx].y = tiro_alien[idx].yi;
            }

            if(tiro_alien[idx].status == 1)
            {
                tiro_alien[idx].colisao = 0; //Marca a colisao como ainda nao ocorrida

                glPushMatrix();
                glBegin(GL_QUADS);

                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex2f(tiro_alien[idx].x, tiro_alien[idx].y);
                glVertex2f((tiro_alien[idx].x + 0.01f),  tiro_alien[idx].y);
                glVertex2f((tiro_alien[idx].x + 0.01f), (tiro_alien[idx].y - 0.05f));
                glVertex2f(tiro_alien[idx].x,   (tiro_alien[idx].y - 0.05f));
                glEnd();
            }

            if(tiro_alien[idx].y < -1.0)
            {
                tiro_alien[idx].natela = 0;
                tiro_alien[idx].status = 0;
            }

            int ColisaoTTPontoInfEsq = tiro.x <= tiro_alien[idx].x && tiro_alien[idx].x <= tiro.x + 0.02f && tiro.y - 0.05f <= tiro_alien[idx].y - 0.05f && tiro_alien[idx].y - 0.05f <= tiro.y;
            int ColisaoTTPontoInfDir = tiro.x <= tiro_alien[idx].x + 0.02f && tiro_alien[idx].x + 0.02f <= tiro.x + 0.02f  && tiro.y - 0.05f <= tiro_alien[idx].y - 0.05f && tiro_alien[idx].y - 0.05f <= tiro.y;
            if(tiro_alien[idx].status == 1 && tiro.status == 1 && (ColisaoTTPontoInfEsq || ColisaoTTPontoInfDir))
            {
                tiro_alien[idx].status = 0;
                tiro.status = 0;
                tiro_alien[idx].colisao--;
            }
            //Animacao de colisao
            if(tiro_alien[idx].colisao < 0)
            {
                if(tiro_alien[idx].colisao > -25 && tiro_alien[idx].colisao <= 0)
                {
                    desenhaSprite(tiro_alien[idx].x - 0.045f, tiro_alien[idx].y, ColisaoTex);
                }
                tiro_alien[idx].colisao--;
            }

            //Colisao de tiro da nave
            int ColisaoTAPontoInfEsq = nave.x <= tiro_alien[idx].x && tiro_alien[idx].x <= nave.x + 0.1f && nave.y - 0.05f <= tiro_alien[idx].y - 0.05f && tiro_alien[idx].y - 0.05f <= nave.y;
            int ColisaoTAPontoInfDir = nave.x <= tiro_alien[idx].x + 0.02f && tiro_alien[idx].x + 0.02f <= nave.x + 0.1f  && nave.y - 0.05f <= tiro_alien[idx].y - 0.05f && tiro_alien[idx].y - 0.05f <= nave.y;
            int ColisaoTAPontoSupEsq = nave.x <= tiro_alien[idx].x && tiro_alien[idx].x <= nave.x + 0.1f && nave.y - 0.05f <= tiro_alien[idx].y && tiro_alien[idx].y <= nave.y;
            int ColisaoTAPontoSupDir = nave.x <= tiro_alien[idx].x + 0.02f && tiro_alien[idx].x + 0.02f <= nave.x + 0.1f  && nave.y - 0.05f <= tiro_alien[idx].y && tiro_alien[idx].y <= nave.y;
            if(tiro_alien[idx].status == 1 && nave.status == 1 && (ColisaoTAPontoInfDir || ColisaoTAPontoInfEsq || ColisaoTAPontoSupEsq || ColisaoTAPontoSupDir))
            {
                tiro_alien[idx].status = 0;
                nave.status = 0;
            }
        }
    }
}
