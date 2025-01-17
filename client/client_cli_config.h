#ifndef __CLIENT_CLI_CONFIG_H__
#define __CLIENT_CLI_CONFIG_H__

#define BACKGROUND_IMG  \
(background) { \
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
    "================================================================================" \
    "                                                                                " \
    "================================================================================" \
    " $:                                                                             " \
    "                                                                                " \
    "                                                                                " \
    "                                                                                ", \
    WHITE \
}

#define CMD_IMG  \
(image) { \
    "===============================================================================\n" \
    " $:                                                                            \n" \
    "                                                                               \n" \
    "                                                                               \n" \
    "                                                                               \n", \
    GREEN, \
    { \
        {0, 18}, \
        79, \
        4 \
    } \
}

/* only use after init a CLientCLI object */
#define DEBUG_LOG(TEXT)  \
attron(COLOR_PAIR(YELLOW)); \
mvprintw(17, 1, "%s", std::string(TEXT).c_str()); \
attroff(COLOR_PAIR(YELLOW)); \
refresh();

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

#define LOGIN_INIT_SCREEN     (LOGIN_TEXT), (LOGIN_USERNAME_TEXT), (LOGIN_USERNAME_BOX), (LOGIN_PASSWORD_TEXT), (LOGIN_PASSWORD_BOX), (LOGIN_SUBMIT_BOX), (LOGIN_QUESTION), (CMD_IMG)

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

#define REGISTER_INIT_SCREEN     (REGISTER_TEXT), (REGISTER_USERNAME_TEXT), (REGISTER_USERNAME_BOX), (REGISTER_PASSWORD_TEXT), (REGISTER_PASSWORD_BOX), (REGISTER_SUBMIT_BOX), (REGISTER_QUESTION), (CMD_IMG)


/* ================================MENU SCREEN=============================== */

#define MENU_IND_POS_1  \
(area) { \
    {28, 5}, \
    2, \
    1 \
}

#define MENU_IND_POS_2  \
(area) { \
    {30, 7}, \
    2, \
    1 \
}

#define MENU_IND_POS_3  \
(area) { \
    {27, 9}, \
    2, \
    1 \
}

#define MENU_IND_POS_4  \
(area) { \
    {31, 11}, \
    2, \
    1 \
}

#define MENU_IND_POS_5  \
(area) { \
    {31, 13}, \
    2, \
    1 \
}

#define MENU_TEXT  \
(image) { \
    "============\n" \
    "    menu    \n" \
    "============", \
    MAGENTA, \
    { \
        {34, 1}, \
        20, \
        3 \
    } \
}

#define MENU_SELECT_TABLE  \
(image) { \
    "  _____________ \n" \
    " | Friend List |\n" \
    "    _________   \n" \
    "   | Setting |  \n" \
    " ______________ \n" \
    "| Instructions |\n" \
    "     ______     \n" \
    "    | Info |    \n" \
    "     ______     \n" \
    "    | Exit |    \n", \
    GREEN, \
    { \
        {31, 4}, \
        20, \
        10 \
    } \
}

#define MENU_LOGOUT_BUTTON  \
(image) { \
    " .==========\n" \
    "<<  Logout  \n" \
    " '==========\n", \
    YELLOW, \
    { \
        {2, 13}, \
        20, \
        3 \
    } \
}

#define MENU_INIT_SCREEN    (MENU_TEXT), (MENU_SELECT_TABLE), (MENU_LOGOUT_BUTTON), (CMD_IMG)

/* ================================FRIENDLIST SCREEN=============================== */

#define FRIENDLIST_SCREEN  \
(background) { \
    "================================================================================" \
    " || << Back                ||   YOUR FRIEND LIST   ||                Next >> || " \
    " | >  ===================  <<___      ++++      ___>>  ===================  < | " \
    " ||           User name       '=================='                   Status  || " \
    " | >---<+>--------------------------------------------------------<+>-------< | " \
    " ||     |                                                          |         || " \
    " | >---<+>--------------------------------------------------------<+>-------< | " \
    " ||     |                                                          |         || " \
    " | >---<+>--------------------------------------------------------<+>-------< | " \
    " ||     |                                                          |         || " \
    " | >---<+>--------------------------------------------------------<+>-------< | " \
    " ||     |                                                          |         || " \
    " | >---<+>--------------------------------------------------------<+>-------< | " \
    " ||     |                                                          |         || " \
    " | >---<+>--------------------------------------------------------<+>-------< | " \
    " ||     |                                                          |         || " \
    "================================================================================", \
    CYAN \
}

#define FRIENDLIST_INIT_SCREEN    (CMD_IMG)

#endif /* __CLIENT_CLI_CONFIG_H__ */