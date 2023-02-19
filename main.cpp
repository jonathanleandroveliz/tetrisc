#include <iostream>
#include <allegro.h>
#include <time.h>
#include "Tetris_allegro.h"

using namespace std;

int main()
{
    //Inicializacion de allegro
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,ANCHO,ALTO,0,0);

    //Inicializacion de audio
    if(install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT,NULL) != 0){
        allegro_message("ERROR: inicializando sistema de sonido\n%s\n", allegro_error);
        return 1;
    }
    set_volume(70, 70);


    // BITMAPS
    BITMAP *buffer = create_bitmap(ANCHO,ALTO);
    BITMAP *muroH = load_bitmap("Imagenes/muro_horizontal.bmp",NULL);
    BITMAP *muroV = load_bitmap("Imagenes/muro_vertical.bmp", NULL);
    BITMAP *img_b = load_bitmap("Imagenes/piezas.bmp", NULL);
    BITMAP *elimn = load_bitmap("Imagenes/elimina_piezas.bmp", NULL);
    BITMAP *img_texto = load_bitmap("Imagenes/textos.bmp", NULL);
    BITMAP *img_num = load_bitmap("Imagenes/numeros.bmp", NULL);
    BITMAP *goBmp = load_bitmap("Imagenes/GameOver.bmp", NULL);
    BITMAP *portada2 = load_bitmap("Imagenes/bmportada.bmp", NULL);
    //JUGADORES
    BITMAP *dibu = load_bitmap("Imagenes/original2.bmp", NULL);
    BITMAP *ardiles = load_bitmap("Imagenes/ardiles1.bmp", NULL);
    BITMAP *romero = load_bitmap("Imagenes/numero2.bmp", NULL);
    BITMAP *tagliafico = load_bitmap("Imagenes/numero3.bmp", NULL);
    BITMAP *montiel = load_bitmap("Imagenes/numero4.bmp", NULL);
    BITMAP *tatabrown = load_bitmap("Imagenes/numero5.bmp", NULL);
    BITMAP *maradona = load_bitmap("Imagenes/numero6.bmp", NULL);
    BITMAP *depaul = load_bitmap("Imagenes/numero7.bmp", NULL);
    BITMAP *huevo = load_bitmap("Imagenes/numero8.bmp", NULL);
    BITMAP *arania = load_bitmap("Imagenes/numero9.bmp", NULL);
    BITMAP *messi = load_bitmap("Imagenes/numero10.bmp", NULL);
    BITMAP *dimaria = load_bitmap("Imagenes/numero11.bmp", NULL);

    portada(portada2);
    delete portada2;

    // SAMPLES
    SAMPLE *col = load_sample("Sonidos/colocar_pieza.wav");
    SAMPLE *eli = load_sample("Sonidos/elimina_fila.wav");
    SAMPLE *rot = load_sample("Sonidos/rotar_pieza.wav");


    MIDI *musica_fondo = load_midi("Sonidos/fondo.mid");
    play_midi(musica_fondo, 1);


    salto:
    //INTEGERS
    int vcaida = 7; // nos ayuda a controlas la velocidad de la caida
    int aux =0;
    int pb = 0;
    int aleatorio , fila , cfila, fin;
    int puntos = 0, nivel = 0;

    // Booleanos para detectar las colisiones
    bool colb = false;
    bool coli = false;
    bool cold = false;
    bool gameOver = false, reproducir;

    //bool winner = false;


    // PIEZAS
    Bloque b_prin = { 5, 2, NORMAL};
    Bloque b_prin_sig = { 15, 5, NORMAL};
    Bloque bl1[3] = { { 0, -1, NORMAL } , { 1, -1, NORMAL } , { 0, 1, NORMAL } };
    Bloque bl2[3] = { { 0, -1, NORMAL } , { 1, 0, NORMAL } , { -1, 0, NORMAL } };
    Bloque bl3[3] = { { 0, -1, NORMAL } , { -1, -1, NORMAL } , { -1, 0, NORMAL } };
    Bloque bl4[3] = { { 0, -1, NORMAL } , { 0, 1, NORMAL } , { 0, 2, NORMAL } };
    Bloque bl5[3] = { { 0, -1, NORMAL } , { 0, 1, NORMAL } , { 1, 1, NORMAL } };
    Bloque bl6[3] = { { -1, -1, NORMAL } , { 0, -1, NORMAL } , { 1, 0, NORMAL } };

    //Generar piezas aleatoriamente
    Pieza pAc(b_prin,bl1,0);
    srand(time(NULL));
    aleatorio = 1 + rand() % 6;
    if(aleatorio == 1) pAc.setBls(bl1), pAc.setColor(NARANJA);
    else if(aleatorio == 2) pAc.setBls(bl2), pAc.setColor(VERDE);
    else if(aleatorio == 3) pAc.setBls(bl3), pAc.setColor(AMARILLO);
    else if(aleatorio == 4) pAc.setBls(bl4), pAc.setColor(MORADO);
    else if(aleatorio == 5) pAc.setBls(bl5), pAc.setColor(ROJO);
    else if(aleatorio == 6) pAc.setBls(bl6), pAc.setColor(AZUL);

    Pieza pSig(b_prin_sig, bl1, 0);
    aleatorio = 1 + rand() % 6;
    if(aleatorio ==  1) pSig.setBls(bl1), pSig.setColor(NARANJA);
    else if(aleatorio == 2) pSig.setBls(bl2), pSig.setColor(VERDE);
    else if(aleatorio == 3) pSig.setBls(bl3), pSig.setColor(AMARILLO);
    else if(aleatorio == 4) pSig.setBls(bl4), pSig.setColor(MORADO);
    else if(aleatorio == 5) pSig.setBls(bl5), pSig.setColor(ROJO);
    else if(aleatorio == 6) pSig.setBls(bl6), pSig.setColor(AZUL);

    //Mostrar pieza
    //Pieza pAc(b_prin,bl1, VERDE);

    limpiar_mapa();
    while (!key[KEY_ESC] && gameOver == false){

        clear_to_color(buffer, 0x000000);
        mostrar_muros(buffer, muroH, muroV);
        mostrar_datos(buffer, img_texto, img_num, puntos);
        mostrar_mapa(buffer,img_b);
        mostrar_nivel(nivel,buffer,dibu,ardiles,romero,tagliafico,montiel,tatabrown,maradona,depaul,huevo,
                      arania,messi,dimaria);

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
            play_sample(col, 100, 150, 1000, 0);
            pAc.insertar_mapa();
            fila = pAc.getY() + 2;
            while(fila > 19)
                fila--;
            fin = fila - 4;
            cfila = fila;
            while(fila >= fin){ //while de la animacion
              if(pAc.fila_llena(fila)){
                    play_sample(eli, 100, 150, 1000, 0);
                for(int i=1; i<11; i++)
                    blit(elimn,buffer,0,0,i*SBLOCK, fila*SBLOCK, 25, 25);
                blit(buffer,screen,0,0,0,0,ANCHO,ALTO);
                for(int i=1; i<11; i++){
                    blit(elimn, buffer, 25, 0, i*SBLOCK, fila*SBLOCK,25,25);
                    blit(buffer, screen, 0, 0, 0, 0, ANCHO, ALTO);
                    blit(elimn, buffer, 50, 0, i*SBLOCK, fila*SBLOCK,25,25);
                    rest(8);
                }
                puntos++;

                if(puntos % 2 == 0){
                    nivel++;
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
            if (pAc.getY() <3){
                gameOver = true;
                break;
            }
            b_prin.x =5, b_prin.y = 2;
            pAc = pSig;
            pAc.setBPrin(b_prin);
            aleatorio = 1 + rand() % 6;
            if(aleatorio == 1) pSig.setBls(bl1), pSig.setColor(NARANJA);
            else if(aleatorio == 2) pSig.setBls(bl2), pSig.setColor(VERDE);
            else if(aleatorio == 3) pSig.setBls(bl3), pSig.setColor(AMARILLO);
            else if(aleatorio == 4) pSig.setBls(bl4), pSig.setColor(MORADO);
            else if(aleatorio == 5) pSig.setBls(bl5), pSig.setColor(ROJO);
            else if(aleatorio == 6) pSig.setBls(bl6), pSig.setColor(AZUL);
            colb = false;
            rest(100);
        }

        Pieza pAux = pAc;
          //Detectando el teclado
        if(key[KEY_RIGHT] && !cold)
            pAc.incrX(1);
        if(key[KEY_LEFT] && !coli)
            pAc.incrX(-1);
        if(key[KEY_UP]){
            reproducir = true;
            pAc.rotar();
            pAc.incrX(1);
            if(pAc.colision_izquierda()){
                pAc = pAux;
                reproducir = false;
            }
            pAc.incrX(-2);
            if(pAc.colision_derecha()){
                pAc = pAux;
                reproducir = false;
            }
            pAc.incrX(1);
            if(reproducir)
            {
                play_sample(rot, 100 , 150, 1000, 0 );
            }
        }
        if(key[KEY_DOWN])
            vcaida = 0;

        if(++aux >= 7){
            vcaida = 7;
            aux = 0;
        }


        if(key[KEY_F1])
        {
            nivel++;
        }
         if(key[KEY_F2])
        {
            nivel--;
        }


        pAc.mostrar_pieza(buffer,img_b);
        pSig.mostrar_pieza(buffer,img_b);
        /*mostrar_bloques(buffer, img_b, 3, 3, VERDE, SMEDIO);*/
        blit(buffer,screen,0,0,0,0,ANCHO,ALTO);

        //Reiniciar variables booleanas
        cold = false;
        coli = false;

        rest(50);
    }


    if(gameOver){
        while(!key[KEY_ESC]){
            blit(goBmp, buffer, 0, 0, 29, 175, 243, 108);
            if(key[KEY_ENTER]){
                goto salto;
            }
            blit(buffer, screen, 0, 0, 0, 0, ANCHO, ALTO);
            rest(5);
        }
    }


    return 0;
}
END_OF_MAIN();
