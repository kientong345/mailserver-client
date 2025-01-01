#include "client_mailbox.h"
#include <iostream>

std::vector<std::pair<std::string, std::string>> Mailbox::get_conversation(const std::string& user1, const std::string& user2) const {
    std::vector<std::pair<std::string, std::string>> chat_history;
    chat_history.emplace_back();

    return chat_history;
}

SentMailbox::SentMailbox()
: Mailbox() {

}

SentMailbox::~SentMailbox() {

}

SentMailbox* SentMailbox::getInstance() {
    static SentMailbox __instance;
    return &__instance;
}

void SentMailbox::save_mail(const std::string& _header, const std::string _content, uint64_t _time) {
    std::lock_guard<std::shared_mutex> glock(sent_mailbox_mut);
    sent_mailbox.emplace_back<sent_mail>({_header, _content, _time});
}

void SentMailbox::delete_mail(uint16_t _position) {
    std::lock_guard<std::shared_mutex> slock(sent_mailbox_mut);
    if ((_position > number_of_mail()) || (_position < 1)) {
        std::cout << "mail out of range\n";
        return;
    }
    sent_mailbox.erase(sent_mailbox.begin() + _position - 1);
}

const sent_mail* SentMailbox::get_mail(uint16_t _position) {
    sent_mail* ret;
    std::shared_lock<std::shared_mutex> slock(sent_mailbox_mut);
    if ((_position > number_of_mail()) || (_position < 1)) {
        std::cout << "mail out of range\n";
        slock.unlock();
        return nullptr;
    }
    ret = &sent_mailbox[_position-1];
    slock.unlock();
    return ret;
}

uint16_t SentMailbox::number_of_mail() {
    return sent_mailbox.size();
}

/**/

ReceivedMailbox::ReceivedMailbox()
: Mailbox() {

}

ReceivedMailbox::~ReceivedMailbox() {

}

ReceivedMailbox* ReceivedMailbox::getInstance() {
    static ReceivedMailbox __instance;
    return &__instance;
}

void ReceivedMailbox::save_mail(const std::string& _header, const std::string _content, uint64_t _time) {
    std::lock_guard<std::shared_mutex> glock(rcv_mailbox_mut);
    received_mailbox.emplace_back<received_mail>({_header, _content, _time});
}

void ReceivedMailbox::delete_mail(uint16_t _position) {
    std::lock_guard<std::shared_mutex> slock(rcv_mailbox_mut);
    if ((_position > number_of_mail()) || (_position < 1)) {
        std::cout << "mail out of range\n";
        return;
    }
    else received_mailbox.erase(received_mailbox.begin() + _position - 1);
}

const received_mail* ReceivedMailbox::get_mail(uint16_t _position) {
    received_mail* ret;
    std::shared_lock<std::shared_mutex> slock(rcv_mailbox_mut);
    if ((_position > number_of_mail()) || (_position < 1)) {
        std::cout << "mail out of range\n";
        slock.unlock();
        return nullptr;
    }
    ret = &received_mailbox[_position-1];
    slock.unlock();
    return ret;
}

uint16_t ReceivedMailbox::number_of_mail() {
    return received_mailbox.size();
}