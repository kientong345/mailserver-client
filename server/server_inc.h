#ifndef __SERVER_INC_H__
#define __SERVER_INC_H__

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <utility>
#include <string>
#include <memory>
#include <vector>
#include <arpa/inet.h>

#define PORT                           8008
#define BUF_SIZE                       1024
#define MAXIMUM_CONNECTED_CLIENT       5

#define NOT_FOUND                      404

#define LOGIN                          "login"
#define REGISTER                       "register"
#define SENDTO                         "sendto"
#define DELETEMAIL                     "deletemail"
#define CHANGENAME                     "changename"
#define SEARCH                         "search"
#define TERMINATE                      "term"
#define WHATISMYNAME                   "whatismyname"
#define CHANGEPASSWORD                 "changepassword"

#define SENT_MAILBOX                   "sent_mailbox"
#define RCV_MAILBOX                    "received_mailbox"

typedef enum {
    REQ_UNIDENTIFY,
    REQ_LOGIN,
    REQ_REGISTER,
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

typedef struct {
    long mtype;
    char mtext[1024];
} message_t;

typedef struct sent_mail {
    std::string receiver;
    std::string content;
    std::string sent_time;
    sent_mail(const std::string& _receiver, const std::string& _content, const std::string& _sent_time) 
    : receiver(_receiver), content(_content), sent_time(_sent_time) {}
} sent_mail;

typedef struct received_mail {
    std::string sender;
    std::string content;
    std::string rcv_time;
    received_mail(const std::string& _sender, const std::string& _content, const std::string& _rcv_time) 
    : sender(_sender), content(_content), rcv_time(_rcv_time) {}
} received_mail;

struct client_data_t {
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