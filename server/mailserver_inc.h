#ifndef __MAILSERVER_INC_H__
#define __MAILSERVER_INC_H__

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <utility>
#include <string>
#include <arpa/inet.h>

#define PORT                           8008
#define BUF_SIZE                       1024
#define MAXIMUM_CONNECTED_CLIENT       5

#define NOT_FOUND                      404

typedef enum {
    E_SUCCESS,
    E_SOCKET_FAILED,
    E_OPT_FAILED,
    E_BIND_FAILED,
    E_LISTEN_FAILED,
    E_SAVE_FAILED
} ERROR_CODE;

typedef enum {
    REQ_UNIDENTIFY,
    REQ_SENDTO,        /* connect to ... */
    REQ_CHANGENAME,    /* change client name in client table */
    REQ_SEARCH,        /* req to search on client table */
    REQ_TERMINATE,     /* terminate connect req(with server) */
    REQ_WHATISMYNAME   /* response the name of requester */
} REQ_TYPE;

struct client_info_t{
    std::string client_name;
    struct sockaddr_in client_addr;
};

using namespace boost::interprocess;
typedef int                                                         key_type;
typedef struct client_info_t                                        val_type;
typedef std::pair<const key_type, val_type>                         elm_type;
typedef allocator<elm_type, managed_shared_memory::segment_manager> shm_allocator;
typedef map<key_type, val_type, std::less<key_type>, shm_allocator> shm_map;

typedef struct {
    long mtype;
    char mtext[1024];
} message_t;

#endif /* __MAILSERVER_INC_H__ */