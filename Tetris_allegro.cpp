#include "Tetris_allegro.h"
#include <allegro.h>
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


int mapa[13][22];
int puntos_nivel[11] = {2,5,15,20,25,30,35,40,45,59,200};
int velocidad_de_nivel[12] = {40,35,30,25,22,18,16,13,12,10,9,5};

void mostrar_bloques(BITMAP *buffer, BITMAP *img_b, int xb, int yb,int color, int tipo){
    blit(img_b,buffer,tipo*SBLOCK, color*SBLOCK, xb*SBLOCK, yb*SBLOCK, SBLOCK,SBLOCK);
}


Pieza::Pieza(Bloque _b_prin, Bloque _bls[3], int _color_p){
    b_prin = _b_prin;
    for(int i=0; i<3; i++)
        bls[i] = _bls[i];
    color_p = _color_p;

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

void Pieza::rotar(int rotacion){
    int aux;
    if (rotacion % 2){
    for(int i=0; i<3; i++){
        aux = bls[i].x;
        bls[i].x = bls[i].y;
        bls[i].y = aux;
        bls[i].x *= -1;

    }
    }
    else{
        for(int i=0; i<3; i++){
        aux = bls[i].x;
        bls[i].x = bls[i].y;
        bls[i].y = aux;
        bls[i].y *= -1;

    }
}
}
bool Pieza::fila_llena(int fila){
    for (int i=1; i<11; i++){
        if(mapa[fila][i] == 9)
            return false;
    }
    return true;
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

void mostrar_numero(BITMAP* buffer, BITMAP* img_num, int numero, int xnum, int ynum){
    int uDigito, pos = 0;
    if(numero !=0){
    while(numero != 0){
        uDigito = numero % 10;
        blit(img_num, buffer, uDigito*30, 0, xnum-pos, ynum, 30,60);
        numero /= 10;
        pos += 38;
        }
    }
    else{
        blit(img_num, buffer,0,0,xnum,ynum,30,60);
    }
}

void mostrar_datos(BITMAP* buffer, BITMAP* img_texto, BITMAP* img_num, int puntos){
    blit(img_texto, buffer, 0, 0, 320, 50, 146, 34);
    blit(img_texto, buffer, 0, 34, 320, 210, 119, 34);
    mostrar_numero(buffer, img_num, puntos, 430, 260);
    blit(img_texto, buffer, 0, 68, 320, 340, 82, 34);

}

void mostrar_nivel(int nivel, BITMAP *buffer,BITMAP *dibu,BITMAP *ardiles,BITMAP *romero,BITMAP *tagliafico,
                   BITMAP *montiel,BITMAP *tatabrown,BITMAP *maradona,BITMAP *depaul,BITMAP *huevo,BITMAP *arania,
                   BITMAP *messi,BITMAP *dimaria){
        if(nivel == 0){
            blit(dibu, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);
        } else if (nivel == 1)
        {
            blit(ardiles, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
        else if (nivel == 2)
        {
            blit(romero, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
        else if (nivel == 3)
        {
            blit(tagliafico, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
        else if (nivel == 4)
        {
            blit(montiel, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
        else if (nivel == 5)
        {
            blit(tatabrown, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
        else if (nivel == 6)
        {
            blit(maradona, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
        else if (nivel == 7)
        {
            blit(depaul, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
        else if (nivel == 8)
        {
            blit(huevo, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
        else if (nivel == 9)
        {
            blit(arania, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
        else if (nivel == 10)
        {
            blit(messi, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
        else
        {
            blit(dimaria, buffer , 0, 0, 330, 390, 120, 200 );
            blit(buffer,screen, 0, 0, 0, 0, ANCHO , ALTO);

        }
    }



void eliminar_fila(int fila){
    for(int i=fila; i>0; i--)
        for(int j=1; j<11; j++)
            mapa[i][j] = mapa[i-1][j];
}

void  portada(BITMAP *portada2){
    int parpadeo = 0;
    while(!key[KEY_ENTER])
    {
        if(parpadeo < 80)
        {
            blit(portada2, screen, 0, 0, 0, 0,ANCHO,ALTO);

        }
        else
        {
            blit(portada2, screen, 500, 0, 0, 0,ANCHO,ALTO);
        }
        rest(5);
        if(++parpadeo == 150){
            parpadeo = 0;
        }
    }
    clear_to_color(screen,0x000000);
}

int aumentar_nivel(int puntos, int nivel){
    if(puntos > puntos_nivel[nivel])
        nivel++;
   return nivel;
}

int velocidad_nivel(int nivel)
{
    return velocidad_de_nivel[nivel];
}

