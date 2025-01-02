#ifndef __CLIENT_GRAPHIC_INC__
#define __CLIENT_GRAPHIC_INC__

#include <string>
#include <vector>

struct coordinate {
    int xPos;
    int yPos;
};

struct area {
    coordinate base;
};

struct rectangular_area : public area {
    int horizontal_length;
    int vertical_length;
};

struct circular_area : public area {
    unsigned int radius;
};

struct image {
    std::string img_path;
    area sector;
    int layer;
};

struct cell {
    image background;
    std::string content;
    int content_layer;
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