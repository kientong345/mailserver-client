#ifndef __CLIENT_INC_H__
#define __CLIENT_INC_H__

#include <chrono>
#include <thread>
#include <string>
#include <utility>
#include <memory>

#define SERVER_IP               "127.0.0.1"
#define SERVER_PORT             8008
#define BUF_SIZE                1024

#define OLDEST_MAIL             (1U)
#define NEWEST_MAIL             (0xFFFFU)

#define WAIT_MS(TIME)           (std::this_thread::sleep_for(std::chrono::milliseconds(TIME)))

#define CLIENT_IP               "127.0.0.1"
#define CLIENT_PORT             7007
//#define __CUSTOM_ADDRESS__

#define __LOGIN_STATE__         (std::make_shared<Client_Ctrl::Login_State>(_client))
#define __REGISTER_STATE__      (std::make_shared<Client_Ctrl::Register_State>(_client))
#define __MENU_STATE__          (std::make_shared<Client_Ctrl::Menu_State>(_client))
#define __FRIENDLIST_STATE__    (std::make_shared<Client_Ctrl::FriendList_State>(_client))
#define __SETTING_STATE__       (std::make_shared<Client_Ctrl::Setting_State>(_client))
#define __INSTRUCTIONS_STATE__  (std::make_shared<Client_Ctrl::Instructions_State>(_client))
#define __INFO_STATE__          (std::make_shared<Client_Ctrl::Info_State>(_client))
#define __CHAT_STATE__          (std::make_shared<Client_Ctrl::Chat_State>(_client))

#define __CURRENT_TIME__        (std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()))

#define LOGIN_SUCCEED           (std::string("login succeed"))
#define LOGIN_FAILED            (std::string("login failed"))
#define REGISTER_SUCCEED        (std::string("register succeed"))
#define REGISTER_FAILED         (std::string("register failed"))

#define LOGIN                   "login"
#define REGISTER                "register"
#define LOGOUT                  "logout"
#define SENDTO                  "sendto"
#define DELETEMAIL              "deletemail"
#define CHANGENAME              "changename"
#define SEARCH                  "search"
#define TERMINATE               "term"
#define WHATISMYNAME            "whatismyname"
#define CHANGEPASSWORD          "changepassword"
#define TOSERVER                "toserver"
#define SHOWCHAT                "showchat"
#define INPUT_LEFT              "left"
#define INPUT_RIGHT             "right"
#define INPUT_UP                "up"
#define INPUT_DOWN              "down"
#define INPUT_SELECT            "select"
#define INPUT_SWITCH            "switch"

#define BACKGROUND_PATH         "./resource/bocchi.jpg"
#define FONT_PATH               "./resource/Arial_Bold_Italic.ttf"

typedef enum {
    REQ_UNIDENTIFY,
    /* server request */
    REQ_LOGIN,
    REQ_REGISTER,
    REQ_LOGOUT,
    REQ_SENDTO,
    REQ_DELETEMAIL,
    REQ_CHANGENAME,
    REQ_SEARCH,
    REQ_TERMINATE,
    REQ_WHATISMYNAME,
    REQ_CHANGEPASSWORD,
    REQ_TOSERVER,
    /* client request */
    REQ_SHOWCHAT,
    REQ_LEFT,
    REQ_RIGHT,
    REQ_UP,
    REQ_DOWN,
    REQ_SELECT,
    REQ_SWITCH
} REQ_TYPE;

typedef enum {
    STATE_NOCHANGE,
    STATE_LOGIN,
    STATE_REGISTATION,
    STATE_MENU,
    STATE_FRIENDLIST,
    STATE_SETTING,
    STATE_INSTRUCTIONS,
    STATE_INFO,
    STATE_CHAT,
    STATE_EXIT
} STATE_TYPE;

typedef enum {
    I_LEFT,
    I_RIGHT,
    I_UP,
    I_DOWN,
    I_SELECT,
    I_SWITCH
} INPUT_TYPE;

typedef enum {
    COMMAND_MODE,
    UI_MODE
} CTRL_MODE;

struct sent_mail {
    std::string receiver;
    std::string content;
    uint64_t sent_time;
    sent_mail(const std::string& _receiver, const std::string& _content, uint64_t _sent_time) 
    : receiver(_receiver), content(_content), sent_time(_sent_time) {}
};

struct received_mail {
    std::string sender;
    std::string content;
    uint64_t rcv_time;
    received_mail(const std::string& _sender, const std::string& _content, uint64_t _rcv_time) 
    : sender(_sender), content(_content), rcv_time(_rcv_time) {}
};

struct chat_line {
    std::string chat_owner;
    std::string chat_content;
    uint64_t chat_time;
    chat_line(const std::string& _owner, const std::string& _content, uint64_t _time) 
    : chat_owner(_owner), chat_content(_content), chat_time(_time) {}
};

typedef std::pair<REQ_TYPE, std::shared_ptr<void>>      req_t;

#endif /* __CLIENT_INC_H__ */