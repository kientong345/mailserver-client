#ifndef __CLIENT_GUI_CONFIG_H__
#define __CLIENT_GUI_CONFIG_H__

#define FONT_PATH               "./resource/TahomaBd.ttf"

#define LOGIN_BACKGROUND_PATH   "./resource/bocchi.jpg"
#define MENU_BACKGROUND_PATH    "./resource/hayasaka.png"

#define INDICATOR(POS)  \
(text) { \
    FONT_PATH, \
    ">>", \
    CYAN(100), \
    30, \
    POS \
}

/* =================================LOGIN SCREEN================================= */
#define LOGIN_IND_POS_1  \
(area) { \
    {130, 200}, \
    50, \
    50 \
}

#define LOGIN_IND_POS_2  \
(area) { \
    {130, 300}, \
    50, \
    50 \
}

#define LOGIN_IND_POS_3  \
(area) { \
    {360, 400}, \
    50, \
    50 \
}

#define LOGIN_IND_POS_4  \
(area) { \
    {280, 480}, \
    50, \
    50 \
}

#define LOGIN_TEXT  \
(text) { \
    FONT_PATH, \
    "login", \
    RED(100), \
    30, \
    { \
        {380, 80}, \
        270, \
        80 \
    } \
}

#define LOGIN_USERNAME_TEXT  \
(text) { \
    FONT_PATH, \
    "user name", \
    BLACK(100), \
    24, \
    { \
        {200, 200}, \
        180, \
        50 \
    } \
}

#define LOGIN_USERNAME_BOX  \
(image) { \
    "./resource/login_box.png", \
    { \
        {400, 200}, \
        300, \
        50 \
    } \
}

#define LOGIN_PASSWORD_TEXT  \
(text) { \
    FONT_PATH, \
    "password", \
    BLACK(100), \
    24, \
    { \
        {200, 300}, \
        180, \
        50 \
    } \
}

#define LOGIN_PASSWORD_BOX  \
(image) { \
    "./resource/login_box.png", \
    { \
        {400, 300}, \
        300, \
        50 \
    } \
}

#define LOGIN_SUBMIT_BOX  \
(cell) { \
    "./resource/login_box.png", \
    FONT_PATH, \
    "Submit", \
    DARK_GRAY(100), \
    { \
        {430, 400}, \
        140, \
        50 \
    } \
}

#define LOGIN_QUESTION  \
(text) { \
    FONT_PATH, \
    "i have no account", \
    GREEN(100), \
    18, \
    { \
        {350, 480}, \
        300, \
        50 \
    } \
}

/* ================================REGISTER SCREEN=============================== */

#define REGISTER_IND_POS_1  \
(area) { \
    {130, 200}, \
    50, \
    50 \
}

#define REGISTER_IND_POS_2  \
(area) { \
    {130, 300}, \
    50, \
    50 \
}

#define REGISTER_IND_POS_3  \
(area) { \
    {360, 400}, \
    50, \
    50 \
}

#define REGISTER_IND_POS_4  \
(area) { \
    {260, 480}, \
    50, \
    50 \
}

#define REGISTER_TEXT  \
(text) { \
    FONT_PATH, \
    "Register", \
    RED(100), \
    30, \
    { \
        {350, 80}, \
        320, \
        80 \
    } \
}

#define REGISTER_USERNAME_TEXT  \
(text) { \
    FONT_PATH, \
    "user name", \
    BLACK(100), \
    24, \
    { \
        {200, 200}, \
        180, \
        50 \
    } \
}

#define REGISTER_USERNAME_BOX  \
(image) { \
    "./resource/login_box.png", \
    { \
        {400, 200}, \
        300, \
        50 \
    } \
}

#define REGISTER_PASSWORD_TEXT  \
(text) { \
    FONT_PATH, \
    "password", \
    BLACK(100), \
    24, \
    { \
        {200, 300}, \
        180, \
        50 \
    } \
}

#define REGISTER_PASSWORD_BOX  \
(image) { \
    "./resource/login_box.png", \
    { \
        {400, 300}, \
        300, \
        50 \
    } \
}

#define REGISTER_SUBMIT_BOX  \
(cell) { \
    "./resource/login_box.png", \
    FONT_PATH, \
    "Submit", \
    DARK_GRAY(100), \
    { \
        {430, 400}, \
        140, \
        50 \
    } \
}

#define REGISTER_QUESTION  \
(text) { \
    FONT_PATH, \
    "i've had an account", \
    GREEN(100), \
    18, \
    { \
        {330, 480}, \
        360, \
        50 \
    } \
}

#endif /* __CLIENT_GUI_CONFIG_H__ */