#include <iostream>
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tetris_allegro.h"

using namespace std;

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
    BITMAP *elimn = load_bitmap("Imagenes/elimina_piezas.bmp", NULL);

    //INTEGERS
    int vcaida = 7; // nos ayuda a controlas la velocidad de la caida
    int aux =0;
    int pb = 0;
    int aleatorio , fila , cfila, fin;

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

    //Generar piezas aleatoriamente
    Pieza pAc(b_prin,bl1,0);
    aleatorio = 1 + rand() % 6;
    if(aleatorio == 1) pAc.setBls(bl1), pAc.setColor(NARANJA);
    else if(aleatorio == 2) pAc.setBls(bl2), pAc.setColor(VERDE);
    else if(aleatorio == 3) pAc.setBls(bl3), pAc.setColor(AMARILLO);
    else if(aleatorio == 4) pAc.setBls(bl4), pAc.setColor(MORADO);
    else if(aleatorio == 5) pAc.setBls(bl5), pAc.setColor(ROJO);
    else if(aleatorio == 6) pAc.setBls(bl6), pAc.setColor(AZUL);


    //Mostrar pieza
    //Pieza pAc(b_prin,bl1, VERDE);

    limpiar_mapa();
    while (!key[KEY_ESC]){



        clear_to_color(buffer, 0x000000);
        mostrar_muros(buffer, muroH, muroV);
        mostrar_mapa(buffer,img_b);

        if(pAc.colision_abajo()) colb = true;
        if(pAc.colision_derecha()) cold = true;
        if(pAc.colision_izquierda()) coli = true;

        // Bajar pieza
        if(pb++ >= vcaida && !colb){
            pb = 0;
            pAc.incrY(1);
        }

        // Cuando hace una colison abajo se genere otra pieza aleatoria
        if(colb){
            pAc.insertar_mapa();
            fila = pAc.getY() + 2;
            while(fila > 19)
                fila--;
            fin = fila - 4;
            cfila = fila;
            while(fila >= fin){
              if(pAc.fila_llena(fila)){
                for(int i=1; i<11; i++)
                    blit(elimn,buffer,0,0,i*SBLOCK, fila*SBLOCK, 25, 25);
                blit(buffer,screen,0,0,0,0,ANCHO,ALTO);
                for(int i=1; i<11; i++){
                    blit(elimn, buffer, 25, 0, i*SBLOCK, fila*SBLOCK,25,25);
                    blit(buffer, screen, 0, 0, 0, 0, ANCHO, ALTO);
                    blit(elimn, buffer, 50, 0, i*SBLOCK, fila*SBLOCK,25,25);
                    rest(8);
                }
              }
              fila--;
            }
            fila= cfila;
            while( fila >= fin){
                if(pAc.fila_llena(fila))
                    eliminar_fila(fila);
                else
                    fila--;
            }
            b_prin.x =5, b_prin.y = 2;
            pAc.setBPrin(b_prin);
            aleatorio = 1 + rand() % 6;
            if(aleatorio == 1) pAc.setBls(bl1), pAc.setColor(NARANJA);
            else if(aleatorio == 2) pAc.setBls(bl2), pAc.setColor(VERDE);
            else if(aleatorio == 3) pAc.setBls(bl3), pAc.setColor(AMARILLO);
            else if(aleatorio == 4) pAc.setBls(bl4), pAc.setColor(MORADO);
            else if(aleatorio == 5) pAc.setBls(bl5), pAc.setColor(ROJO);
            else if(aleatorio == 6) pAc.setBls(bl6), pAc.setColor(AZUL);
            colb = false;
            rest(100);
        }

        Pieza pAux = pAc;
          //Detectando el teclado
        if(key[KEY_RIGHT] && !cold)
            pAc.incrX(1);
        if(key[KEY_LEFT] && !coli)
            pAc.incrX(-1);
        if(key[KEY_UP])
            pAc.rotar();
            pAc.incrX(1);
            if(pAc.colision_izquierda())
                pAc = pAux;
            pAc.incrX(-2);
            if(pAc.colision_derecha())
                pAc = pAux;
            pAc.incrX(1);
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

        rest(40);
    }


    return 0;
}
END_OF_MAIN();
