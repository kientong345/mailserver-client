#ifndef __CLIENT_CLI_H__
#define __CLIENT_CLI_H__

#include "client_cli_inc.h"
#include "client_inc.h"

class ClientCLI {
private:
    int _text_color;
    int _theme_color;
    ClientCLI();

public:
    static ClientCLI* getInstance();

    void init(int text_color = COLOR_WHITE, int theme_color = COLOR_BLACK);
    void end();

    void display_allscreen(const background& _bg);

    void display_entity(const image& _img);
    void display_entity(const text& _text);
    void display_entity(const textbox& _textbox);

    template<typename T, typename... Args>
    void display_multiple_entity(const T& _first, const Args&... _rest) {
        display_entity(_first);
        if constexpr (sizeof...(_rest) > 0) {
            display_multiple_entity(_rest...);
        }
    }

    // void update_entity(const image& _old_img, const image& _new_img);
    // void update_entity(const text& _old_text, const text& _new_text);
    // void update_entity(const textbox& _old_textbox, const textbox& _new_textbox);

    void erase_area(const area& _region);

    INPUT_TYPE get_user_input();
    std::string get_user_cmd(int xpos = 1, int ypos = 1);
    void pause_ui();
    void continue_ui();

    ~ClientCLI();

};

#endif /* __CLIENT_CLI_H__ */