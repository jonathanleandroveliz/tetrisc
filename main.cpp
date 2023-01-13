#include <iostream>
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Tamaños
#define ANCHO 500
#define ALTO 525
#define SBLOCK 25
// Colores
#define NARANJA 0
#define AZUL 1
#define MORADO 2
#define ROJO 3
#define AMARILLO 4
#define VERDE 5
//Tipo de bloques
#define NORMAL 0
#define SMEDIO 1
#define SOMBRA 2

using namespace std;

int mapa[13][22];

void mostrar_bloques(BITMAP *buffer, BITMAP *img_b, int xb, int yb,int color, int tipo){
 blit(img_b,buffer,tipo*SBLOCK, color*SBLOCK, xb*SBLOCK, yb*SBLOCK, SBLOCK,SBLOCK);
}

struct Bloque{
int x, y, tipo;
};

class Pieza{
private:
    Bloque b_prin;
    Bloque bls[3];
    int color_p;
public:
    Pieza(Bloque _b_prin, Bloque _bls[3], int _color_p);
    void mostrar_pieza(BITMAP *buffer, BITMAP *img_b);
    void incrX(int incr){ b_prin.x += incr; }
    void incrY(int incr){ b_prin.y += incr; }
    bool colision_abajo();
    bool colision_derec();
    bool colision_izquierda();
};

Pieza::Pieza(Bloque _b_prin, Bloque _bls[3], int _color_p){
    b_prin = _b_prin;
    for (int i=0; i<3 ; i++)
        bls[i] = _bls[i];
    color_p = _color_p;
}

void Pieza::mostrar_pieza(BITMAP* buffer, BITMAP* img_b){
    int xbls, ybls;
    b_prin.tipo = SMEDIO;
    mostrar_bloques(buffer, img_b, b_prin.x, b_prin.y, color_p, b_prin.tipo);
    for(int i=0; i<3; i++){
        xbls = b_prin.x + bls[i].x;
        ybls = b_prin.y + bls[i].y;
        if((xbls >= b_prin.x && ybls >= b_prin.y ))
            bls[i].tipo = SOMBRA;
        else
            bls[i].tipo = NORMAL;
        mostrar_bloques(buffer, img_b, xbls, ybls, color_p, bls[i].tipo);
    }
}

bool Pieza::colision_abajo(){
    if (mapa[b_prin.y+1][b_prin.x] != 9)
        return true;
    int xbls , ybls;
    for(int i=0; i<3; i++){
        xbls = b_prin.x + bls[i].x;
        ybls = b_prin.y + bls[i].y;
        if(mapa[ybls+1][xbls] != 9)
            return true;
    }
    return false;
}

bool Pieza::colision_derec(){
    if (mapa[b_prin.y][b_prin.x+1] != 9)
        return true;
    int xbls , ybls;
    for(int i=0; i<3; i++){
        xbls = b_prin.x + bls[i].x;
        ybls = b_prin.y + bls[i].y;
        if(mapa[ybls][xbls+1] != 9)
            return true;
    }
    return false;
}

bool Pieza::colision_izquierda(){
    if (mapa[b_prin.y][b_prin.x-1] != 9)
        return true;
    int xbls , ybls;
    for(int i=0; i<3; i++){
        xbls = b_prin.x + bls[i].x;
        ybls = b_prin.y + bls[i].y;
        if(mapa[ybls][xbls-1] != 9)
            return true;
    }
    return false;
}

void mostrar_muros(BITMAP *buffer, BITMAP *muroH, BITMAP *muroV){
blit(muroV,buffer,0,0,0,0,25,500);
blit(muroV,buffer,0,0,275,0,25,500);
blit(muroH,buffer,0,0,0,500,300,25);
}

void limpiar_mapa(){
    for(int i=0 ; i<21 ; i++){
        for(int j=0 ; j<12; j++){
            if(j == 0 || j == 11 || i ==20)
                mapa[i][j]= 8;
            else
                mapa[i][j]= 9;
        }
    }
}

int main()
{
    //Inicializacion de allegro
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,ANCHO,ALTO,0,0);

    // BITMAPS
    BITMAP *buffer = create_bitmap(ANCHO,ALTO);
    BITMAP *muroH = load_bitmap("Imagenes/muro_horizontal.bmp",NULL);
    BITMAP *muroV = load_bitmap("Imagenes/muro_vertical.bmp", NULL);
    BITMAP *img_b = load_bitmap("Imagenes/piezas.bmp", NULL);

    //INTEGERS
    int vcaida = 7; // nos ayuda a controlas la velocidad de la caida
    int aux =0;
    int pb = 0;
    int aleatorio;

    // Booleanos para detectar las colisiones
    bool colb = false;
    bool coli = false;
    bool cold = false;

    // PIEZAS
    Bloque b_prin = { 5, 2, NORMAL};
    Bloque bl1[3] = { { 0, -1, NORMAL } , { 1, -1, NORMAL } , { 0, 1, NORMAL } };
    Bloque bl2[3] = { { 0, -1, NORMAL } , { 1, 0, NORMAL } , { -1, 0, NORMAL } };
    Bloque bl3[3] = { { 0, -1, NORMAL } , { -1, -1, NORMAL } , { -1, 0, NORMAL } };
    Bloque bl4[3] = { { 0, -1, NORMAL } , { 0, 1, NORMAL } , { 0, 2, NORMAL } };
    Bloque bl5[3] = { { 0, -1, NORMAL } , { 0, 1, NORMAL } , { 1, 1, NORMAL } };
    Bloque bl6[3] = { { -1, -1, NORMAL } , { 0, -1, NORMAL } , { 1, 0, NORMAL } };

    Pieza pAc(b_prin,bl1, VERDE);

    limpiar_mapa();
    while (!key[KEY_ESC]){

        clear_to_color(buffer, 0x000000);
        mostrar_muros(buffer, muroH, muroV);

        if(pAc.colision_abajo()) colb = true;
        if(pAc.colision_derec()) cold = true;
        if(pAc.colision_izquierda()) coli = true;

        // Bajar pieza
        if(pb++ >= vcaida && !colb){
            pb = 0;
            pAc.incrY(1);
        }

          //Detectando el teclado
        if(key[KEY_RIGHT] && !cold)
            pAc.incrX(1);
        if(key[KEY_LEFT] && !coli)
            pAc.incrX(-1);
        if(key[KEY_DOWN])
            vcaida = 0;

        if(++aux >= 7){
            vcaida = 7;
            aux = 0;
        }


        pAc.mostrar_pieza(buffer,img_b);
        /*mostrar_bloques(buffer, img_b, 3, 3, VERDE, SMEDIO);*/
        blit(buffer,screen,0,0,0,0,ANCHO,ALTO);

        //Reiniciar variables booleanas
        cold = false;
        coli = false;

        rest(30);
    }


    return 0;
}
END_OF_MAIN();
