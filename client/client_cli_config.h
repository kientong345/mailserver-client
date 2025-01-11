#ifndef __CLIENT_CLI_CONFIG_H__
#define __CLIENT_CLI_CONFIG_H__

#define BACKGROUND_IMG  \
"================================================================================" \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"                                                                                " \
"================================================================================" \
"command:                                                                        " \
"                                                                                " \
"                                                                                " \
"                                                                                "

#define CMD_IMG  \
(image) { \
    "command:                                                                       \n" \
    "                                                                               \n" \
    "                                                                               \n" \
    "                                                                               \n", \
    WHITE, \
    { \
        {0, 19}, \
        79, \
        4 \
    } \
}

#define INDICATOR(POS)  \
(text) { \
    ">>", \
    CYAN, \
    POS \
}

/* =================================LOGIN SCREEN================================= */
#define LOGIN_IND_POS_1  \
(area) { \
    {16, 7}, \
    2, \
    1 \
}

#define LOGIN_IND_POS_2  \
(area) { \
    {16, 9}, \
    2, \
    1 \
}

#define LOGIN_IND_POS_3  \
(area) { \
    {30, 12}, \
    2, \
    1 \
}

#define LOGIN_IND_POS_4  \
(area) { \
    {27, 14}, \
    2, \
    1 \
}

#define LOGIN_TEXT  \
(image) { \
    "===========\n" \
    "   login\n" \
    "===========", \
    MAGENTA, \
    { \
        {34, 3}, \
        20, \
        3 \
    } \
}

#define LOGIN_USERNAME_TEXT  \
(text) { \
    "user name:", \
    WHITE, \
    { \
        {20, 7}, \
        10, \
        1 \
    } \
}

#define LOGIN_USERNAME_BOX  \
(textbox) { \
    { \
        "", \
        BLACK, \
        { \
            {33, 7}, \
            20, \
            1 \
        } \
    }, \
    WHITE \
}

#define LOGIN_PASSWORD_TEXT  \
(text) { \
    "password:", \
    WHITE, \
    { \
        {20, 9}, \
        10, \
        1 \
    } \
}

#define LOGIN_PASSWORD_BOX  \
(textbox) { \
    { \
        "", \
        BLACK, \
        { \
            {33, 9}, \
            20, \
            1 \
        } \
    }, \
    WHITE \
}

#define LOGIN_SUBMIT_BOX  \
(textbox) { \
    { \
        "Submit", \
        GREEN, \
        { \
            {36, 12}, \
            6, \
            1 \
        } \
    }, \
    GREEN \
}

#define LOGIN_QUESTION  \
(text) { \
    "i have no account", \
    WHITE, \
    { \
        {31, 14}, \
        20, \
        1 \
    } \
}

/* ================================REGISTER SCREEN=============================== */
#define REGISTER_IND_POS_1  \
(area) { \
    {16, 7}, \
    2, \
    1 \
}

#define REGISTER_IND_POS_2  \
(area) { \
    {16, 9}, \
    2, \
    1 \
}

#define REGISTER_IND_POS_3  \
(area) { \
    {30, 12}, \
    2, \
    1 \
}

#define REGISTER_IND_POS_4  \
(area) { \
    {27, 14}, \
    2, \
    1 \
}

#define REGISTER_TEXT  \
(image) { \
    "==============\n" \
    "   register\n" \
    "==============", \
    YELLOW, \
    { \
        {32, 3}, \
        20, \
        3 \
    } \
}

#define REGISTER_USERNAME_TEXT  \
(text) { \
    "user name:", \
    WHITE, \
    { \
        {20, 7}, \
        10, \
        1 \
    } \
}

#define REGISTER_USERNAME_BOX  \
(textbox) { \
    { \
        "", \
        BLACK, \
        { \
            {33, 7}, \
            20, \
            1 \
        } \
    }, \
    WHITE \
}

#define REGISTER_PASSWORD_TEXT  \
(text) { \
    "password:", \
    WHITE, \
    { \
        {20, 9}, \
        10, \
        1 \
    } \
}

#define REGISTER_PASSWORD_BOX  \
(textbox) { \
    { \
        "", \
        BLACK, \
        { \
            {33, 9}, \
            20, \
            1 \
        } \
    }, \
    WHITE \
}

#define REGISTER_SUBMIT_BOX  \
(textbox) { \
    { \
        "Submit", \
        GREEN, \
        { \
            {36, 12}, \
            6, \
            1 \
        } \
    }, \
    GREEN \
}

#define REGISTER_QUESTION  \
(text) { \
    "i've had an account", \
    WHITE, \
    { \
        {30, 14}, \
        20, \
        1 \
    } \
}

#endif /* __CLIENT_CLI_CONFIG_H__ */