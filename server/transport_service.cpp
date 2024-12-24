#include "transport_service.h"
#include "server_ultility.h"
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>

Transporter::Transporter()
: server_fd(-1),
  client_fd(-1),
  msg_id(-1) {
    memset(rcv_buf, '\0', sizeof(rcv_buf));
}

Transporter::~Transporter() {

}

Transporter* Transporter::getInstance() {
    static Transporter __instance;
    return &__instance;
}

/**
 * @brief init socket for server
 * @return none
 */
void Transporter::init() {
    struct sockaddr_in server_addr;
    int opt = 1;
    server_fd = ::socket(AF_INET, SOCK_STREAM, 0);

    ::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t)sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    ::bind(server_fd, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr));
}

/**
 * @brief change server to 'listening' state to hear any connecting request
 * @return none
 */
void Transporter::listen_mode() {
    ::listen(server_fd, MAXIMUM_CONNECTED_CLIENT);
    std::cout << "server is listening ...\n";
}

/**
 * @brief server wait until received a connecting request, then accept that request
 * @brief then update client fd (this server process now serve this client)
 * @return address of the connected client (ip and port)
 */
struct sockaddr_in Transporter::wait_connect_request() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    client_fd = ::accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    std::cout << "connected to " << inet_ntoa(client_addr.sin_addr) << " - port " << std::to_string(ntohs(client_addr.sin_port)) << std::endl;
    return client_addr;
}

/**
 * @brief response to the client this server is serving
 * @param _message: response message
 * @return none
 */
void Transporter::response(const std::string& _message) {
    ::send(client_fd, _message.c_str(), _message.size()+1, 0);
}

/**
 * @brief wait until received a request from the client this server is serving
 * @return received message
 */
std::string Transporter::receive_request() {
    std::string recv_msg;
    int bytercv = ::recv(client_fd, rcv_buf, BUF_SIZE, 0);
    rcv_buf[bytercv] = '\0';
    recv_msg += rcv_buf;
    rcv_buf[0] = '\0';
    return recv_msg;
}

/**
 * @brief disconnect to the client this server is serving
 * @return none
 */
void Transporter::disconnect() {
    ::close(client_fd);
}

/**
 * @brief close the socket of this server
 * @return none
 */
void Transporter::end() {
    ::close(server_fd);
}

/**
 * @brief open the mailbox for the client this server is serving to communicate with other clients
 * @param _usr_name: name of the owner of this mailbox
 * @return none
 */
void Transporter::open_mailbox(const std::string& _usr_name) {
    msg_id = ::msgget(getKeyfromName(_usr_name), 0666|IPC_CREAT);
}

/**
 * @brief close the mailbox for the client this server is serving
 * @return none
 */
void Transporter::close_mailbox() {
    ::msgctl(msg_id, IPC_RMID, NULL);
}

/**
 * @brief send a message to the mailbox of another client
 * @param _rcv_name: name of the receiver
 * @param _message: sent message
 * @return none
 */
void Transporter::send_to_mailbox(const std::string& _rcv_name, const std::string& _message) {
    int _rcv_msg_id = ::msgget(getKeyfromName(_rcv_name), 0666|IPC_CREAT);
    static message_t sent_message; // not thread safe, still ok if only 1 thread call this func
    sent_message.mtype = 1;
    sent_message.mtext[0] = '\0';
    ::strcpy(sent_message.mtext, _message.c_str());
    ::msgsnd(_rcv_msg_id, &sent_message, sizeof(sent_message.mtext), 0);
}

/**
 * @brief wait until read something from the mailbox of the client this server is serving
 * @return read message
 */
std::string Transporter::receive_from_mailbox() {
    static message_t rcv_message; // not thread safe, still ok if only 1 thread call this func
    rcv_message.mtype = 1;
    rcv_message.mtext[0] = '\0';
    int bytercv = ::msgrcv(msg_id, &rcv_message, sizeof(rcv_message.mtext), 1, 0);
    rcv_message.mtext[bytercv] = '\0';
    return std::string(rcv_message.mtext);
}