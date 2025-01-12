#include "client_ctrl.h"
#include "client_ultility.h"
#include "client_cli_config.h"
#include <signal.h>
#include <iostream>

Client_Ctrl::Client_Ctrl()
: _transporter(nullptr),
  _sent_mailbox(nullptr),
  _received_mailbox(nullptr),
  //_graphic(nullptr),
  _cli(nullptr),
  _current_mode(UI_MODE),
  _login_succeed(false),
  _manager(this) {

}

Client_Ctrl::~Client_Ctrl() {

}

void Client_Ctrl::client_init() {
    _transporter = ClientTransporter::getInstance();
    _sent_mailbox = Mailbox<sent_mail>::getInstance();
    _received_mailbox = Mailbox<received_mail>::getInstance();
    // _graphic = ClientGraphic::getInstance();
    _cli = ClientCLI::getInstance();

    _transporter->init();
    // _graphic->init();
    _cli->init();
    _cli->display_allscreen({BACKGROUND_IMG, WHITE});
}

void Client_Ctrl::client_main() {
    _transporter->connect_to_server(SERVER_IP, SERVER_PORT);
    _transporter->set_task_on_receive_data([this](){
        std::string _rcv_mail = _transporter->receive_response();
        std::string _sender = getWord(_rcv_mail, 1);
        std::string _content = getWord(_rcv_mail, 2);
        uint64_t _rcv_time = std::stoull(getWord(_rcv_mail, 3));
        _received_mailbox->save_mail(_sender, _content, _rcv_time);
    });

    user_handler();
}

void Client_Ctrl::client_end() {
    // _graphic->end();
    _cli->end();
}

void Client_Ctrl::user_handler() {
    _manager.set_state(STATE_LOGIN);
   
    while (1) {
        if (_current_mode == UI_MODE) ui_handler();
        else if (_current_mode == COMMAND_MODE) command_handler();
    }
}

void Client_Ctrl::ui_handler() {
    INPUT_TYPE _input = _cli->get_user_input();
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
        _manager.execute_request(parseRequest(INPUT_SWITCH));
        // TODO: i really really should register the switch as an interrupt signal instead of
        // asking from user as a key binding input, how did users in command mode input that key?
        break;
    default:
        break;
    }
}

void Client_Ctrl::command_handler() {
    std::string user_req = _cli->get_user_cmd(10, 19);
    _cli->display_entity(CMD_IMG); // clean cmd area
    req_t req = parseRequest(user_req);
    _manager.execute_request(req);
}

static Client_Ctrl _my_app;

void sigtermhandler(int signo) {
    if (signo == SIGTERM) {
        _my_app.client_end();
        exit(0);
    }
}

int main() {
    ::signal(SIGTERM, sigtermhandler);

    _my_app.client_init();
    _my_app.client_main();

    return 0;
}

Client_Ctrl::State::State(Client_Ctrl* _target)
: _client(_target) {

}

STATE_TYPE Client_Ctrl::State::execute_request(const req_t& _request) {
    REQ_TYPE req_type = _request.first;
    if (req_type == REQ_TOSERVER) {
        std::string _message = *(static_cast<std::string*>(_request.second.get()));
        _client->_transporter->send_request(_message);
        return STATE_NOCHANGE;
    }
    else if (req_type == REQ_SHOWCHAT) {
        // ???
        return STATE_NOCHANGE;
    }
    else if (req_type == REQ_SWITCH) {
        if (_client->_current_mode == UI_MODE) {
            _client->_current_mode = COMMAND_MODE;
            _client->_cli->pause_ui();
        }
        else if (_client->_current_mode == COMMAND_MODE) {
            _client->_current_mode = UI_MODE;
            _client->_cli->continue_ui();
        }
        return STATE_NOCHANGE;
    }
    else {
        return execute_specific_request(_request);
    }
}

Client_Ctrl::Login_State::Login_State(Client_Ctrl* _target)
: State(_target), _current_option(LOGIN_OPTION::USER_NAME) {

}

