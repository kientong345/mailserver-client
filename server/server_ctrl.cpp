#include "server_ctrl.h"
#include "server_ultility.h"
#include <thread>
#include <iostream>
#include <signal.h>
#include <tuple>
#include <sys/wait.h>

Server_Ctrl::Server_Ctrl()
: _transporter(nullptr),
  _soft_database(nullptr),
  _hard_database(nullptr),
  is_serving(false) {

}

Server_Ctrl::~Server_Ctrl() {

}

void Server_Ctrl::server_init() {
    _transporter = Transporter::getInstance();
    _soft_database = Soft_Database::getInstance();
    _hard_database = Hard_Database::getInstance();

    _transporter->init();
    _soft_database->load_client_table();
}

void Server_Ctrl::server_main() {
    _transporter->listen_mode();

    while (1) {
        _client_addr = _transporter->wait_connect_request();

        int pid = fork();
        if (pid == 0) {
            client_handler(); /* in child process */
            break; /* exit after handle client, action may depend on return val of handler */
        }
        else {
            _transporter->disconnect(); /* in parent process */
        }
    }
}

void Server_Ctrl::server_shutdown() {
    is_serving = false;
    _transporter->end();
    while ((wait(NULL)) > 0);
}

void Server_Ctrl::client_handler() {
    /* the first req' contents from client are always the user_name + password */
    verify_account();
    _soft_database->update_client_status(_client_name, ONLINE);

    is_serving = true;
    _transporter->open_mailbox(_client_name);
    std::thread mailbox_handler_thread(&Server_Ctrl::mailbox_handler, this);

    while (is_serving) {
        std::string _request = _transporter->receive_request();
        std::cout << "server received " + _request + "\n";
        req_t _req = parseRequest(_request);

        if (execute_request(_req) == E_TERM) break;
    }
    _transporter->disconnect();
    _transporter->end();
    is_serving = false;
    if (mailbox_handler_thread.joinable()) mailbox_handler_thread.join();
}

void Server_Ctrl::mailbox_handler() {
    while (is_serving) {
        std::string rcv_message = _transporter->receive_from_mailbox();
        _transporter->response(rcv_message);
    }
    _transporter->close_mailbox();
}

