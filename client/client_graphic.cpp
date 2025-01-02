#include "client_graphic.h"

ClientGraphic::ClientGraphic() {

}

ClientGraphic::~ClientGraphic() {

}

ClientGraphic* ClientGraphic::getInstance() {
    static ClientGraphic __instance;
    return &__instance;
}

void ClientGraphic::display_entity(const image& _img) {

}

void ClientGraphic::display_entity(const cell& _cell) {

}

void ClientGraphic::display_entity(const cell_row& _cell_row) {

}

void ClientGraphic::display_entity(const table& _table) {

}

void ClientGraphic::update_entity(const image& _old_img, const image& _new_img) {

}

void ClientGraphic::update_entity(const cell& _old_cell, const cell& _new_cell) {

}

void ClientGraphic::update_entity(const cell_row& _old_cell_row, const cell_row& _new_cell_row) {

}

void ClientGraphic::update_entity(const table& _old_table, const table& _new_table) {

}

void ClientGraphic::erase_area(const rectangular_area& _region, int layer) {

}

void ClientGraphic::erase_area(const circular_area& _region, int layer) {

}