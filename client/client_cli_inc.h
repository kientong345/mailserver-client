#ifndef __CLIENT_CLI_INC_H__
#define __CLIENT_CLI_INC_H__

#include <string>
#include <cstdint>
#include <ncurses.h>

struct color {
    uint8_t red, green, blue;
};

#define WHITE         COLOR_WHITE
#define BLACK         COLOR_BLACK
#define RED           COLOR_RED
#define GREEN         COLOR_GREEN
#define BLUE          COLOR_BLUE
#define CYAN          COLOR_CYAN
#define MAGENTA       COLOR_MAGENTA
#define YELLOW        COLOR_YELLOW

struct background {
    std::string img_str;
    int img_color;
};

struct coordinate {
    int xPos;
    int yPos;
};

struct area {
    coordinate base;
    int horizontal_length;
    int vertical_length;
};

struct image {
    std::string img_str;
    int img_color;
    area sector;
};

struct text {
    std::string content;
    int text_color;
    area sector;
};

struct textbox {
    text content;
    int box_color;
};

/*
═ ║ ╝ ╚ ╗ ╔ ╠ ╣ ╦ ╩ ╬
*/

#endif /* __CLIENT_CLI_INC_H__ */