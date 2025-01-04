#ifndef __CLIENT_GRAPHIC_H__
#define __CLIENT_GRAPHIC_H__

#include "client_graphic_inc.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class ClientGraphic {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    ClientGraphic();

public:
    static ClientGraphic* getInstance();

    void init();
    void end();

    void display_allscreen(const std::string& _path);

    void display_entity(const image& _img);
    void display_entity(const text& _text);
    void display_entity(const cell& _cell);
    void display_entity(const cell_row& _cell_row);
    void display_entity(const table& _table);

    void update_entity(const image& _old_img, const image& _new_img);
    void update_entity(const text& _old_text, const text& _new_text);
    void update_entity(const cell& _old_cell, const cell& _new_cell);
    void update_entity(const cell_row& _old_cell_row, const cell_row& _new_cell_row);
    void update_entity(const table& _old_table, const table& _new_table);

    void erase_area(const area& _region, int layer=0);

    ~ClientGraphic();

};

#endif /* __CLIENT_GRAPHIC_H__ */