#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/src/algif.h"
#include <stdbool.h>

/*
   [Hero object]
*/

typedef enum HeroType
{
    hold = 0,
    run,
    atk,
    dodge
} HeroType;
typedef struct _Hero
{
    int x, y;
    int width, height;              // the width and height of image
    int life,full_life;
    bool dir;                       // true: face to right, false: face to left
    int state;                      // the state of Hero
    ALGIF_ANIMATION *gif_status[4]; // gif for each state. 0: stop, 1: move, 2:attack
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    int anime;      // counting the time of animation
    int anime_time; // indicate how long the animation
    bool new_proj;
    Shape *hitbox; // the hitbox of object
} Hero;
Elements *New_Hero(int label); 
void _Hero_update_position(Elements *self, int dx, int dy);
void Hero_update(Elements *self);
void Hero_interact(Elements *self, Elements *target);
void Hero_draw(Elements *self);
void Hero_destory(Elements *self);

#endif