#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "starfield.h"
#include "ship.h"

enum {
  MENU_ITEM_PLAY = 0,
  MENU_ITEM_EXIT = 1,
};

struct FONTS {
  ALLEGRO_FONT *fontLarge;
  ALLEGRO_FONT *fontSmall;
};

struct COLORS {
  ALLEGRO_COLOR menuYellow;
  ALLEGRO_COLOR menuWhite;
};

struct MENU_ITEM {
  int id;
  char text[5];
};

struct MENU {
  int selectedId;
  int menuLength;
  struct MENU_ITEM menu[];
};

void selectMenuItem(struct MENU *menu, ALLEGRO_KEYBOARD_EVENT event) {
  int selectUp(int currentSelected) {
    return currentSelected-1 < 0 ? menu->menuLength - 1 : currentSelected - 1;
  }

  int selectDown(int currentSelected) {
    return currentSelected+1 == menu->menuLength ? 0 : currentSelected + 1;
  }

  for(int i=0; i<menu->menuLength; i++) {
    if (menu->menu[i].id == menu->selectedId) {
      int newSelected = event.keycode == ALLEGRO_KEY_UP ? selectUp(i) : selectDown(i);
      menu->selectedId = newSelected;
      break;
    };
  }
}

void refreshMenu(struct FONTS fonts, struct COLORS colors, int displayWidth, int displayHeight, ALLEGRO_BITMAP *menuBitmap, struct MENU *menu, ALLEGRO_DISPLAY *display) {
  int fontSmallHeight = al_get_font_line_height(fonts.fontSmall);

  al_set_target_bitmap(menuBitmap);
  al_clear_to_color(al_premul_rgba(255,255,255,0));

  for (int i=0; i<menu->menuLength; i++) {
    al_draw_textf(fonts.fontSmall, menu->menu[i].id == menu->selectedId ? colors.menuYellow : colors.menuWhite, displayWidth/2, displayHeight*.6+(i * al_get_font_line_height(fonts.fontSmall)), ALLEGRO_ALIGN_CENTER, menu->menu[i].text);
  }
  al_set_target_backbuffer(display);
  al_draw_bitmap(menuBitmap, 0, 0, 0);
  al_flip_display();
}

void drawTitle(struct FONTS fonts, struct COLORS colors, int displayWidth, int displayHeight) {
  al_draw_textf(fonts.fontLarge, colors.menuWhite, displayWidth/2, displayHeight*.3, ALLEGRO_ALIGN_CENTER, "ASTEROIDS");
}

void unloadMenu(struct FONTS fonts, struct COLORS colors, struct MENU *menu) {
  for(float i=255; i>=0; i-=10) {

  }
}

bool handleKeyboardEvent(ALLEGRO_EVENT *current_event, struct MENU *menu, struct FONTS fonts) {
  if (current_event->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
    exit(0);
  }
  if (current_event->keyboard.keycode == ALLEGRO_KEY_DOWN || current_event->keyboard.keycode == ALLEGRO_KEY_UP) {
    selectMenuItem(menu, current_event->keyboard);
    return true;
  }
  if (current_event->keyboard.keycode == ALLEGRO_KEY_ENTER) {
    if (menu->selectedId == MENU_ITEM_EXIT) {
      exit(0);
    }
    if (menu->selectedId == MENU_ITEM_PLAY) {
      // unloadMenu(fonts, display, menu);
      return true;
    }
  }
  printf("%d\n", current_event->keyboard.keycode);
}

int init_main_menu(ALLEGRO_DISPLAY *display_main) {
  ALLEGRO_FONT *fontLarge, *fontSmall;
  ALLEGRO_EVENT current_event;
  ALLEGRO_BITMAP *starfield;
  ALLEGRO_BITMAP *ship;
  ALLEGRO_BITMAP *menuBitmap;
  int displayWidth = al_get_display_width(display_main);
  int displayHeight = al_get_display_height(display_main);
  menuBitmap = al_create_bitmap(displayWidth, displayHeight);

  al_init_primitives_addon();
  al_init_font_addon();
  al_init_image_addon();
  al_init_ttf_addon();
  al_install_mouse();
  al_install_keyboard();

  int menuOpacity = 255;

  struct COLORS colors = {
    .menuWhite = al_premul_rgba(255,255,255, menuOpacity),
    .menuYellow = al_premul_rgba(232, 252, 3, menuOpacity),
  };

  struct MENU_ITEM play = {
    .id = MENU_ITEM_PLAY,
    .text = "PLAY",
  };

  struct MENU_ITEM exit = {
    .id = MENU_ITEM_EXIT,
    .text = "EXIT",
  };

  struct MENU_ITEM menuItems[2] = { play, exit };
  struct MENU *menu = malloc(sizeof(int) * 2 + sizeof (struct MENU_ITEM) * 2);

  menu->selectedId = 0;
  menu->menuLength = 2;
  memcpy(menu->menu, menuItems, sizeof(menuItems));

  fontLarge = al_load_ttf_font("data/DejaVuSans.ttf", 36, 0);
  fontSmall = al_load_ttf_font("data/DejaVuSans.ttf", 18, 0);

  if (!fontLarge || !fontSmall) {
    printf("Failed to load fonts");
  }

  struct FONTS fonts = {
    fontLarge = fontLarge,
    fontSmall = fontSmall,
  };

  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();

  al_register_event_source(queue, al_get_mouse_event_source());
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(display_main));

  starfield = generateStarfield(displayWidth, displayHeight);
  ship = generateShip();
  al_set_target_backbuffer(display_main);
  drawTitle(fonts, colors, displayWidth, displayHeight);
  al_draw_bitmap(starfield, -displayWidth, -displayHeight, 0);
  al_draw_bitmap(ship, displayWidth/2-al_get_bitmap_width(ship)/2, displayHeight/2-al_get_bitmap_height(ship)/2, 0);
  refreshMenu(fonts, colors, displayWidth, displayHeight, menuBitmap, menu, display_main);

  while (true) {
    al_wait_for_event(queue, &current_event);
    switch (current_event.type) {
      case ALLEGRO_EVENT_KEY_DOWN:
        bool refreshRequired = handleKeyboardEvent(&current_event, menu, fonts);
        if (refreshRequired) {
          refreshMenu(fonts, colors, displayWidth, displayHeight, menuBitmap, menu, display_main);
        }
        break;
      case ALLEGRO_EVENT_DISPLAY_RESIZE:
        al_acknowledge_resize(display_main);
        displayWidth = current_event.display.width;
        displayHeight = current_event.display.height;
        starfield = generateStarfield(displayWidth, displayHeight);
        al_set_target_backbuffer(display_main);
        al_draw_bitmap(starfield, -(al_get_display_width(display_main)), -(al_get_display_height(display_main)), 0);
        al_draw_bitmap(ship, al_get_display_width(display_main)/2-al_get_bitmap_width(ship)/2, al_get_display_height(display_main)/2 - al_get_bitmap_height(ship)/2, 0);
        refreshMenu(fonts, colors, displayWidth, displayHeight, menuBitmap, menu, display_main);
        break;
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        return 0;

      default:
        break;
    };
  };
}
