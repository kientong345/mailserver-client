#include "client_mailbox.h"
#include <iostream>
#include <algorithm>

template<class MailType>
Mailbox<MailType>::Mailbox()
: _mail_observer(nullptr) {

}

template<class MailType>
Mailbox<MailType>::~Mailbox() {

}

template<class MailType>
Mailbox<MailType>* Mailbox<MailType>::getInstance() {
    static Mailbox<MailType> __instance;
    return &__instance;
}

template<class MailType>
void Mailbox<MailType>::save_mail(const std::string& _header, const std::string _content, uint64_t _time) {
    std::lock_guard<std::shared_mutex> glock(mailbox_mut);
    mailbox.emplace_back(_header, _content, _time);
    if (_mail_observer) {
        _mail_observer->update_new_mail(mailbox.back());
    }
}

template<class MailType>
void Mailbox<MailType>::delete_mail(uint16_t _position) {
    std::lock_guard<std::shared_mutex> slock(mailbox_mut);
    if ((_position > number_of_mail()) || (_position < 1)) {
        std::cout << "mail out of range\n";
        return;
    }
    mailbox.erase(mailbox.begin() + _position - 1);
}

template<class MailType>
const MailType* Mailbox<MailType>::get_mail(uint16_t _position) {
    MailType* ret;
    std::shared_lock<std::shared_mutex> slock(mailbox_mut);
    if ((_position > number_of_mail()) || (_position < 1)) {
        std::cout << "mail out of range\n";
        slock.unlock();
        return nullptr;
    }
    ret = &mailbox[_position-1];
    slock.unlock();
    return ret;
}

template<class MailType>
const std::vector<MailType>& Mailbox<MailType>::get_mailbox() {
    return mailbox;
}

template<class MailType>
uint16_t Mailbox<MailType>::number_of_mail() {
    return mailbox.size();
}

template<class MailType>
void Mailbox<MailType>::set_observer(MailboxManager* _observer) {
    _mail_observer = _observer;
}

MailboxManager::MailboxManager(const std::string& _username, Mailbox<sent_mail>* _sent_mailbox, Mailbox<received_mail>* _received_mailbox)
: _current_username(_username), sent_mailbox(_sent_mailbox), received_mailbox(_received_mailbox),
  latest_sent_mail(nullptr), latest_recv_mail(nullptr),
  force_quit(false) {
    sent_mailbox->set_observer(this);
    received_mailbox->set_observer(this);
}

MailboxManager::~MailboxManager() {
    
}

/**
 * @brief force the function 'get_latest_chatline' exit from waiting state, must be call in another thread
 * @return none
 */
void MailboxManager::unblock() {
    {
        std::lock_guard<std::mutex> glock(mut);
        force_quit = true;
    }
    cond_var.notify_all();
}

void MailboxManager::update_new_mail(const sent_mail& _new_mail) {
    {
        std::lock_guard<std::mutex> glock(mut);
        latest_sent_mail = std::make_unique<sent_mail>(_new_mail);
    }
    cond_var.notify_all();
}

void MailboxManager::update_new_mail(const received_mail& _new_mail) {
    {
        std::lock_guard<std::mutex> glock(mut);
        latest_recv_mail = std::make_unique<received_mail>(_new_mail);
    }
    cond_var.notify_all();
}

/**
 * @brief get the chat history between 2 users
 * @param user: the user who is chatting with _current_user
 * @param msg_num: number of messages to be retrieved, default is 0xFFFF
 * @return a vector contain chat history between 2 user, sorting by sent/rcv time
 */
std::vector<chat_line> MailboxManager::get_conversation(const std::string& user, uint16_t msg_num) {
    std::vector<chat_line> chat_history;
    uint16_t counter = 0;
    for (const auto& _mail : sent_mailbox->get_mailbox()) {
        if (_mail.receiver == user) {
            chat_history.emplace_back(_current_username, _mail.content, _mail.sent_time);
            ++counter;
            if (counter >= msg_num) break;
        }
    }
    counter = 0;
    for (const auto& _mail : received_mailbox->get_mailbox()) {
        if (_mail.sender == user) {
            chat_history.emplace_back(_mail.sender, _mail.content, _mail.rcv_time);
            ++counter;
            if (counter >= msg_num) break;
        }
    }
    std::stable_sort(chat_history.begin(), chat_history.end(), [](const chat_line& line1, const chat_line& line2){
        return line1.chat_time < line2.chat_time;
    });

    {
        std::lock_guard<std::mutex> glock(mut);
        latest_sent_mail = nullptr;
        latest_recv_mail = nullptr;
    }

    if ((msg_num != 0xFFFF) && (chat_history.size() > msg_num)) {
        return std::vector<chat_line>(chat_history.end() - msg_num, chat_history.end());
    }
    else {
        return chat_history;
    }
}

/**
 * @brief get the latest chat line with username filter, then clean retrieved chat line
 * @param user: the user who is chatting with _current_user
 * @return latest chat line with username filter
 */
chat_line MailboxManager::get_latest_chatline(const std::string& user) {
    chat_line ret("", "", 0);
    std::unique_lock<std::mutex> ulock(mut);
    // wait until there is a mail to retrieve
    cond_var.wait(ulock, [this, &user]() {
        return (latest_sent_mail && (latest_sent_mail->receiver == user)) ||
               (latest_recv_mail && (latest_recv_mail->sender == user)) ||
               (force_quit == true);
    });
    if (force_quit) {
        return ret;
    }

    if (latest_sent_mail && latest_recv_mail) {
        if (latest_sent_mail->sent_time <= latest_recv_mail->rcv_time) {
            ret = {_current_username, latest_sent_mail->content, latest_sent_mail->sent_time};
            latest_sent_mail = nullptr;
        }
        else if (latest_sent_mail->sent_time > latest_recv_mail->rcv_time) {
            ret = {latest_recv_mail->sender, latest_recv_mail->content, latest_recv_mail->rcv_time};
            latest_recv_mail = nullptr;
        }
    }
    else if (!latest_sent_mail && latest_recv_mail) {
        ret = {latest_recv_mail->sender, latest_recv_mail->content, latest_recv_mail->rcv_time};
        latest_recv_mail = nullptr;
    }
    else if (latest_sent_mail && !latest_recv_mail) {
        ret = {_current_username, latest_sent_mail->content, latest_sent_mail->sent_time};
        latest_sent_mail = nullptr;
    }
    return ret;
}

template class Mailbox<sent_mail>;
template class Mailbox<received_mail>;