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

#define __LOGIN_STATE__         (std::make_shared<Login_State>(_client))
#define __REGISTER_STATE__      (std::make_shared<Register_State>(_client))
#define __MENU_STATE__          (std::make_shared<Menu_State>(_client))

#define __CURRENT_TIME__        (std::string(std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()))

#define LOGIN_SUCCEED           (std::string("login succeed"))
#define LOGIN_FAILED            (std::string("login failed"))
#define REGISTER_SUCCEED        (std::string("register succeed"))
#define REGISTER_FAILED         (std::string("register failed"))

#define LOGIN                   "login"
#define REGISTER                "register"
#define SENDTO                  "sendto"
#define DELETEMAIL              "deletemail"
#define CHANGENAME              "changename"
#define SEARCH                  "search"
#define TERMINATE               "term"
#define WHATISMYNAME            "whatismyname"
#define CHANGEPASSWORD          "changepassword"
#define TOSERVER                "toserver"
#define SHOWCHAT                "showchat"
#define INPUT_BACK              "back"
#define INPUT_NEXT              "next"
#define INPUT_UP                "up"
#define INPUT_DOWN              "down"
#define INPUT_SELECT            "select"
#define INPUT_SWITCH            "switch"

typedef enum {
    REQ_UNIDENTIFY,
    /* server request */
    REQ_LOGIN,
    REQ_REGISTER,
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
    REQ_BACK,
    REQ_NEXT,
    REQ_UP,
    REQ_DOWN,
    REQ_SELECT,
    REQ_SWITCH
} REQ_TYPE;

typedef enum {
    STATE_NOCHANGE,
    STATE_LOGIN,
    STATE_REGISTATION,
    STATE_MENU
} STATE_TYPE;

typedef enum {
    I_BACK,
    I_NEXT,
    I_UP,
    I_DOWN,
    I_SELECT,
    I_SWITCH
} INPUT_TYPE;

typedef enum {
    COMMAND_MODE,
    UI_MODE
} CTRL_MODE;

typedef struct {
    std::string receiver;
    std::string content;
    uint64_t sent_time;
} sent_mail;

typedef struct {
    std::string sender;
    std::string content;
    uint64_t rcv_time;
} received_mail;

typedef struct {
    std::string chat_owner;
    std::string chat_content;
    uint64_t chat_time;
} chat_line;

typedef std::pair<REQ_TYPE, std::shared_ptr<void>>      req_t;

#endif /* __CLIENT_INC_H__ */