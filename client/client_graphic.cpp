#include "client_graphic.h"
#include <iostream>

ClientGraphic::ClientGraphic()
: window(nullptr),
  renderer(nullptr) {

}

ClientGraphic::~ClientGraphic() {
    
}

ClientGraphic* ClientGraphic::getInstance() {
    static ClientGraphic __instance;
    return &__instance;
}

void ClientGraphic::init() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();

    window = SDL_CreateWindow(
        "window name",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void ClientGraphic::end() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
}

void ClientGraphic::display_allscreen(const std::string& _path) {
    SDL_Surface* window_background = IMG_Load(_path.c_str());
    SDL_Texture* window_texture = SDL_CreateTextureFromSurface(renderer, window_background);
    SDL_FreeSurface(window_background);

    /* display background */
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, window_texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(window_texture);
}

void ClientGraphic::display_entity(const image& _img) {
    SDL_Rect update_area = {
        _img.sector.base.xPos,
        _img.sector.base.yPos,
        _img.sector.horizontal_length,
        _img.sector.vertical_length
    };

    SDL_Surface* img_bg = IMG_Load(_img.img_path.c_str());
    SDL_Texture* update_texture = SDL_CreateTextureFromSurface(renderer, img_bg);
    SDL_FreeSurface(img_bg);
    
    SDL_RenderCopy(renderer, update_texture, nullptr, &update_area);
    SDL_RenderPresent(renderer);
    
    SDL_DestroyTexture(update_texture);
}

void ClientGraphic::display_entity(const text& _text) {
    SDL_Color text_color = {
        _text.text_color.red,
        _text.text_color.green,
        _text.text_color.blue,
        _text.text_color.opacity
    };
    SDL_Rect text_area = {
        _text.sector.base.xPos,
        _text.sector.base.yPos,
        _text.sector.horizontal_length,
        _text.sector.vertical_length
    };

    TTF_Font* font = TTF_OpenFont(_text.font_path.c_str(), _text.text_size);
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, _text.content.c_str(), text_color);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);

    SDL_RenderCopy(renderer, text_texture, nullptr, &text_area);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(font);
}

void ClientGraphic::display_entity(const cell& _cell) {
    SDL_Color text_color = {
        _cell.text_color.red,
        _cell.text_color.green,
        _cell.text_color.blue,
        _cell.text_color.opacity
    };
    SDL_Rect cell_area = {
        _cell.sector.base.xPos,
        _cell.sector.base.yPos,
        _cell.sector.horizontal_length,
        _cell.sector.vertical_length
    };

    SDL_Surface* img_bg = IMG_Load(_cell.img_path.c_str());
    SDL_Texture* bg_texture = SDL_CreateTextureFromSurface(renderer, img_bg);
    SDL_FreeSurface(img_bg);
    SDL_RenderCopy(renderer, bg_texture, nullptr, &cell_area);
    SDL_RenderPresent(renderer);

    TTF_Font* font = TTF_OpenFont(_cell.font_path.c_str(), _cell.sector.vertical_length * 0.7);
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, _cell.content.c_str(), text_color);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);   
    SDL_RenderCopy(renderer, text_texture, nullptr, &cell_area);
    SDL_RenderPresent(renderer);
    
    SDL_DestroyTexture(bg_texture);
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(font);
}

void ClientGraphic::display_entity(const cell_row& _cell_row) {

}

void ClientGraphic::display_entity(const table& _table) {

}

void ClientGraphic::update_entity(const image& _old_img, const image& _new_img) {

}

void ClientGraphic::update_entity(const text& _old_text, const text& _new_text) {

}

void ClientGraphic::update_entity(const cell& _old_cell, const cell& _new_cell) {

}

void ClientGraphic::update_entity(const cell_row& _old_cell_row, const cell_row& _new_cell_row) {

}

void ClientGraphic::update_entity(const table& _old_table, const table& _new_table) {

}

void ClientGraphic::erase_area(const area& _region, int layer) {

}