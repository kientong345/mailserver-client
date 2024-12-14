#ifndef __MAILCLIENT_CTRL_H__
#define __MAILCLIENT_CTRL_H__

#include <sys/types.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <utility>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "mailclient_inc.h"

class Client {
public:
    Client();
    ~Client();

    ERROR_CODE setup(const std::string& _ipaddress = "", const uint16_t& _port = 0);
    ERROR_CODE connect(const std::string& _ipaddress, const uint16_t& _port);
    ERROR_CODE sendRequest(const std::string& _message);
    ERROR_CODE receiveResponse(std::string& _res); // to be delete
    void start();
    void shutdown();
    const sent_mail* checkmail(std::vector<sent_mail> _sent_mailbox, uint16_t _mail_position) const;
    const received_mail* checkmail(std::vector<received_mail> _rcv_mailbox, uint16_t _mail_position) const;

private:
    char rcv_buf[BUF_SIZE];
    char send_buf[BUF_SIZE];
    std::vector<sent_mail> sent_mailbox;
    std::vector<received_mail> received_mailbox;
    int client_fd;
    std::thread send_thread;
    std::thread recv_thread;
    std::mutex send_mut;
    std::condition_variable send_cv;

private:
    void send_thread_func();
    void recv_thread_func();
    std::pair<REQ_TYPE, std::shared_ptr<void>> parseRequest(const std::string& _message);
    void executeRequest(const std::pair<REQ_TYPE, std::shared_ptr<void>>& _req);
    void updateMyName();

};

#endif /* __MAILCLIENT_CTRL_H__ */