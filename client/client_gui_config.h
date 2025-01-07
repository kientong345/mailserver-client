#ifndef __CLIENT_GUI_CONFIG_H__
#define __CLIENT_GUI_CONFIG_H__

#define FONT_PATH               "./resource/Arial_Bold_Italic.ttf"

#define LOGIN_BACKGROUND_PATH   "./resource/bocchi.jpg"
#define MENU_BACKGROUND_PATH    "./resource/hayasaka.png"

#define LOGIN_USERNAME_BOX  \
static_cast<image>({ \
    "./resource/login_box.png", \
    { \
        {400, 350}, \
        500, \
        200 \
    } \
})

#define LOGIN_PASSWORD_BOX  \
static_cast<image>({ \
    "./resource/login_box.png", \
    { \
        {400, 600}, \
        500, \
        200 \
    } \
})

#define LOGIN_SUBMIT_BOX  \
static_cast<cell>({ \
    "./resource/login_box.png", \
    FONT_PATH, \
    "Submit", \
    DARK_GRAY(100), \
    { \
        {500, 850}, \
        300, \
        150 \
    } \
})

#define LOGIN_QUESTION  \
static_cast<text>({ \
    FONT_PATH, \
    "i have no account", \
    BLACK(100), \
    24, \
    { \
        {300, 1100}, \
        700, \
        150 \
    } \
})

#endif /* __CLIENT_GUI_CONFIG_H__ */