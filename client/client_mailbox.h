#ifndef __CLIENT_MAILBOX_H__
#define __CLIENT_MAILBOX_H__

#include "client_inc.h"
#include <vector>
#include <utility>
#include <shared_mutex>

class Mailbox {
protected:
    std::vector<sent_mail> sent_mailbox;
    std::vector<received_mail> received_mailbox;    
    std::shared_mutex sent_mailbox_mut;
    std::shared_mutex rcv_mailbox_mut;

public:
    virtual void save_mail(const std::string& _header, const std::string _content, uint64_t _time) = 0;
    virtual void delete_mail(uint16_t _position) = 0;
    virtual const void* get_mail(uint16_t _position) = 0;
    virtual uint16_t number_of_mail() = 0;

    std::vector<std::pair<std::string, std::string>> get_conversation(const std::string& user1, const std::string& user2) const;

};

class SentMailbox : public Mailbox {
private:
    SentMailbox();

public:
    static SentMailbox* getInstance();

    void save_mail(const std::string& _header, const std::string _content, uint64_t _time) override;
    void delete_mail(uint16_t _position) override;
    const sent_mail* get_mail(uint16_t _position) override;
    uint16_t number_of_mail() override;

    ~SentMailbox();

};

class ReceivedMailbox : public Mailbox {
private:
    ReceivedMailbox();

public:
    static ReceivedMailbox* getInstance();

    void save_mail(const std::string& _header, const std::string _content, uint64_t _time) override;
    void delete_mail(uint16_t _position) override;
    const received_mail* get_mail(uint16_t _position) override;
    uint16_t number_of_mail() override;

    ~ReceivedMailbox();

};

#endif /* __CLIENT_MAILBOX_H__ */