#include "tetris_allegro.h"
#include <allegro.h>
// Tama�os
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


int mapa[13][22];

void mostrar_bloques(BITMAP *buffer, BITMAP *img_b, int xb, int yb,int color, int tipo){
    blit(img_b,buffer,tipo*SBLOCK, color*SBLOCK, xb*SBLOCK, yb*SBLOCK, SBLOCK,SBLOCK);
}


Pieza::Pieza(Bloque _b_prin, Bloque _bls[3], int _color_p){
    b_prin = _b_prin;
    for(int i=0; i<3; i++){
    bls[i] = _bls[i];
    color_p = _color_p;
    };
}

void Pieza::mostrar_pieza(BITMAP* buffer, BITMAP* img_b){
    int xbls, ybls;
    b_prin.tipo = SMEDIO;
    mostrar_bloques(buffer,img_b, b_prin.x, b_prin.y, color_p, b_prin.tipo);
    for(int i=0; i<3; i++){
    xbls = b_prin.x + bls[i].x;
    ybls = b_prin.y + bls[i].y;
    if ((xbls >= b_prin.x) && (ybls >= b_prin.y)){
        bls[i].tipo = SOMBRA;
    }
    else
    {
        bls[i].tipo = NORMAL;
    }
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

bool Pieza::colision_derecha(){
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

void Pieza::insertar_mapa(){
    int xbls, ybls; //variables para almacenar el resto de los bloques
    mapa[b_prin.y][b_prin.x] = (b_prin.tipo*10) + color_p;
    for(int i=0; i<3; i++){
        xbls = b_prin.x + bls[i].x;
        ybls = b_prin.y + bls[i].y;
        mapa[ybls][xbls] = (bls[i].tipo*10 + color_p);
    }
}

void mostrar_muros(BITMAP *buffer, BITMAP *muroH, BITMAP *muroV){
blit(muroV,buffer,0,0,0,0,25,500);
blit(muroV,buffer,0,0,275,0,25,500);
blit(muroH,buffer,0,0,0,500,300,25);
}

void limpiar_mapa(){
    for(int i=0; i<21; i++){
        for(int j=0; j<12; j++){
            if (j==0 || j==11 || i==20)
                mapa[i][j] = 8;
            else
                mapa[i][j]= 9;
        }
    }
}

void mostrar_mapa(BITMAP* buffer, BITMAP* img_b){
    int t, c; //variable para tipo de bloque y color que queremos mostrar
    for(int i=0; i<20; i++){
        for(int j=1; j<11; j++){
            if(mapa[i][j] != 9){
                t = mapa[i][j] / 10; //asi se extrae el primer digito que es igual al tipo y el sgundo tipo es igual al color
                c = mapa[i][j] % 10; //asi extraemos el ultimo digito o sea el color
                mostrar_bloques(buffer, img_b, j, i, c, t);
            }
        }
    }

}