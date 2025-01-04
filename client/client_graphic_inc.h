#ifndef __CLIENT_GRAPHIC_INC__
#define __CLIENT_GRAPHIC_INC__

#include <string>
#include <vector>
#include <cstdint>

#define WINDOW_WIDTH       1000
#define WINDOW_HEIGHT      600

struct color {
    uint8_t red, green, blue;
    uint8_t opacity;
};

#define OPACITY(X)         (((X >= 0) && (X <= 100)) ? static_cast<uint8_t>(255.0/100.0*(X)) : (0))

#define WHITE(OPC)         {255, 255, 255, OPACITY(OPC)}
#define BLACK(OPC)         {0, 0, 0, OPACITY(OPC)}
#define RED(OPC)           {255, 0, 0, OPACITY(OPC)}
#define GREEN(OPC)         {0, 255, 0, OPACITY(OPC)}
#define BLUE(OPC)          {0, 0, 255, OPACITY(OPC)}
#define CYAN(OPC)          {0, 255, 255, OPACITY(OPC)}
#define MAGENTA(OPC)       {255, 0, 255, OPACITY(OPC)}
#define YELLOW(OPC)        {255, 255, 0, OPACITY(OPC)}
#define GRAY(OPC)          {128, 128, 128, OPACITY(OPC)}
#define LIGHT_GRAY(OPC)    {192, 192, 192, OPACITY(OPC)}
#define DARK_GRAY(OPC)     {64, 64, 64, OPACITY(OPC)}

struct coordinate {
    int xPos;
    int yPos;
};

struct area {
    coordinate base;
    int horizontal_length;
    int vertical_length;
};

// struct rectangular_area : public area {
//     int horizontal_length;
//     int vertical_length;
// };

// struct circular_area : public area {
//     unsigned int radius;
// };

struct image {
    std::string img_path;
    area sector;
    // int layer;
};

struct text {
    std::string font_path;
    std::string content;
    color text_color;
    int text_size;
    area sector;
};

struct cell {
    std::string img_path;
    std::string font_path;
    std::string content;
    color text_color;
    area sector;
    // int content_layer;
};

struct cell_row {
    std::vector<cell> cell_set;
    unsigned int max_cell;
};

struct table {
    std::vector<cell_row> row_set;
    unsigned int max_row;
};


#endif /* __CLIENT_GRAPHIC_INC__ */