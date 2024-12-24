#ifndef __TRANSPORT_SERVICE_H__
#define __TRANSPORT_SERVICE_H__

#include "server_inc.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <string>

class Transporter {
private:
    char rcv_buf[BUF_SIZE];
    int server_fd;
    int client_fd;

    int msg_id;

    Transporter();

public:
    static Transporter* getInstance();

    /* socket service */

    void init();
    void listen_mode();
    struct sockaddr_in wait_connect_request();
    void response(const std::string& _message);
    std::string receive_request();
    void disconnect();
    void end();

    /* mailbox service to send message between clients */
    
    void open_mailbox(const std::string& _usr_name); // should pass the owner's name of this msgqueue to this func
    void close_mailbox();
    void send_to_mailbox(const std::string& _rcv_name, const std::string& _message);
    std::string receive_from_mailbox();

    ~Transporter();

};

#endif /* __TRANSPORT_SERVICE_H__ */