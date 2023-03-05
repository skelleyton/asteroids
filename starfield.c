#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_BITMAP *generateStarfield(int displayWidth, int displayHeight) {
  ALLEGRO_BITMAP *starfield = al_create_bitmap(displayWidth * 3, displayHeight * 3);
  al_clear_to_color(al_map_rgb(0,0,0));
  al_set_target_bitmap(starfield);

  al_init_primitives_addon();

  for(int i=0; i<displayHeight; i++) {
    int x = rand() % displayWidth * 3;
    int y = rand() % displayHeight * 3;
    al_draw_filled_rounded_rectangle(x, y, x+2, y+2, 2, 2, al_map_rgb(255,255,255));
  }

  return starfield;
}
