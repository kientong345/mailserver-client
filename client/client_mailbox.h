#ifndef __CLIENT_MAILBOX_H__
#define __CLIENT_MAILBOX_H__

#include "client_inc.h"
#include <vector>
#include <utility>
#include <shared_mutex>

template<class MailType>
class Mailbox {
protected:
    std::vector<MailType> mailbox;  
    std::shared_mutex mailbox_mut;

    Mailbox();

public:
    static Mailbox* getInstance();
    void save_mail(const std::string& _header, const std::string _content, uint64_t _time);
    void delete_mail(uint16_t _position);
    const MailType* get_mail(uint16_t _position);
    std::vector<MailType>& get_mailbox();
    uint16_t number_of_mail();
    std::vector<chat_line> get_conversation(const std::string& user1, const std::string& user2) const;

    ~Mailbox();

};

class MailboxManager {
private:
    Mailbox<sent_mail>* sent_mailbox;
    Mailbox<received_mail>* received_mailbox;
public:
    MailboxManager(Mailbox<sent_mail>* _sent_mailbox, Mailbox<received_mail>* _received_mailbox);
    ~MailboxManager();

    std::vector<chat_line> get_conversation(const std::string& user1, const std::string& user2);

};

#endif /* __CLIENT_MAILBOX_H__ */