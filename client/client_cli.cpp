#include "client_cli.h"
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <string>

ClientCLI::ClientCLI()
: _text_color(0),
  _theme_color(0) {

}

ClientCLI::~ClientCLI() {
    
}

ClientCLI* ClientCLI::getInstance() {
    static ClientCLI __instance;
    return &__instance;
}

void ClientCLI::init(int text_color, int theme_color) {
    _text_color = text_color;
    _theme_color = theme_color;
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();
    init_pair(COLOR_BLACK, COLOR_BLACK, _theme_color);
    init_pair(COLOR_RED, COLOR_RED, _theme_color);
    init_pair(COLOR_GREEN, COLOR_GREEN, _theme_color);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, _theme_color);
    init_pair(COLOR_BLUE, COLOR_BLUE, _theme_color);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, _theme_color);
    init_pair(COLOR_CYAN, COLOR_CYAN, _theme_color);
    init_pair(COLOR_WHITE, COLOR_WHITE, _theme_color);

    init_pair(8, _text_color, _theme_color);

    clear();
    bkgd(COLOR_PAIR(8));
    refresh();
}

void ClientCLI::end() {
    curs_set(1);
    clear();
    keypad(stdscr, FALSE);
    echo();
    nocbreak();
    endwin();
}

void ClientCLI::display_allscreen(const background& _bg) {
    erase();
    attron(COLOR_PAIR(_bg.img_color));
    printw("%s", _bg.img_str.c_str());
    attroff(COLOR_PAIR(_bg.img_color));
    refresh();
}

void ClientCLI::display_entity(const image& _img) {
    std::vector<std::string> line_vec;
    auto it = _img.img_str.begin();
    while (it != _img.img_str.end()) {
        std::string line = "";
        while (*it != '\n' && it != _img.img_str.end()) {
            line += *it;
            ++it;
        }
        line_vec.push_back(std::move(line));
        if (line_vec.size() >= _img.sector.vertical_length) break;
        if (it != _img.img_str.end()) ++it;
    }
    attron(COLOR_PAIR(_img.img_color));
    uint8_t y_offset = 0;
    for (const auto& _line : line_vec) {
        mvprintw(_img.sector.base.yPos + y_offset,
                _img.sector.base.xPos,
                "%.*s",
                _img.sector.horizontal_length,
                _line.c_str());
        ++y_offset;
    }
    attroff(COLOR_PAIR(_img.img_color));
    refresh();
}

void ClientCLI::display_entity(const text& _text) {
    attron(COLOR_PAIR(_text.text_color));
    uint8_t y_offset = 0;
    mvprintw(_text.sector.base.yPos + y_offset,
            _text.sector.base.xPos,
            "%.*s",
            _text.sector.horizontal_length,
            _text.content.c_str());
    ++y_offset;
    attroff(COLOR_PAIR(_text.text_color));
    refresh();
}

void ClientCLI::display_entity(const textbox& _textbox) {
    std::string horizontal_border(_textbox.content.sector.horizontal_length, '=');
    // for (int i = 0; i < _textbox.content.sector.horizontal_length; ++i)
    //     horizontal_border += "═";
    // std::string upper_border = "╔═" + horizontal_border + "═╗";
    // std::string lower_border = "╚═" + horizontal_border + "═╝";
    std::string upper_border = ".=" + horizontal_border + "=.";
    std::string lower_border = "'=" + horizontal_border + "='";

    attron(COLOR_PAIR(_textbox.box_color));
    move(_textbox.content.sector.base.yPos - 1, _textbox.content.sector.base.xPos - 2);
    printw("%s", upper_border.c_str());
    move(_textbox.content.sector.base.yPos + _textbox.content.sector.vertical_length, _textbox.content.sector.base.xPos - 2);
    printw("%s", lower_border.c_str());
    for (int i = 0; i < _textbox.content.sector.vertical_length; ++i) {
        move(_textbox.content.sector.base.yPos + i, _textbox.content.sector.base.xPos - 2);
        printw("|");
        move(_textbox.content.sector.base.yPos + i, _textbox.content.sector.base.xPos + _textbox.content.sector.horizontal_length + 1);
        printw("|");
    }
    attroff(COLOR_PAIR(_textbox.box_color));
    refresh();
    display_entity(_textbox.content);
}

void ClientCLI::erase_area(const area& _region) {
    const std::string empty_line(_region.horizontal_length, ' ');
    for (int y_offset = 0; y_offset < _region.vertical_length; ++y_offset) {
        mvprintw(_region.base.yPos + y_offset,
                _region.base.xPos,
                "%.*s",
                _region.horizontal_length,
                empty_line.c_str());
    }
    refresh();
}

/**
 * @brief wait key input from user
 * @return the inputted key
 */
INPUT_TYPE ClientCLI::get_user_input() {
    INPUT_TYPE ret;

    bool invalid_key = true;
    while (invalid_key) {
        int key = getch();
        switch (key) {
        case KEY_UP:
            ret = I_UP;
            invalid_key = false;
            break;
        case KEY_DOWN:
            ret = I_DOWN;
            invalid_key = false;
            break;
        case KEY_LEFT:
            ret = I_LEFT;
            invalid_key = false;
            break;
        case KEY_RIGHT:
            ret = I_RIGHT;
            invalid_key = false;
            break;
        case 10: // ASCII code of Enter
            ret = I_SELECT;
            invalid_key = false;
            break;
        case 27: // ASCII code of Esc
            ret = I_SWITCH;
            invalid_key = false;
            break;
        default: 
            break;
        }
    }

    return ret;
}

std::string ClientCLI::get_user_cmd(int xpos, int ypos) {
    move(ypos, xpos);
    char tmp[255] = {0};
    getstr(tmp);
    return std::string(tmp);
}

/**
 * @brief temperary return to cmd mode and return the cursor to a position
 * @param xpos: x-coordinate of the cursor
 * @param ypos: y-coordinate of the cursor
 * @return none
 */
void ClientCLI::pause_ui() {
    keypad(stdscr, FALSE);
    echo();
    nocbreak();
    curs_set(1);
}

/**
 * @brief continue cli(ui) mode
 * @return none
 */
void ClientCLI::continue_ui() {
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}