void Client_Ctrl::Login_State::update_indicator() {
    switch (_current_option) {
    case LOGIN_OPTION::USER_NAME:
        _client->_cli->display_entity(INDICATOR(LOGIN_IND_POS_1));
        break;
    case LOGIN_OPTION::PASSWORD:
        _client->_cli->display_entity(INDICATOR(LOGIN_IND_POS_2));
        break;
    case LOGIN_OPTION::SUBMIT:
        _client->_cli->display_entity(INDICATOR(LOGIN_IND_POS_3));
        break;
    case LOGIN_OPTION::CREATE_NEW_ACCOUNT:
        _client->_cli->display_entity(INDICATOR(LOGIN_IND_POS_4));
        break;
    default:
        break;
    }
}

void Client_Ctrl::Login_State::clear_indicator() {
    switch (_current_option) {
    case LOGIN_OPTION::USER_NAME:
        _client->_cli->erase_area(LOGIN_IND_POS_1);
        break;
    case LOGIN_OPTION::PASSWORD:
        _client->_cli->erase_area(LOGIN_IND_POS_2);
        break;
    case LOGIN_OPTION::SUBMIT:
        _client->_cli->erase_area(LOGIN_IND_POS_3);
        break;
    case LOGIN_OPTION::CREATE_NEW_ACCOUNT:
        _client->_cli->erase_area(LOGIN_IND_POS_4);
        break;
    default:
        break;
    }
}

void Client_Ctrl::Login_State::show() {
    _client->_cli->display_allscreen({BACKGROUND_IMG, WHITE});
    _client->_cli->display_entity(LOGIN_TEXT);
    _client->_cli->display_entity(LOGIN_USERNAME_TEXT);
    _client->_cli->display_entity(LOGIN_USERNAME_BOX);
    _client->_cli->display_entity(LOGIN_PASSWORD_TEXT);
    _client->_cli->display_entity(LOGIN_PASSWORD_BOX);
    _client->_cli->display_entity(LOGIN_SUBMIT_BOX);
    _client->_cli->display_entity(LOGIN_QUESTION);
    update_indicator();
}

STATE_TYPE Client_Ctrl::Login_State::left() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Login_State::right() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Login_State::up() {
    clear_indicator();
    if (_current_option == LOGIN_OPTION::USER_NAME) _current_option = LOGIN_OPTION::CREATE_NEW_ACCOUNT;
    else _current_option = static_cast<LOGIN_OPTION>(static_cast<uint8_t>(_current_option)-1);
    update_indicator();
    return STATE_NOCHANGE;
}
STATE_TYPE Client_Ctrl::Login_State::down() {
    clear_indicator();
    if (_current_option == LOGIN_OPTION::CREATE_NEW_ACCOUNT) _current_option = LOGIN_OPTION::USER_NAME;
    else _current_option = static_cast<LOGIN_OPTION>(static_cast<uint8_t>(_current_option)+1);
    update_indicator();
    return STATE_NOCHANGE;
}
STATE_TYPE Client_Ctrl::Login_State::select() {
    std::string res;
    switch (_current_option) {
    case LOGIN_OPTION::USER_NAME:
        _client->_cli->erase_area(LOGIN_USERNAME_BOX.content.sector);
        _client->_cli->pause_ui();
        _user_name = std::move(_client->_cli->get_user_cmd(LOGIN_USERNAME_BOX.content.sector.base.xPos,
                                                        LOGIN_USERNAME_BOX.content.sector.base.yPos));
        _client->_cli->continue_ui();
        return STATE_NOCHANGE;
    case LOGIN_OPTION::PASSWORD:
        _client->_cli->erase_area(LOGIN_PASSWORD_BOX.content.sector);
        _client->_cli->pause_ui();
        _password = std::move(_client->_cli->get_user_cmd(LOGIN_PASSWORD_BOX.content.sector.base.xPos,
                                                        LOGIN_PASSWORD_BOX.content.sector.base.yPos));
        _client->_cli->continue_ui();
        return STATE_NOCHANGE;
    case LOGIN_OPTION::SUBMIT:
        _client->_transporter->send_request(LOGIN " " + _user_name + " " + _password);
        WAIT_MS(100);
        res = _client->_received_mailbox->get_mailbox().back().content;
        if (res == LOGIN_SUCCEED) {
            _client->_login_succeed = true;
            return STATE_MENU;
        }
        else return STATE_NOCHANGE;
    case LOGIN_OPTION::CREATE_NEW_ACCOUNT:
        return STATE_REGISTATION;
    default:
        return STATE_NOCHANGE;
    }
}
STATE_TYPE Client_Ctrl::Login_State::execute_specific_request(const req_t& _request) {
    REQ_TYPE req_type = _request.first;
    switch (req_type) {
    case REQ_UP:
        return up();
    case REQ_DOWN:
        return down();
    case REQ_SELECT:
        return select();
    default:
        return STATE_NOCHANGE;
    }
}

