#include <stdio.h>
#include <string.h>

#include <allegro5/allegro.h>

#include "mainMenu.h"

int main(int argc, char **argv) {
  if (!al_init()) {
    printf("Failed to init Allegro\n");
    return 0;
  }

  char *title = "Asteroids";
  ALLEGRO_EVENT current_event;

  al_set_new_display_flags(ALLEGRO_RESIZABLE);
  ALLEGRO_DISPLAY *display_main = al_create_display(800, 800);
  al_set_window_title(display_main, title);

  init_main_menu(display_main);
}
