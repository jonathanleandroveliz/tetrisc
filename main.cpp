#include <iostream>
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
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

void mostrar_muros(BITMAP *buffer, BITMAP *muroH, BITMAP *muroV){
blit(muroV,buffer,0,0,0,0,25,500);
blit(muroV,buffer,0,0,275,0,25,500);
blit(muroH,buffer,0,0,0,500,300,25);
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

    // PIEZAS
    Bloque b_prin = { 5, 2, NORMAL};
    Bloque bl1[3] = { { 0, -1, NORMAL } , { 1, -1, NORMAL } , { 0, 1, NORMAL } };
    Bloque bl2[3] = { { 0, -1, NORMAL } , { 1, 0, NORMAL } , { -1, 0, NORMAL } };
    Bloque bl3[3] = { { 0, -1, NORMAL } , { -1, -1, NORMAL } , { -1, 0, NORMAL } };
    Bloque bl4[3] = { { 0, -1, NORMAL } , { 0, 1, NORMAL } , { 0, 2, NORMAL } };
    Bloque bl5[3] = { { 0, -1, NORMAL } , { 0, 1, NORMAL } , { 1, 1, NORMAL } };
    Bloque bl6[3] = { { -1, -1, NORMAL } , { 0, -1, NORMAL } , { 1, 0, NORMAL } };

    Pieza pAc(b_prin,bl6, VERDE);

    while (!key[KEY_ESC]){
        clear_to_color(buffer, 0x000000);
        mostrar_muros(buffer, muroH, muroV);

        pAc.mostrar_pieza(buffer,img_b);
        /*mostrar_bloques(buffer, img_b, 3, 3, VERDE, SMEDIO);*/
        blit(buffer,screen,0,0,0,0,ANCHO,ALTO);
        rest(5);
    }


    return 0;
}
END_OF_MAIN();
