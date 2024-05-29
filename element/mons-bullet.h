#ifndef Monsbullet_H_INCLUDED
#define Monsbullet_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [Projectile object]
*/
typedef struct _Monsbullet
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    int v;             // the velocity of projectile
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
} Monsbullet;
Elements *New_Monsbullet(int label, int x, int y, int v);
void Monsbullet_update(Elements *self);
void Monsbullet_interact(Elements *self, Elements *tar);
void Monsbullet_draw(Elements *self);
void Monsbullet_destory(Elements *self);
void _Monsbullet_update_position(Elements *self, int dx, int dy);
#endif
