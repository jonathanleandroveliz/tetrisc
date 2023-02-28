#ifndef TETRIS_ALLEGRO_H_INCLUDED
#define TETRIS_ALLEGRO_H_INCLUDED

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



void mostrar_bloques(BITMAP *buffer, BITMAP *img_b, int xb, int yb,int color, int tipo);

struct Bloque{
    int x , y, tipo;
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
        bool colision_derecha();
        bool colision_izquierda();
        bool fila_llena(int fila);
        void insertar_mapa();
        int getY(){ return b_prin.y; }
        int getX(){ return b_prin.x; }
        void setBPrin(Bloque newB_prin){ b_prin = newB_prin; }
        void setBls(Bloque newBls[3]){ for(int i=0; i<3; i++) bls[i] = newBls[i]; }
        void setColor(int newColor){ color_p = newColor ; }
        void rotar(int rotacion);
};

void mostrar_muros(BITMAP *buffer, BITMAP *muroH, BITMAP *muroV);
void mostrar_numero(BITMAP *buffer, BITMAP *img_num, int numero, int xnum, int ynum);
void mostrar_datos(BITMAP *buffer, BITMAP *img_texto, BITMAP *img_num, int puntos);
void mostrar_nivel(int nivel, BITMAP *buffer,BITMAP *dibu,BITMAP *ardiles,BITMAP *romero,BITMAP *tagliafico,
                   BITMAP *montiel,BITMAP *tatabrown,BITMAP *maradona,BITMAP *depaul,BITMAP *huevo,BITMAP *arania,
                   BITMAP *messi,BITMAP *dimaria);
void limpiar_mapa();
void mostrar_mapa(BITMAP *buffer, BITMAP *img_b);
void eliminar_fila(int fila);
void portada(BITMAP *portada2);
int aumentar_nivel(int puntos, int nivel);
int velocidad_nivel(int nivel);


#endif // TETRIS_ALLEGRO_H_INCLUDED
