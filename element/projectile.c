#include "projectile.h"
#include "../shapes/Circle.h"
/*
   [Projectile function]
*/
Elements *New_Projectile(int label, int x, int y, int v)//生成時初始化條件
{
    Projectile *pDerivedObj = (Projectile *)malloc(sizeof(Projectile));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/projectile.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->v = v;
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x + pDerivedObj->width / 2,
                                     pDerivedObj->y + pDerivedObj->height / 2,
                                     min(pDerivedObj->width, pDerivedObj->height) / 2);
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Tree_L;
    pObj->inter_obj[pObj->inter_len++] = Floor_L;
    pObj->inter_obj[pObj->inter_len++] = Monster_L;
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Projectile_update;
    pObj->Interact = Projectile_interact;
    pObj->Draw = Projectile_draw;
    pObj->Destroy = Projectile_destory;

    return pObj;
}
void Projectile_update(Elements *self)//每貞做怎樣的事情
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    _Projectile_update_position(self, Obj->v, 0);
}
void _Projectile_update_position(Elements *self, int dx, int dy)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    Obj->x += dx;
    Obj->y += dy;
    Shape *hitbox = Obj->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}
void Projectile_interact(Elements *self, Elements *tar)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if (tar->label == Monster_L)
    {
        Monster *mons = ((Monster *)(tar->pDerivedObj));
        if (mons->hitbox->overlap(mons->hitbox, Obj->hitbox))
        {
            self->dele = true;
        }
    }
    else if (tar->label == Tree_L)
    {
        Tree *tree = ((Tree *)(tar->pDerivedObj));
        if (tree->hitbox->overlap(tree->hitbox, Obj->hitbox))
        {
            self->dele = true;
        }
    }
    else if (tar->label == Floor_L)
    {
        if (Obj->x < 0 - Obj->width)
            self->dele = true;//dele become true->disappear
        else if (Obj->x > WIDTH + Obj->width)
            self->dele = true;
    }
    
}
void Projectile_draw(Elements *self)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if (Obj->v > 0)
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, ALLEGRO_FLIP_HORIZONTAL);
    else
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Projectile_destory(Elements *self)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
