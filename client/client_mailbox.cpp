#include "client_mailbox.h"
#include <iostream>
#include <algorithm>

template<class MailType>
Mailbox<MailType>::Mailbox() {

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
const MailType* Mailbox<MailType>::get_mail(uint16_t _position) const {
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
const std::vector<MailType>& Mailbox<MailType>::get_mailbox() const {
    return mailbox;
}

template<class MailType>
uint16_t Mailbox<MailType>::number_of_mail() const {
    return mailbox.size();
}

MailboxManager::MailboxManager(Mailbox<sent_mail>* _sent_mailbox, Mailbox<received_mail>* _received_mailbox)
: sent_mailbox(_sent_mailbox), received_mailbox(_received_mailbox) {

}

MailboxManager::~MailboxManager() {

}

/**
 * @brief get the chat history between 2 users
 * @param user1: current user who call this function
 * @param user2: the user who is chatting with user1
 * @return a vector contain chat history between 2 user, sorting by sent/rcv time
 */
std::vector<chat_line> MailboxManager::get_conversation(const std::string& user1, const std::string& user2) const {
    std::vector<chat_line> chat_history;
    for (const auto& _mail : sent_mailbox->get_mailbox()) {
        if (_mail.receiver == user2) {
            chat_history.emplace_back(_mail.receiver, _mail.content, _mail.sent_time);
        }
    }
    for (const auto& _mail : received_mailbox->get_mailbox()) {
        if (_mail.sender == user1) {
            chat_history.emplace_back(_mail.sender, _mail.content, _mail.rcv_time);
        }
    }
    std::stable_sort(chat_history.begin(), chat_history.end(), [](const chat_line& line1, const chat_line& line2){
        return line1.chat_time < line2.chat_time;
    });

    return chat_history;
}

template class Mailbox<sent_mail>;
template class Mailbox<received_mail>;