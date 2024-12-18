#ifndef __CLIENT_GUI_H__
#define __CLIENT_GUI_H__

#include "mailclient_inc.h"
#include <stack>
#include <vector>

class Client_GUI {
public:
    Client_GUI();
    ~Client_GUI();

    void displayMainScreen() const;
    void displaySentMailbox(const std::vector<sent_mail>& _sent_mailbox) const;
    void displayReceivedMailbox(const std::vector<received_mail>& _rcv_mailbox) const;
    void displaySentMail(const std::vector<sent_mail>& _sent_mailbox, const uint16_t& _position) const;
    void displayReceivedMail(const std::vector<received_mail>& _rcv_mailbox, const uint16_t& _position) const;

    void displayPreviousPage();
    void displayNextPage();

    void back();

private:
    uint8_t current_page;
    std::stack<int> previous_stage;

};

#endif /* __CLIENT_GUI_H__ */