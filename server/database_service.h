#ifndef __DATABASE_SERVICE_H__
#define __DATABASE_SERVICE_H__

#include "server_inc.h"
#include <vector>
#include <memory>

class Database {
private:
    shm_map* client_table; // std::map<std::string, struct sockaddr_in> client_table
    managed_shared_memory segment;

    Database();

public:
    static Database* getInstance();

    void load_client_table();
    void save_client_info(const std::string& client_name, const std::string& password, const struct sockaddr_in& client_addr);
    void save_sent_mail(const std::string& client_name, const std::string& _receiver, const std::string& _content, const std::string& _sent_time);
    void save_received_mail(const std::string& client_name, const std::string& _sender, const std::string& _content, const std::string& _rcv_time);
    void delete_sent_mail(const std::string& client_name, const unsigned short& _position);
    void delete_received_mail(const std::string& client_name, const unsigned short& _position);
    void update_client_name(const std::string& client_name, const std::string& new_name);
    void update_client_password(const std::string& client_name, const std::string& new_pass);
    void update_client_address(const std::string& client_name, const struct sockaddr_in& new_address);

    const std::string* get_client_password(const std::string& client_name) const;
    const struct sockaddr_in* get_client_addr(const std::string& client_name) const;
    const std::vector<sent_mail>* get_sent_mailbox(const std::string& client_name) const;
    const std::vector<received_mail>* get_received_mailbox(const std::string& client_name) const;

    bool client_name_exist(const std::string& client_name) const;
    const shm_map* get_client_table() const;

    ~Database();

};

#endif /* __DATABASE_SERVICE_H__ */