Client_Ctrl::Register_State::Register_State(Client_Ctrl* _target)
: State(_target), _current_option(REGISTER_OPTION::USER_NAME) {

}

void Client_Ctrl::Register_State::update_indicator() {
    switch (_current_option) {
    case REGISTER_OPTION::USER_NAME:
        _client->_cli->display_entity(INDICATOR(REGISTER_IND_POS_1));
        break;
    case REGISTER_OPTION::PASSWORD:
        _client->_cli->display_entity(INDICATOR(REGISTER_IND_POS_2));
        break;
    case REGISTER_OPTION::SUBMIT:
        _client->_cli->display_entity(INDICATOR(REGISTER_IND_POS_3));
        break;
    case REGISTER_OPTION::HAD_ACCOUNT:
        _client->_cli->display_entity(INDICATOR(REGISTER_IND_POS_4));
        break;
    default:
        break;
    }
}

void Client_Ctrl::Register_State::clear_indicator() {
    switch (_current_option) {
    case REGISTER_OPTION::USER_NAME:
        _client->_cli->erase_area(LOGIN_IND_POS_1);
        break;
    case REGISTER_OPTION::PASSWORD:
        _client->_cli->erase_area(LOGIN_IND_POS_2);
        break;
    case REGISTER_OPTION::SUBMIT:
        _client->_cli->erase_area(LOGIN_IND_POS_3);
        break;
    case REGISTER_OPTION::HAD_ACCOUNT:
        _client->_cli->erase_area(LOGIN_IND_POS_4);
        break;
    default:
        break;
    }
}

void Client_Ctrl::Register_State::show() {
    _client->_cli->display_allscreen({BACKGROUND_IMG, WHITE});
    _client->_cli->display_entity(REGISTER_TEXT);
    _client->_cli->display_entity(REGISTER_USERNAME_TEXT);
    _client->_cli->display_entity(REGISTER_USERNAME_BOX);
    _client->_cli->display_entity(REGISTER_PASSWORD_TEXT);
    _client->_cli->display_entity(REGISTER_PASSWORD_BOX);
    _client->_cli->display_entity(REGISTER_SUBMIT_BOX);
    _client->_cli->display_entity(REGISTER_QUESTION);
    update_indicator();
}

