#include "database_service.h"
#include "server_ultility.h"
#include <iostream>

Database::Database() {
    shared_memory_object::remove("client_table");
    segment = managed_shared_memory(open_or_create, "client_table", 1024);
    client_table = segment.construct<shm_map>("client_table")(std::less<key_type>(), segment.get_segment_manager());
}

Database::~Database() {
    segment.destroy<shm_map>("client_table");
}

Database* Database::getInstance() {
    static Database __instance;
    return &__instance;
}

/**
 * @brief load saved client table from file system into client table in RAM
 * @return none
 */
void Database::load_client_table() {

}

/**
 * @brief save a client info to database (overwrite to existing client or create a new one)
 * @param client_name: name of the client
 * @param client_addr: address of the client (ip and port)
 * @return none
 */
void Database::save_client_info(const std::string& client_name, const struct sockaddr_in& client_addr) {
    if (!client_name_exist(client_name)) {
        client_table->insert({client_name, std::make_shared<struct client_data_t>()});
    }
    else {
        client_table->at(client_name)->address = client_addr;
    }
}

/**
 * @brief save a new mail sent by a client
 * @param client_name: name of the client
 * @param _receiver: name of received client of this mail
 * @param _content: content of the mail
 * @return none
 */
void Database::save_sent_mail(const std::string& client_name, const std::string& _receiver, const std::string& _content) {
    if (!client_name_exist(client_name)) {
        std::cout << "client name not existed\n";
        return;
    }
    client_table->at(client_name)->sent_mailbox.emplace_back(_receiver, _content);
}

/**
 * @brief save a new mail received by a client
 * @param client_name: name of the client
 * @param _sender: name of client who has sent this mail
 * @param _content: content of the mail
 * @return none
 */
void Database::save_received_mail(const std::string& client_name, const std::string& _sender, const std::string& _content) {
    if (!client_name_exist(client_name)) {
        std::cout << "client name not existed\n";
        return;
    }
    client_table->at(client_name)->received_mailbox.emplace_back(_sender, _content);
}

/**
 * @brief delete a mail in sent mailbox of a client
 * @param client_name: name of the client
 * @param _position: position of the mail in the mailbox
 * @return none
 */
void Database::delete_sent_mail(const std::string& client_name, const unsigned short& _position) {
    if (!client_name_exist(client_name)) {
        std::cout << "client name not existed\n";
        return;
    }
    auto _sent_mailbox = client_table->at(client_name)->sent_mailbox;
    if (_position > _sent_mailbox.size()) {
        std::cout << "position out of range\n";
        return;
    }
    _sent_mailbox.erase(_sent_mailbox.begin() + _position - 1);
}

/**
 * @brief delete a mail in received mailbox of a client
 * @param client_name: name of the client
 * @param _position: position of the mail in the mailbox
 * @return none
 */
void Database::delete_received_mail(const std::string& client_name, const unsigned short& _position) {
    if (!client_name_exist(client_name)) {
        std::cout << "client name not existed\n";
        return;
    }
    auto _rcv_mailbox = client_table->at(client_name)->received_mailbox;
    if (_position > _rcv_mailbox.size()) {
        std::cout << "position out of range\n";
        return;
    }
    _rcv_mailbox.erase(_rcv_mailbox.begin() + _position - 1);
}

/**
 * @brief update name of a client
 * @param client_name: current name of the client
 * @param new_name: the new name
 * @return none
 */
void Database::update_client_name(const std::string& client_name, const std::string& new_name) {
    if (!client_name_exist(client_name)) {
        std::cout << "client name not existed\n";
        return;
    }
    if (client_name_exist(new_name)) {
        std::cout << "new name is already existed\n";
        return;
    }
    auto _client_data(client_table->at(client_name));
    client_table->erase(client_name);
    client_table->insert({new_name, _client_data});
}

/**
 * @brief get current address of a client
 * @param client_name: name of the client
 * @return pointer to the address, return null if client name is not existed
 */
const struct sockaddr_in* Database::get_client_addr(const std::string& client_name) const {
    if (!client_name_exist(client_name)) {
        std::cout << "client name not existed\n";
        return nullptr;
    }
    return &(client_table->at(client_name)->address);
}

/**
 * @brief get current sent mailbox of a client
 * @param client_name: name of the client
 * @return pointer to the sent mailbox, return null if client name is not existed
 */
const std::vector<sent_mail>* Database::get_sent_mailbox(const std::string& client_name) const {
    if (!client_name_exist(client_name)) {
        std::cout << "client name not existed\n";
        return nullptr;
    }
    return &(client_table->at(client_name)->sent_mailbox);
}

/**
 * @brief get current received mailbox of a client
 * @param client_name: name of the client
 * @return pointer to the received mailbox, return null if client name is not existed
 */
const std::vector<received_mail>* Database::get_received_mailbox(const std::string& client_name) const {
    if (!client_name_exist(client_name)) {
        std::cout << "client name not existed\n";
        return nullptr;
    }
    return &(client_table->at(client_name)->received_mailbox);
}

/**
 * @brief check if a client name has been taken or not
 * @param client_name: name of the client
 * @return checking result
 */
bool Database::client_name_exist(const std::string& client_name) const {
    return (client_table->find(client_name) == client_table->end()) ? false : true;
}

const shm_map* Database::get_client_table() const {
    return client_table;
}