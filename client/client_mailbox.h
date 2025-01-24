#ifndef __CLIENT_MAILBOX_H__
#define __CLIENT_MAILBOX_H__

#include "client_inc.h"
#include <vector>
#include <list>
#include <utility>
#include <shared_mutex>
#include <memory>
#include <mutex>
#include <condition_variable>

class MailboxManager;

template<class MailType>
class Mailbox {
protected:
    std::vector<MailType> mailbox;  
    std::shared_mutex mailbox_mut;
    MailboxManager* _mail_observer;

    Mailbox();

public:
    static Mailbox* getInstance();
    void save_mail(const std::string& _header, const std::string _content, uint64_t _time);
    void delete_mail(uint16_t _position);
    const MailType* get_mail(uint16_t _position);
    const std::vector<MailType>& get_mailbox();
    uint16_t number_of_mail();

    void set_observer(MailboxManager* _observer);

    ~Mailbox();

};

class MailboxManager {
private:
    Mailbox<sent_mail>* sent_mailbox;
    Mailbox<received_mail>* received_mailbox;
    std::unique_ptr<sent_mail> latest_sent_mail;
    std::unique_ptr<received_mail> latest_recv_mail;

    std::mutex mut;
    std::condition_variable cond_var;

    bool force_quit;

public:
    MailboxManager(Mailbox<sent_mail>* _sent_mailbox, Mailbox<received_mail>* _received_mailbox);
    ~MailboxManager();

    void update_new_mail(const sent_mail& _new_mail);
    void update_new_mail(const received_mail& _new_mail);

    std::vector<chat_line> get_conversation(const std::string& user, uint16_t msg_num = 0xFFFF);
    chat_line get_latest_chatline(const std::string& user);

    void unblock();

};

#endif /* __CLIENT_MAILBOX_H__ */