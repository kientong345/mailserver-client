#ifndef __CLIENT_GRAPHIC_H__
#define __CLIENT_GRAPHIC_H__

#include "client_graphic_inc.h"

class ClientGraphic {
private:
    ClientGraphic();

public:
    static ClientGraphic* getInstance();

    void display_entity(const image& _img);
    void display_entity(const cell& _cell);
    void display_entity(const cell_row& _cell_row);
    void display_entity(const table& _table);

    void update_entity(const image& _old_img, const image& _new_img);
    void update_entity(const cell& _old_cell, const cell& _new_cell);
    void update_entity(const cell_row& _old_cell_row, const cell_row& _new_cell_row);
    void update_entity(const table& _old_table, const table& _new_table);

    void erase_area(const rectangular_area& _region, int layer);
    void erase_area(const circular_area& _region, int layer);

    ~ClientGraphic();

};

#endif /* __CLIENT_GRAPHIC_H__ */