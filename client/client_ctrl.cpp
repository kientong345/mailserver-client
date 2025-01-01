#include "client_ctrl.h"
#include "client_ultility.h"
#include <signal.h>
#include <iostream>

Client_Ctrl::Client_Ctrl()
: _transporter(nullptr),
  _sent_mailbox(nullptr),
  _received_mailbox(nullptr),
  _graphic(nullptr),
  _current_mode(UI_MODE) {

}

Client_Ctrl::~Client_Ctrl() {

}

void Client_Ctrl::client_init() {
    _transporter = ClientTransporter::getInstance();
    _sent_mailbox = SentMailbox::getInstance();
    _received_mailbox = ReceivedMailbox::getInstance();
    _graphic = ClientGraphic::getInstance();

    _transporter->init();
}

void Client_Ctrl::client_main() {
    _transporter->connect_to_server(SERVER_IP, SERVER_PORT);
    /* to be change */
    ClientManager(this).set_state(STATE_LOGIN);
    std::string _my_name;
    std::cout << "enter your username: ";
    std::getline(std::cin, _my_name);
    _transporter->send_request(_my_name);
    /* to be change */

    user_handler();
}

void Client_Ctrl::user_handler() {
    ClientManager(this).set_state(STATE_MENU);
   
    while (1) {
        if (_current_mode == UI_MODE) ui_handler();
        else if (_current_mode == COMMAND_MODE) command_handler();
    }
}

void Client_Ctrl::ui_handler() {
    ClientManager _manager(this);
    INPUT_TYPE _input = get_user_input();
    switch (_input) {
    case I_LEFT:
        _manager.left();
        break;
    case I_RIGHT:
        _manager.right();
        break;
    case I_UP:
        _manager.up();
        break;
    case I_DOWN:
        _manager.down();
        break;
    case I_SELECT:
        _manager.select();
        break;
    case I_SWITCH:
        _current_mode = COMMAND_MODE;
        // TODO: i really really should register the switch as an interrupt signal instead of
        // asking from user as a key binding input, how did users in command mode input that key?
        break;
    default:
        break;
    }
}

void Client_Ctrl::command_handler() {
    std::string user_req;
    std::getline(std::cin, user_req);
    req_t req = parseRequest(user_req);
    ClientManager(this).execute_request(req);
}

void Client_Ctrl::login() {
    auto _login = [this](){
        std::cout << "enter your user name: ";
        std::string user_name;
        std::getline(std::cin, user_name);
        std::cout << "enter your password: ";
        std::string user_pass;
        std::getline(std::cin, user_pass);
        _transporter->send_request("login " + user_name + " " + user_pass);

        std::string res = _transporter->receive_response();
        if (res == "succeed") return true;
        else return false;
    };
    auto _register = [this](){
        std::cout << "enter your user name: ";
        std::string user_name;
        std::getline(std::cin, user_name);
        std::cout << "enter your password: ";
        std::string user_pass;
        std::getline(std::cin, user_pass);
        _transporter->send_request("register " + user_name + " " + user_pass);

        std::string res = _transporter->receive_response();
        if (res == "succeed") return true;
        else return false;
    };
    int option = 0;
    while (1) {
        std::cout << "type 0 to login, 1 to register: ";
        std::cin >> option;
        switch (option)
        {
        case 0:
            if (_login()) return;
            break;
        case 1:
            if (_register()) return;
            break;
        default:
            break;
        }
    }
}

static Client_Ctrl _my_app;

void sigtermhandler(int signo) {
    if (signo == SIGTERM) {
        exit(0);
    }
}

int main() {
    ::signal(SIGTERM, sigtermhandler);

    _my_app.client_init();
    _my_app.client_main();

    return 0;
}