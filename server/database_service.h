#ifndef __DATABASE_SERVICE_H__
#define __DATABASE_SERVICE_H__

#include "server_inc.h"
#include <vector>
#include <memory>

class Database {
protected:
    shm_map* client_table; // std::map<std::string, struct sockaddr_in> client_table
    managed_shared_memory segment;
    Database();

public:
    virtual ~Database();

    void load_client_table();

    virtual void save_client_info(const std::string& client_name, const std::string& password, const struct sockaddr_in& client_addr) = 0;
    virtual void save_sent_mail(const std::string& client_name, const std::string& _receiver, const std::string& _content, uint64_t _sent_time) = 0;
    virtual void save_received_mail(const std::string& client_name, const std::string& _sender, const std::string& _content, uint64_t _rcv_time) = 0;
    virtual void delete_sent_mail(const std::string& client_name, const unsigned short& _position) = 0;
    virtual void delete_received_mail(const std::string& client_name, const unsigned short& _position) = 0;
    virtual void update_client_name(const std::string& client_name, const std::string& new_name) = 0;
    virtual void update_client_password(const std::string& client_name, const std::string& new_pass) = 0;
    virtual void update_client_address(const std::string& client_name, const struct sockaddr_in& new_address) = 0;

    const std::string* get_client_password(const std::string& client_name) const;
    const struct sockaddr_in* get_client_addr(const std::string& client_name) const;
    const std::vector<sent_mail>* get_sent_mailbox(const std::string& client_name) const;
    const std::vector<received_mail>* get_received_mailbox(const std::string& client_name) const;

    bool client_name_exist(const std::string& client_name) const;
    const shm_map* get_client_table() const;

};

class Soft_Database : virtual public Database {
private:
    Soft_Database();
public:
    static Soft_Database* getInstance();

    void save_client_info(const std::string& client_name, const std::string& password, const struct sockaddr_in& client_addr) override;
    void save_sent_mail(const std::string& client_name, const std::string& _receiver, const std::string& _content, uint64_t _sent_time) override;
    void save_received_mail(const std::string& client_name, const std::string& _sender, const std::string& _content, uint64_t _rcv_time) override;
    void delete_sent_mail(const std::string& client_name, const unsigned short& _position) override;
    void delete_received_mail(const std::string& client_name, const unsigned short& _position) override;
    void update_client_name(const std::string& client_name, const std::string& new_name) override;
    void update_client_password(const std::string& client_name, const std::string& new_pass) override;
    void update_client_address(const std::string& client_name, const struct sockaddr_in& new_address) override;

    ~Soft_Database() override;
};

class Hard_Database : virtual public Database {
private:
    Hard_Database();
public:
    static Hard_Database* getInstance();

    void save_client_info(const std::string& client_name, const std::string& password, const struct sockaddr_in& client_addr) override;
    void save_sent_mail(const std::string& client_name, const std::string& _receiver, const std::string& _content, uint64_t _sent_time) override;
    void save_received_mail(const std::string& client_name, const std::string& _sender, const std::string& _content, uint64_t _rcv_time) override;
    void delete_sent_mail(const std::string& client_name, const unsigned short& _position) override;
    void delete_received_mail(const std::string& client_name, const unsigned short& _position) override;
    void update_client_name(const std::string& client_name, const std::string& new_name) override;
    void update_client_password(const std::string& client_name, const std::string& new_pass) override;
    void update_client_address(const std::string& client_name, const struct sockaddr_in& new_address) override;

    ~Hard_Database() override;
};

#endif /* __DATABASE_SERVICE_H__ */