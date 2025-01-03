#ifndef __SERVER_INC_H__
#define __SERVER_INC_H__

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <utility>
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <arpa/inet.h>

#define PORT                           8008
#define BUF_SIZE                       1024
#define MAXIMUM_CONNECTED_CLIENT       5

#define NOT_FOUND                      404

#define LOGIN                          "login"
#define REGISTER                       "register"
#define LOGOUT                         "logout"
#define SENDTO                         "sendto"
#define DELETEMAIL                     "deletemail"
#define CHANGENAME                     "changename"
#define SEARCH                         "search"
#define TERMINATE                      "term"
#define WHATISMYNAME                   "whatismyname"
#define CHANGEPASSWORD                 "changepassword"

#define SENT_MAILBOX                   "sent_mailbox"
#define RCV_MAILBOX                    "received_mailbox"
#define LOGIN_SUCCEED                  (std::string("login succeed"))
#define LOGIN_FAILED                   (std::string("login failed"))
#define REGISTER_SUCCEED               (std::string("register succeed"))
#define REGISTER_FAILED                (std::string("register failed"))

#define __CURRENT_TIME__               (std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()))

typedef enum {
    REQ_UNIDENTIFY,
    REQ_LOGIN,
    REQ_REGISTER,
    REQ_LOGOUT,
    REQ_SENDTO,        /* connect to ... */
    REQ_DELETEMAIL,    /* delete a mail in mailbox */
    REQ_CHANGENAME,    /* change client name in client table */
    REQ_SEARCH,        /* req to search on client table */
    REQ_TERMINATE,     /* terminate connect req(with server) */
    REQ_WHATISMYNAME,   /* response the name of requester */
    REQ_CHANGEPASSWORD
} REQ_TYPE;

typedef enum {
    E_SUCCESS,
    E_TERM,
    E_LOGIN_FAILED,
    E_LOGIN_SUCCEED,
    E_REGISTER_FAILED,
    E_REGISTER_SUCCEED
} ERROR_CODE;

typedef enum {
    OFFLINE,
    ONLINE
} USER_STATUS;

typedef struct {
    long mtype;
    char mtext[1024];
} message_t;

typedef struct sent_mail {
    std::string receiver;
    std::string content;
    uint64_t sent_time;
    sent_mail(const std::string& _receiver, const std::string& _content, uint64_t _sent_time) 
    : receiver(_receiver), content(_content), sent_time(_sent_time) {}
} sent_mail;

typedef struct received_mail {
    std::string sender;
    std::string content;
    uint64_t rcv_time;
    received_mail(const std::string& _sender, const std::string& _content, uint64_t _rcv_time) 
    : sender(_sender), content(_content), rcv_time(_rcv_time) {}
} received_mail;

struct client_data_t {
    USER_STATUS status;
    struct sockaddr_in address;
    std::string password;
    std::vector<sent_mail> sent_mailbox;
    std::vector<received_mail> received_mailbox;
};

using namespace boost::interprocess;
typedef std::string                                                 key_type;
typedef std::shared_ptr<struct client_data_t>                       val_type;
typedef std::pair<const key_type, val_type>                         elm_type;
typedef allocator<elm_type, managed_shared_memory::segment_manager> shm_allocator;
typedef map<key_type, val_type, std::less<key_type>, shm_allocator> shm_map;

typedef std::pair<REQ_TYPE, std::shared_ptr<void>>                  req_t;

#endif /* __SERVER_INC_H__ */