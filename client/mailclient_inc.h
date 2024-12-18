#ifndef __MAILCLIENT_INC_H__
#define __MAILCLIENT_INC_H__

#include <chrono>
#include <thread>
#include <string>

#define SERVER_IP       "127.0.0.1"
#define SERVER_PORT     8008
#define BUF_SIZE        1024

#define OLDEST_MAIL     (1U)
#define NEWEST_MAIL     (0xFFFFU)

#define WAIT_MS(TIME)   (std::this_thread::sleep_for(std::chrono::milliseconds(TIME)))

typedef enum {
    E_SUCCESS,
    E_SOCKET_FAILED,
    E_OPT_FAILED,
    E_BIND_FAILED,
    E_CONNECT_FAILED,
    E_SEND_FAILED,
    E_RCV_FAILED
} ERROR_CODE;

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

typedef struct {
    std::string receiver;
    std::string content;
} sent_mail;

typedef struct {
    std::string sender;
    std::string content;
} received_mail;

#endif /* __MAILCLIENT_INC_H__ */