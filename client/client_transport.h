#ifndef __CLIENT_TRANSPORT_H__
#define __CLIENT_TRANSPORT_H__

#include "client_inc.h"
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

class ClientTransporter {
private:
    char rcv_buf[BUF_SIZE];
    char send_buf[BUF_SIZE];
    int client_fd;
    std::thread send_thread;
    std::thread recv_thread;
    std::mutex send_mut;
    std::condition_variable send_cv;

    ClientTransporter();
    
public:
    static ClientTransporter* getInstance();

    void init();
    void connect_to_server(const std::string& _ipaddress, uint16_t _port);
    void send_request(const std::string& _message);
    std::string receive_response();
    void end();

    ~ClientTransporter();

private:
    void send_thread_func();
    void recv_thread_func();

};

#endif /* __CLIENT_TRANSPORT_H__ */