STATE_TYPE Client_Ctrl::Register_State::left() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Register_State::right() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Register_State::up() {
    clear_indicator();
    if (_current_option == REGISTER_OPTION::USER_NAME) _current_option = REGISTER_OPTION::HAD_ACCOUNT;
    else _current_option = static_cast<REGISTER_OPTION>(static_cast<uint8_t>(_current_option)-1);
    update_indicator();
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Register_State::down() {
    clear_indicator();
    if (_current_option == REGISTER_OPTION::HAD_ACCOUNT) _current_option = REGISTER_OPTION::USER_NAME;
    else _current_option = static_cast<REGISTER_OPTION>(static_cast<uint8_t>(_current_option)+1);
    update_indicator();
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Register_State::select() {
    std::string res;
    switch (_current_option) {
    case REGISTER_OPTION::USER_NAME:
        _client->_cli->erase_area(REGISTER_USERNAME_BOX.content.sector);
        _client->_cli->pause_ui();
        _user_name = std::move(_client->_cli->get_user_cmd(REGISTER_USERNAME_BOX.content.sector.base.xPos,
                                                        REGISTER_USERNAME_BOX.content.sector.base.yPos));
        _client->_cli->continue_ui();
        return STATE_NOCHANGE;
    case REGISTER_OPTION::PASSWORD:
        _client->_cli->erase_area(REGISTER_PASSWORD_BOX.content.sector);
        _client->_cli->pause_ui();
        _password = std::move(_client->_cli->get_user_cmd(REGISTER_PASSWORD_BOX.content.sector.base.xPos,
                                                        REGISTER_PASSWORD_BOX.content.sector.base.yPos));
        _client->_cli->continue_ui();
        return STATE_NOCHANGE;
    case REGISTER_OPTION::SUBMIT:
        _client->_transporter->send_request(REGISTER " " + _user_name + " " + _password);
        WAIT_MS(100);
        res = _client->_received_mailbox->get_mailbox().back().content;
        if (res == REGISTER_SUCCEED) {
            _client->_login_succeed = true;
            return STATE_MENU;
        }
        else return STATE_NOCHANGE;
    case REGISTER_OPTION::HAD_ACCOUNT:
        return STATE_LOGIN;
    default:
        return STATE_NOCHANGE;
    }
}

STATE_TYPE Client_Ctrl::Register_State::execute_specific_request(const req_t& _request) {
    REQ_TYPE req_type = _request.first;
    switch (req_type) {
    case REQ_UP:
        return up();
    case REQ_DOWN:
        return down();
    case REQ_SELECT:
        return select();
    default:
        return STATE_NOCHANGE;
    }
}

Client_Ctrl::Menu_State::Menu_State(Client_Ctrl* _target)
: State(_target), _current_option(MENU_OPTION::FRIENDLIST) {

}

void Client_Ctrl::Menu_State::show() {
    //_client->_graphic->display_allscreen(MENU_BACKGROUND_PATH);
}
STATE_TYPE Client_Ctrl::Menu_State::left() {
    _client->_transporter->send_request(LOGOUT);
    return STATE_LOGIN;
}
STATE_TYPE Client_Ctrl::Menu_State::right() {
    return STATE_NOCHANGE;
}
STATE_TYPE Client_Ctrl::Menu_State::up() {
    if (_current_option == MENU_OPTION::FRIENDLIST) _current_option = MENU_OPTION::EXIT;
    else _current_option = static_cast<MENU_OPTION>(static_cast<uint8_t>(_current_option)-1);
    return STATE_NOCHANGE;
}
STATE_TYPE Client_Ctrl::Menu_State::down() {
    if (_current_option == MENU_OPTION::EXIT) _current_option = MENU_OPTION::FRIENDLIST;
    else _current_option = static_cast<MENU_OPTION>(static_cast<uint8_t>(_current_option)+1);
    return STATE_NOCHANGE;
}
STATE_TYPE Client_Ctrl::Menu_State::select() {
    switch (_current_option) {
    case MENU_OPTION::FRIENDLIST:
        return STATE_FRIENDLIST;
    case MENU_OPTION::SETTING:
        return STATE_SETTING;
    case MENU_OPTION::INSTRUCTIONS:
        return STATE_INSTRUCTIONS;
    case MENU_OPTION::INFO:
        return STATE_INFO;
    case MENU_OPTION::EXIT:
        _client->_transporter->send_request(TERMINATE);
        _client->client_end();
    default:
        return STATE_NOCHANGE;
    }
}
STATE_TYPE Client_Ctrl::Menu_State::execute_specific_request(const req_t& _request) {
    REQ_TYPE req_type = _request.first;
    switch (req_type) {
    case REQ_LOGOUT:
        return left();
    case REQ_UP:
        return up();
    case REQ_DOWN:
        return down();
    case REQ_SELECT:
        return select();
    default:
        return STATE_NOCHANGE;
    }
}

Client_Ctrl::FriendList_State::FriendList_State(Client_Ctrl* _target)
: State(_target), _current_option(0), _friend_num(0) {
    // update _current_option and _friend_num
}

void Client_Ctrl::FriendList_State::show() {

}
STATE_TYPE Client_Ctrl::FriendList_State::left() {
    return STATE_MENU;
}
STATE_TYPE Client_Ctrl::FriendList_State::right() {
    /* may do some actions:
    add friend,
    delete friend,
    search friend,
    mark a friend,
    show info of a friend (ip or smt?)
    reload the page?
    */
    return STATE_NOCHANGE;
}
STATE_TYPE Client_Ctrl::FriendList_State::up() {
    if (_friend_num > 0) {
        _current_option = (_current_option > 1) ? _current_option-1 : _friend_num;
    }
    return STATE_NOCHANGE;
}
STATE_TYPE Client_Ctrl::FriendList_State::down() {
    if (_friend_num > 0) {
        _current_option = (_current_option < _friend_num) ? _current_option+1 : 1;
    }
    return STATE_NOCHANGE;
}
STATE_TYPE Client_Ctrl::FriendList_State::select() {
    return STATE_CHAT;
}
STATE_TYPE Client_Ctrl::FriendList_State::execute_specific_request(const req_t& _request) {
    return STATE_NOCHANGE;
}

Client_Ctrl::Setting_State::Setting_State(Client_Ctrl* _target)
: State(_target) {

}

void Client_Ctrl::Setting_State::show() {

}

STATE_TYPE Client_Ctrl::Setting_State::left() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Setting_State::right() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Setting_State::up() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Setting_State::down() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Setting_State::select() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Setting_State::execute_specific_request(const req_t& _request) {
    return STATE_NOCHANGE;
}

Client_Ctrl::Instructions_State::Instructions_State(Client_Ctrl* _target)
: State(_target) {

}

void Client_Ctrl::Instructions_State::show() {

}

STATE_TYPE Client_Ctrl::Instructions_State::left() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Instructions_State::right() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Instructions_State::up() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Instructions_State::down() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Instructions_State::select() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Instructions_State::execute_specific_request(const req_t& _request) {
    return STATE_NOCHANGE;
}

Client_Ctrl::Info_State::Info_State(Client_Ctrl* _target)
: State(_target) {

}

void Client_Ctrl::Info_State::show() {

}

STATE_TYPE Client_Ctrl::Info_State::left() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Info_State::right() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Info_State::up() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Info_State::down() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Info_State::select() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Info_State::execute_specific_request(const req_t& _request) {
    return STATE_NOCHANGE;
}

Client_Ctrl::Chat_State::Chat_State(Client_Ctrl* _target)
: State(_target) {

}

void Client_Ctrl::Chat_State::show() {

}

STATE_TYPE Client_Ctrl::Chat_State::left() {
    return STATE_FRIENDLIST;
}

STATE_TYPE Client_Ctrl::Chat_State::right() {
    if (_message != "") {
        _client->_transporter->send_request(SENDTO " " + _friendname + " " + _message + __CURRENT_TIME__);
        _message = "";
    }
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Chat_State::up() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Chat_State::down() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Chat_State::select() {
    return STATE_NOCHANGE;
}

STATE_TYPE Client_Ctrl::Chat_State::execute_specific_request(const req_t& _request) {
    return STATE_NOCHANGE;
}

Client_Ctrl::ClientManager::ClientManager(Client_Ctrl* _target)
: _client(_target) {

}

void Client_Ctrl::ClientManager::set_state(STATE_TYPE _state) {
    switch (_state) {
    case STATE_LOGIN:
        current_state = __LOGIN_STATE__;
        break;
    case STATE_REGISTATION:
        current_state = __REGISTER_STATE__;
        break;
    case STATE_MENU:
        current_state = __MENU_STATE__;
        break;
    case STATE_FRIENDLIST:
        current_state = __FRIENDLIST_STATE__;
        break;
    case STATE_SETTING:
        current_state = __SETTING_STATE__;
        break;
    case STATE_INSTRUCTIONS:
        current_state = __INSTRUCTIONS_STATE__;
        break;
    case STATE_INFO:
        current_state = __INFO_STATE__;
        break;
    case STATE_CHAT:
        current_state = __CHAT_STATE__;
        break;
    case STATE_EXIT:
    case STATE_NOCHANGE:
    default:
        break;
    }
    if (_state != STATE_NOCHANGE) current_state->show();
}

void Client_Ctrl::ClientManager::left() {
    STATE_TYPE next_state = current_state->left();
    set_state(next_state);
}

void Client_Ctrl::ClientManager::right() {
    STATE_TYPE next_state = current_state->right();
    set_state(next_state);
}

void Client_Ctrl::ClientManager::up() {
    STATE_TYPE next_state = current_state->up();
    set_state(next_state);
}

void Client_Ctrl::ClientManager::down() {
    STATE_TYPE next_state = current_state->down();
    set_state(next_state);
}

void Client_Ctrl::ClientManager::select() {
    STATE_TYPE next_state = current_state->select();
    set_state(next_state);
}

void Client_Ctrl::ClientManager::execute_request(const req_t& _request) {
    STATE_TYPE next_state = current_state->execute_request(_request);
    set_state(next_state);
}