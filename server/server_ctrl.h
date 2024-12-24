#ifndef __SERVER_CTRL_H__
#define __SERVER_CTRL_H__

#include "server_inc.h"
#include "database_service.h"
#include "transport_service.h"

class Server_Ctrl {
private:
    Database* _database;
    Transporter* _transporter;

    std::string _client_name; // i am serving this guy!
    struct sockaddr_in _client_addr; // address of 'this guy'
    bool is_serving; // am i serving or not?

public:
    Server_Ctrl();
    ~Server_Ctrl();

    void server_init();
    void server_main();
    void server_shutdown();

private:
    void client_handler();
    void mailbox_handler();
    int execute_request(const req_t& _request);

};

#endif /* __SERVER_CTRL_H__ */