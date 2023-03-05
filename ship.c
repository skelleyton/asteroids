#include<stdio.h>

#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>

ALLEGRO_BITMAP *generateShip() {
  ALLEGRO_BITMAP *ship = al_create_bitmap(16, 16);
  al_set_target_bitmap(ship);
  al_clear_to_color(al_map_rgba(0,0,0,0));

  al_draw_triangle(8,0,0,16,16,16, al_map_rgb(166,166,166), 2);

  return ship;
}
