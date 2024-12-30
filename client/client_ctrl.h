#ifndef __CLIENT_CTRL_H__
#define __CLIENT_CTRL_H__

#include "client_inc.h"
#include "client_transport.h"
#include "client_mailbox.h"
#include "client_manager.h"

class Client_Ctrl {
private:
    ClientTransporter* _transporter;
    Mailbox* _sent_mailbox;
    Mailbox* _received_mailbox;
    ClientManager* _manager;

    CTRL_MODE _current_mode;
    
public:
    Client_Ctrl();
    ~Client_Ctrl();

    void client_init();
    void client_main();

private:
    void user_handler();
    void ui_handler();
    void command_handler();
    void execute_request(const req_t& _request);
    void login();

};

#endif /* __CLIENT_CTRL_H__ */