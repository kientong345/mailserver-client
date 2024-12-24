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

#define SENDTO                         "sendto"
#define DELETEMAIL                     "deletemail"
#define CHANGENAME                     "changename"
#define SEARCH                         "search"
#define TERMINATE                      "term"
#define WHATISMYNAME                   "whatismyname"

typedef enum {
    REQ_UNIDENTIFY,
    REQ_SENDTO,        /* connect to ... */
    REQ_DELETEMAIL,    /* delete a mail in mailbox */
    REQ_CHANGENAME,    /* change client name in client table */
    REQ_SEARCH,        /* req to search on client table */
    REQ_TERMINATE,     /* terminate connect req(with server) */
    REQ_WHATISMYNAME   /* response the name of requester */
} REQ_TYPE;

typedef struct {
    long mtype;
    char mtext[1024];
} message_t;

typedef struct sent_mail {
    std::string receiver;
    std::string content;
    sent_mail(const std::string& _receiver, const std::string& _content) 
    : receiver(_receiver), content(_content) {}
} sent_mail;

typedef struct received_mail {
    std::string sender;
    std::string content;
    received_mail(const std::string& _sender, const std::string& _content) 
    : sender(_sender), content(_content) {}
} received_mail;

struct client_data_t {
    struct sockaddr_in address;
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