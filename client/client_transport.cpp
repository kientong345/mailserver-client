#include "client_transport.h"
#include <arpa/inet.h>
#include <iostream>

ClientTransporter::ClientTransporter()
: client_fd(-1),
  receive_task(nullptr) {
    send_buf[0] = '\0';
    rcv_buf[0] = '\0';
}

ClientTransporter::~ClientTransporter() {

}

ClientTransporter* ClientTransporter::getInstance() {
    static ClientTransporter __instance;
    return &__instance;
}

/**
 * @brief init socket for client, then prepare environment to communicate with server
 * @return none
 */
void ClientTransporter::init() {
    client_fd = ::socket(AF_INET, SOCK_STREAM, 0);

#ifdef __CUSTOM_ADDRESS__
    struct sockaddr_in client_addr;
    uint32_t client_ip = 0;
    inet_pton(AF_INET, CLIENT_IP, &client_ip); /* change client ip in need */
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = client_ip;
    client_addr.sin_port = htons(CLIENT_PORT);

    ::bind(client_fd, (struct sockaddr*)&client_addr, (socklen_t)sizeof(client_addr));
#endif /* __CUSTOM_ADDRESS__ */
}

/**
 * @brief wait until connected to a server
 * @param _ipaddress: ip address of a server to be connected
 * @param _port: port of a server to be connected
 * @return none
 */
void ClientTransporter::connect_to_server(const std::string& _ipaddress, uint16_t _port) {
    /* indicate server */
    struct sockaddr_in server_addr;
    uint32_t server_ip;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, _ipaddress.c_str(), &server_ip);
    server_addr.sin_addr.s_addr = server_ip;
    server_addr.sin_port = htons(_port);

    /* connect to that server */
    ::connect(client_fd, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr));
    // std::cout << "connected to " + _ipaddress + "-port:" + std::to_string(_port) + "\n";
    /* prepare 2 threads to send and receive data with server */
    send_thread = std::thread(&ClientTransporter::send_thread_func, this);
    recv_thread = std::thread(&ClientTransporter::recv_thread_func, this);
}

/**
 * @brief send a message to server
 * @param _message: message to be sent
 * @return none
 */
void ClientTransporter::send_request(const std::string& _message) {
    {
        std::lock_guard<std::mutex> glock(send_mut);
        std::copy(_message.begin(), _message.end(), send_buf);
        send_buf[_message.size()] = '\0';
    }
    send_cv.notify_one();
    WAIT_MS(10);
}

/**
 * @brief wait until there is a message from server, then return it
 * @return message from server
 */
std::string ClientTransporter::receive_response() {
    std::string _res = "";
    while (rcv_buf[0] == '\0') {
        WAIT_MS(10); // avoid busy-waiting
    }
    _res += rcv_buf;
    rcv_buf[0] = '\0';
    return _res;
}

void ClientTransporter::end() {

}

void ClientTransporter::set_task_on_receive_data(std::function<void(void)> task_func) {
    receive_task = task_func;
}

/**
 * @brief awake whenever send_request execute, and send the message from send_request to server
 * @return none
 */
void ClientTransporter::send_thread_func() {
    while(1) {
        if (client_fd < 0) {
            WAIT_MS(1000); // avoid busy-waiting
            continue;
        }

        std::unique_lock<std::mutex> ulock(send_mut);
        send_cv.wait(ulock, [this](){return send_buf[0] != '\0';});
        ::send(client_fd, send_buf, sizeof(send_buf), 0);
        send_buf[0] = '\0';
    }
}
    
/**
 * @brief wait until read a message from server, then store it until receive_response read it
 * @return none
 */
void ClientTransporter::recv_thread_func() {
    while(1) {
        if (client_fd < 0) {
            WAIT_MS(1000); // avoid busy-waiting
            continue;
        }

        int bytercv = ::recv(client_fd, rcv_buf, BUF_SIZE, 0);
        rcv_buf[bytercv] = '\0';
        // execute any registered task
        if (receive_task) receive_task();
    }
}