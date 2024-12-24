#include "client_GUI.h"
#include <iostream>
#include <vector>
#include <algorithm>

Client_GUI::Client_GUI()
: current_page(0) {

}

Client_GUI::~Client_GUI() {

}

void Client_GUI::displayMainScreen() const {
    std::cout << "-------------------------------------------------------------\n"
              << "     COPY-FROM-MESSENGER/GMAIL/...APP WITHOUT PEMMISSION     \n"
              << "                                                             \n"
              << "                                                             \n"
              << "                use command line for request                 \n"
              << "          (GUI would be available when i have time)          \n"
              << "-------------------------------------------------------------\n";
}

void Client_GUI::displaySentMailbox(const std::vector<sent_mail>& _sent_mailbox) const {
    std::cout << "-------------------------------------------------------------\n"
              << "                         SENT MAILBOX                        \n"
              << "                                                             \n";
    int index = 1;
    std::for_each(_sent_mailbox.begin(), _sent_mailbox.end(), [&index](auto _mail){
        std::cout << index << ". to: " << _mail.receiver << "\n";
        ++index;
    });
    std::cout << "-------------------------------------------------------------\n";
}

void Client_GUI::displayReceivedMailbox(const std::vector<received_mail>& _rcv_mailbox) const {
    std::cout << "-------------------------------------------------------------\n"
              << "                       RECEIVED MAILBOX                      \n"
              << "                                                             \n";
    int index = 1;
    std::for_each(_rcv_mailbox.begin(), _rcv_mailbox.end(), [&index](auto _mail){
        std::cout << index << ". from: " << _mail.sender << "\n";
        ++index;
    });
    std::cout << "-------------------------------------------------------------\n";
}

void Client_GUI::displaySentMail(const std::vector<sent_mail>& _sent_mailbox, const uint16_t& _position) const {
    std::cout << "-------------------------------------------------------------\n"
              << "                           CONTENT                           \n"
              << "                                                             \n";
    std::cout << "to: " << _sent_mailbox[_position-1].receiver << "\n";
    std::cout << "content: " << _sent_mailbox[_position-1].content << "\n";
    std::cout << "-------------------------------------------------------------\n";
}

void Client_GUI::displayReceivedMail(const std::vector<received_mail>& _rcv_mailbox, const uint16_t& _position) const {
    std::cout << "-------------------------------------------------------------\n"
              << "                           CONTENT                           \n"
              << "                                                             \n";
    std::cout << "from: " << _rcv_mailbox[_position-1].sender << "\n";
    std::cout << "content: " << _rcv_mailbox[_position-1].content << "\n";
    std::cout << "-------------------------------------------------------------\n";
}

void Client_GUI::displayPreviousPage() {

}

void Client_GUI::displayNextPage() {

}

void Client_GUI::back() {

}