ERROR_CODE Server_Ctrl::execute_request(const req_t& _request) {
    REQ_TYPE req_type = _request.first;
    if (req_type == REQ_LOGIN) {
        auto content = static_cast<std::pair<std::string, std::string>*>(_request.second.get());
        const std::string* _correct_client_password = _soft_database->get_client_password(content->first);
        if (!_correct_client_password) {
            _transporter->response(mail_form("server", LOGIN_FAILED, __CURRENT_TIME__));
            return E_LOGIN_FAILED; // client username not existed
        }
        if (*_correct_client_password == content->second) {
            _client_name = content->first;
            _soft_database->update_client_address(_client_name, _client_addr);
            _transporter->response(mail_form("server", LOGIN_SUCCEED, __CURRENT_TIME__));
            return E_LOGIN_SUCCEED;
        }
        else {
            _transporter->response(mail_form("server", LOGIN_FAILED, __CURRENT_TIME__));
            return E_LOGIN_FAILED; // wrong password
        }
    }
    else if (req_type == REQ_REGISTER) {
        auto content = static_cast<std::pair<std::string, std::string>*>(_request.second.get());
        if (!(_soft_database->client_name_exist(content->first))) {
            _client_name = content->first;
            std::string _client_password = content->second;
            _soft_database->save_client_info(_client_name, _client_password, _client_addr);
            _transporter->response(mail_form("server", REGISTER_SUCCEED, __CURRENT_TIME__));
            return E_REGISTER_SUCCEED;
        }
        else {
            _transporter->response(mail_form("server", REGISTER_FAILED, __CURRENT_TIME__));
            return E_REGISTER_FAILED;
        }
    }
    else if (req_type == REQ_LOGOUT) {
        //return E_TERM;
        _soft_database->update_client_status(_client_name, OFFLINE);
        verify_account();
    }
    else if (req_type == REQ_SENDTO) {
        // sorry for garbage code!
        auto content = static_cast<std::tuple<std::string, std::string, uint64_t>*>(_request.second.get());
        /* elements in tuple: sender - content - sent_time */
        _transporter->send_to_mailbox(std::get<0>(*content), mail_form(_client_name, std::get<1>(*content), std::to_string(std::get<2>(*content))));
        _soft_database->save_sent_mail(_client_name, std::get<0>(*content), std::get<1>(*content), std::get<2>(*content));
        _soft_database->save_received_mail(std::get<0>(*content), _client_name, std::get<1>(*content), std::get<2>(*content));
    }
    else if (req_type == REQ_DELETEMAIL) {
        auto content = static_cast<std::pair<std::string, uint16_t>*>(_request.second.get());
        if (content->first == SENT_MAILBOX) {
            _soft_database->delete_sent_mail(_client_name, content->second);
        }
        else if (content->first == RCV_MAILBOX) {
            _soft_database->delete_received_mail(_client_name, content->second);
        }
        else _transporter->response(mail_form("server", "wrong mailbox type", __CURRENT_TIME__));
    }
    else if (req_type == REQ_CHANGENAME) {
        std::string new_name = *(static_cast<std::string*>(_request.second.get()));
        if (_soft_database->client_name_exist(new_name)) {
            _transporter->response(mail_form("server", "that name has already existed", __CURRENT_TIME__));
        }
        else {
            _soft_database->update_client_name(_client_name, new_name);
            /* sadly, each mailbox is binded to a name, so... */
            _transporter->close_mailbox();
            _transporter->open_mailbox(new_name);
            _client_name = new_name;
        }
    }
    else if (req_type == REQ_SEARCH) {
        std::string msg_back = "";
        std::string target_name = *(static_cast<std::string*>(_request.second.get()));
        if (target_name == "") {
            for (auto& user : *(_soft_database->get_client_table())) {
                msg_back += "user name: ";
                msg_back += user.first; msg_back += " - ";
                msg_back += "ip address: ";
                msg_back += inet_ntoa(user.second->address.sin_addr); msg_back += " - ";
                msg_back += "port: ";
                msg_back += std::to_string(ntohs(user.second->address.sin_port));
            }
        }
        else {
            if (!_soft_database->client_name_exist(target_name)) {
                msg_back += "client not found";
            }
            else {
                msg_back += "user name: ";
                msg_back += target_name; msg_back += " - ";
                msg_back += "ip address: ";
                msg_back += inet_ntoa(_soft_database->get_client_addr(target_name)->sin_addr); msg_back += " - ";
                msg_back += "port: ";
                msg_back += std::to_string(ntohs(_soft_database->get_client_addr(target_name)->sin_port));
            }
        }
        _transporter->response(mail_form("server", msg_back, __CURRENT_TIME__));
    }
    else if (req_type == REQ_TERMINATE) {
        _soft_database->update_client_status(_client_name, OFFLINE);
        return E_TERM;
    }
    else if (req_type == REQ_WHATISMYNAME) {
        _transporter->response(mail_form("server", _client_name, __CURRENT_TIME__));
    }
    else if (req_type == REQ_CHANGEPASSWORD) {
        std::string new_pass = *(static_cast<std::string*>(_request.second.get()));
        _soft_database->update_client_password(_client_name, new_pass);
    }
    else if (req_type == REQ_GIVEMEUSERLIST) {
        auto _userlist = data_to_json(_soft_database->get_user_list());
        _transporter->response(json_mail_form("server", _userlist, __CURRENT_TIME__));
    }
    else { // req_type == REQ_UNIDENTIFY
        std::string this_req = *(static_cast<std::string*>(_request.second.get()));
        std::cout << "unidentified request: \'" + this_req + "\'\n";
    }
    
    return E_SUCCESS;
}

void Server_Ctrl::verify_account() {
    while (1) {
        /* verication_message = [login/register] [user_name] [password] */
        std::string verification_message = _transporter->receive_request();
        ERROR_CODE ret = execute_request(parseRequest(verification_message));
        if (ret == E_LOGIN_SUCCEED) {
            std::cout << "login successfully\n";
            break;
        }
        else if (ret == E_LOGIN_FAILED) {
            std::cout << "login fail\n";
            continue;
        }
        else if (ret == E_REGISTER_SUCCEED) {
            std::cout << "register successfully\n";
            break;
        }
        else if (ret == E_REGISTER_FAILED) {
            std::cout << "register fail\n";
            continue;
        }
    }
}

static Server_Ctrl my_server;

void sigterm_handler(int signo) {
    if (signo == SIGTERM) {
        my_server.server_shutdown();
        exit(0);
    }
}

int main() {
    ::signal(SIGTERM, sigterm_handler);

    my_server.server_init();
    my_server.server_main();

    return 0;
}