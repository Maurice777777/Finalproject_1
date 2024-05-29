#include "mons-bullet.h"
#include "../shapes/Circle.h"
/*
   [Projectile function]
*/
Elements *New_Monsbullet(int label, int x, int y, int v)//生成時初始化條件
{
    Monsbullet *pDerivedObj = (Monsbullet *)malloc(sizeof(Monsbullet));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/bullet1.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->v = v;
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x + pDerivedObj->width / 2,
                                     pDerivedObj->y + pDerivedObj->height / 2,
                                     min(pDerivedObj->width, pDerivedObj->height) / 2);
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Character_L;
    pObj->inter_obj[pObj->inter_len++] = Floor_L;
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Monsbullet_update;
    pObj->Interact = Monsbullet_interact;
    pObj->Draw = Monsbullet_draw;
    pObj->Destroy = Monsbullet_destory;

    return pObj;
}
void Monsbullet_update(Elements *self)//每貞做怎樣的事情
{
    Monsbullet *Obj = ((Monsbullet *)(self->pDerivedObj));
    _Monsbullet_update_position(self, Obj->v, 0);
}
void _Monsbullet_update_position(Elements *self, int dx, int dy)
{
    Monsbullet *Obj = ((Monsbullet *)(self->pDerivedObj));
    Obj->x += dx;
    Obj->y += dy;
    Shape *hitbox = Obj->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}
void Monsbullet_interact(Elements *self, Elements *tar)
{
    Monsbullet *Obj = ((Monsbullet *)(self->pDerivedObj));
    if (tar->label == Floor_L)
    {
        if (Obj->x < 0 - Obj->width)
            self->dele = true;//dele become true->disappear
        else if (Obj->x > WIDTH + Obj->width)
            self->dele = true;
    }
    else if (tar->label == Character_L)
    {
        Character *charc = ((Character *)(tar->pDerivedObj));
        if (charc->hitbox->overlap(charc->hitbox, Obj->hitbox))
        {
            self->dele = true;
        }
    }
}
void Monsbullet_draw(Elements *self)
{
    Monsbullet *Obj = ((Monsbullet *)(self->pDerivedObj));
    if (Obj->v > 0)
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, ALLEGRO_FLIP_HORIZONTAL);
    else
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Monsbullet_destory(Elements *self)
{
    Monsbullet *Obj = ((Monsbullet *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
