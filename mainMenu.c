#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

enum {
  MENU_ITEM_PLAY = 0,
  MENU_ITEM_EXIT = 1,
};

struct FONTS {
  ALLEGRO_FONT *fontLarge;
  ALLEGRO_FONT *fontSmall;
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

void refreshDisplay(struct FONTS fonts, ALLEGRO_DISPLAY *display, struct MENU *menu) {
  int fontSmallHeight = al_get_font_line_height(fonts.fontSmall);
  ALLEGRO_COLOR white = al_map_rgb(255,255,255);
  ALLEGRO_COLOR yellow = al_map_rgb(232, 252, 3);

  al_acknowledge_resize(display);
  int width = al_get_display_width(display);
  int height = al_get_display_height(display);
  al_clear_to_color(al_map_rgba(0,0,0,1));

  al_draw_textf(fonts.fontLarge, white, width/2, height*.3, ALLEGRO_ALIGN_CENTER, "ASTEROIDS");

  for (int i=0; i<menu->menuLength; i++) {
    al_draw_textf(fonts.fontSmall, menu->menu[i].id == menu->selectedId ? yellow : white, width/2, height*.6+(i * al_get_font_line_height(fonts.fontSmall)), ALLEGRO_ALIGN_CENTER, menu->menu[i].text);
  }
  al_flip_display();
}

int init_main_menu(ALLEGRO_DISPLAY *display_main) {
  ALLEGRO_FONT *fontLarge, *fontSmall;
  ALLEGRO_EVENT current_event;

  al_init_primitives_addon();
  al_init_font_addon();
  al_init_image_addon();
  al_init_ttf_addon();
  al_install_mouse();
  al_install_keyboard();

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

  refreshDisplay(fonts, display_main, menu);

  while (true) {
  al_wait_for_event(queue, &current_event);

    switch (current_event.type) {
      case ALLEGRO_EVENT_KEY_DOWN:
        if (current_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
          return 0;
        }
        if (current_event.keyboard.keycode == ALLEGRO_KEY_DOWN || current_event.keyboard.keycode == ALLEGRO_KEY_UP) {
          selectMenuItem(menu, current_event.keyboard);
          refreshDisplay(fonts, display_main, menu);
        }
        if (current_event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
          if (menu->selectedId == MENU_ITEM_EXIT) {
            return 0;
          }
        }
        printf("%d\n", current_event.keyboard.keycode);
        break;

      case ALLEGRO_EVENT_DISPLAY_RESIZE:
        refreshDisplay(fonts, display_main, menu);
        break;
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        return 0;

      default:
        break;
    };
  };
}
