#ifndef __CLIENT_INC_H__
#define __CLIENT_INC_H__

#include <chrono>
#include <thread>
#include <string>
#include <utility>
#include <memory>

#define SERVER_IP           "127.0.0.1"
#define SERVER_PORT         8008
#define BUF_SIZE            1024

#define OLDEST_MAIL         (1U)
#define NEWEST_MAIL         (0xFFFFU)

#define WAIT_MS(TIME)       (std::this_thread::sleep_for(std::chrono::milliseconds(TIME)))

#define CLIENT_IP           "127.0.0.1"
#define CLIENT_PORT         7007
//#define __CUSTOM_ADDRESS__

#define REGISTATION_STATE   (std::make_shared<Registation_State>())
#define MENU_STATE          (std::make_shared<Menu_State>())

#define CURRENT_TIME        (std::string())

typedef enum {
    REQ_UNIDENTIFY,
    REQ_SENDTO,
    REQ_CHANGENAME,
    REQ_SEARCH,
    REQ_TERM,
    REQ_WHATISMYNAME,
    REQ_TOSERVER,
    REQ_SHOWMAIL,
    REQ_DELETEMAIL
} REQ_TYPE;

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
} sent_mail;

typedef struct {
    std::string sender;
    std::string content;
} received_mail;

typedef std::pair<REQ_TYPE, std::shared_ptr<void>>      req_t;

#endif /* __CLIENT_INC_H__ */