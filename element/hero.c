#include "hero.h"
#include "../scene/sceneManager.h"
#include "projectile.h"

#include "monster.h"
#include "../shapes/Rectangle.h"
#include "../algif5/src/algif.h"
#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
/*
   [Hero function]
*/
Elements *New_Hero(int label)
{
    printf("i");
    Hero *pDerivedObj = (Hero *)malloc(sizeof(Hero));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load Hero images
    char state_string[4][10] = {"stop", "move", "attack","dodge"};
    for (int i = 0; i < 4; i++)
    {
        //printf("HHHH");
        char buffer[50];
        sprintf(buffer, "assets/image/hero/%s.gif", state_string[i]);
        //sprintf(buffer, "assets/image/chara_%s.gif", state_string[i]);
        pDerivedObj->gif_status[i] = algif_new_gif(buffer, -1);
    }

    //沒動
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/atk_sound.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());

    // initial the geometric information of Hero
    pDerivedObj->width = pDerivedObj->gif_status[0]->width;
    pDerivedObj->height = pDerivedObj->gif_status[0]->height;
    pDerivedObj->x = 300;
    pDerivedObj->y = HEIGHT - pDerivedObj->height - 60;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->dir = true; // true: face to right, false: face to left
    pDerivedObj->life=pDerivedObj->full_life=100;
    // initial the animation component
    pDerivedObj->state = STOP;
    pDerivedObj->new_proj = false;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Hero_draw;
    pObj->Update = Hero_update;
    pObj->Interact = Hero_interact;
    pObj->Destroy = Hero_destory;
    //printf("i");
    return pObj;
}
void Hero_update(Elements *self)
{
    // use the idea of finite state machine to deal with different state
    Hero *chara = ((Hero *)(self->pDerivedObj));
    if(chara->state == dodge){
        if(chara->dir)_Hero_update_position(self, -5, 0);
        else _Hero_update_position(self, 5, 0);
        chara->state = hold;
    }
    if (chara->state == hold)
    {
        if(key_state[ALLEGRO_KEY_L]){
            chara->state = dodge;
        }
        else if (key_state[ALLEGRO_KEY_J])
        {
            chara->state = atk;
        }
        else if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            chara->state = run;
        }
        else if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            chara->state = run;
        }
        else
        {
            chara->state = hold;
        }
    }
    else if (chara->state == run)
    {
        if(key_state[ALLEGRO_KEY_L]){
            chara->state = dodge;
        }
        else if (key_state[ALLEGRO_KEY_J])
        {
            chara->state = atk;
        }
        else if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            _Hero_update_position(self, -5, 0);
            chara->state = run;
        }
        else if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            _Hero_update_position(self, 5, 0);
            chara->state = run;
        }
        if (chara->gif_status[chara->state]->done)
            chara->state = hold;
    }
    else if (chara->state == atk)
    {
        if (chara->gif_status[chara->state]->done)
        {
            chara->state = hold;
            chara->new_proj = false;
        }
        // if (chara->gif_status[atk]->display_index == 2 && chara->new_proj == false)
        // {
        //     Elements *pro;
        //     if (chara->dir)
        //     {
        //         pro = New_Projectile(Projectile_L,
        //                              chara->x + chara->width - 100,
        //                              chara->y + 10,
        //                              5);
        //     }
        //     else
        //     {
        //         pro = New_Projectile(Projectile_L,
        //                              chara->x - 50,
        //                              chara->y + 10,
        //                              -5);
        //     }
        //     _Register_elements(scene, pro);
        //     chara->new_proj = true;
        // }
    }
}
void hp_bar(int x,int y,int w,int h,int rest){
    //al_draw_rounded_rectangle(x, y, x+w, y+h,1,1,al_map_rgb(255,0,0),2);
    al_draw_rectangle(x, y, x+w, y+h,al_map_rgb(255,0,0),1);
    al_draw_filled_rounded_rectangle(x, y, x+rest,y+h,1,1, al_map_rgb(255,0,0));

/*
    al_init_font_addon();
    al_init_ttf_addon();
    
    char c[3];
    sprintf(c, "%d", rest);
    al_draw_text(
        al_load_font("pirulen.ttf",50,0),
        al_map_rgb(0,0,0),  x,  y, 0,c
    );
    //al_destroy_font(font);
    */
}
void Hero_draw(Elements *self)
{
    // with the state, draw corresponding image
    Hero *chara = ((Hero *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->gif_status[chara->state], al_get_time());
    if (frame)
    {
        hp_bar(chara->x,chara->y,chara->full_life,5,
                   chara->life);
        //chara->life-=chara->life>0?1:0;
        al_draw_bitmap(frame, chara->x, chara->y, ((chara->dir) ? 0:ALLEGRO_FLIP_HORIZONTAL ));
    }
    if (chara->state == atk && chara->gif_status[chara->state]->display_index == 2)
    {
        al_play_sample_instance(chara->atk_Sound);
    }
}
void Hero_destory(Elements *self)
{
    Hero *Obj = ((Hero *)(self->pDerivedObj));
    al_destroy_sample_instance(Obj->atk_Sound);
    for (int i = 0; i < 3; i++)
        algif_destroy_animation(Obj->gif_status[i]);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}

void _Hero_update_position(Elements *self, int dx, int dy)
{
    Hero *chara = ((Hero *)(self->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

void Hero_interact(Elements *self, Elements *tar) {
    // 攻擊+扣血
    Hero *hero=(Hero*)self;
    Monster*mon=(Monster*)tar;

    if (hero->gif_status[atk]->display_index == 2&&(hero->x-mon->x)<=20)
        hero->life-=5;

}