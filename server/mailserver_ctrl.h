#ifndef __MAILSERVER_CTRL_H__
#define __MAILSERVER_CTRL_H__

#include <sys/types.h>
#include <arpa/inet.h>
#include <map>
#include <string>
#include <memory>
#include "mailserver_inc.h"

class Server {
public:
    ~Server();

    static Server* getInstance();
    ERROR_CODE setup();
    ERROR_CODE start();
    void shutdown();
    ERROR_CODE restart();

private:
    char rcv_buf[BUF_SIZE];
    int server_fd;
    shm_map* client_table; // std::map<std::string, struct sockaddr_in> client_table
    managed_shared_memory segment;
    Server();

private:
    int saveClientInfo(const struct sockaddr_in& _client_addr, const std::string& _client_name = "");
    std::pair<REQ_TYPE, std::shared_ptr<void>> parseRequest(const std::string& _message) const;
    int getKeyfromName(const std::string& client_name) const;
    void client_handler(const int& client_fd, const int& client_key);
    void msgque_handler(const int& client_fd, const int& msg_id);

};

#endif /* __MAILSERVER_CTRL_H__ */