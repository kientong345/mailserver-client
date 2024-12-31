#ifndef __CLIENT_CTRL_H__
#define __CLIENT_CTRL_H__

#include "client_inc.h"
#include "client_transport.h"
#include "client_mailbox.h"
#include "client_graphic.h"

class Client_Ctrl {
protected:
    ClientTransporter* _transporter;
    Mailbox* _sent_mailbox;
    Mailbox* _received_mailbox;
    ClientGraphic* _graphic;

    CTRL_MODE _current_mode;

    void user_handler();
    void ui_handler();
    void command_handler();
    void login();
    
public:
    Client_Ctrl();
    ~Client_Ctrl();

    void client_init();
    void client_main();

    class State;
    class ClientManager;

    /* nested class State */
    class State {
    protected:
        Client_Ctrl* _client;
        virtual STATE_TYPE execute_specific_request(const req_t& _request) = 0; // including user special key input from keyboard
    public:
        State(Client_Ctrl* _target) : _client(_target) {}
        virtual ~State() = default;
        virtual void show() = 0;
        virtual STATE_TYPE back() = 0;
        virtual STATE_TYPE next() = 0;
        virtual STATE_TYPE up() = 0;
        virtual STATE_TYPE down() = 0;
        virtual STATE_TYPE select() = 0;
        STATE_TYPE execute_request(const req_t& _request) {
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
            else {
                return execute_specific_request(_request);
            }
        }
    };

    /* nested class ClientManager */
    class ClientManager {
    private:
        Client_Ctrl* _client;
        std::shared_ptr<State> current_state;
    public:
        ClientManager(Client_Ctrl* _target) : _client(_target) {}
        ~ClientManager() = default;
        void set_state(STATE_TYPE _state) {
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
            case STATE_NOCHANGE:
            default:
                break;
            }
        }
        void back() {
            STATE_TYPE next_state = current_state->back();
            set_state(next_state);
        }
        void next() {
            STATE_TYPE next_state = current_state->next();
            set_state(next_state);
        }
        void up() {
            STATE_TYPE next_state = current_state->up();
            set_state(next_state);
        }
        void down() {
            STATE_TYPE next_state = current_state->down();
            set_state(next_state);
        }
        void select() {
            STATE_TYPE next_state = current_state->select();
            set_state(next_state);
        }
        void execute_request(const req_t& _request) {
            STATE_TYPE next_state = current_state->execute_request(_request);
            set_state(next_state);
        }
    };

    /*
    ---------------------------------------
    |                login                |
    |                                     |
    |     user name: abcdef               |
    |   >  password: *********            |
    |                                     |
    |               submit                |
    |          create new account         |
    | < back                       next > |
    ---------------------------------------
    */
    class Login_State : public State {
    private:
        enum class LOGIN_OPTION {
            USER_NAME,
            PASSWORD,
            SUBMIT,
            CREATE_NEW_ACCOUNT
        };
        LOGIN_OPTION _current_option;
        std::string _user_name;
        std::string _password;
    public:
        Login_State(Client_Ctrl* _target) : State(_target), _current_option(LOGIN_OPTION::USER_NAME) {}
        ~Login_State() = default;
        void show() override {

        }
        STATE_TYPE back() override {
            return STATE_NOCHANGE;
        }
        STATE_TYPE next() override {
            return STATE_NOCHANGE;
        }
        STATE_TYPE up() override {
            if (_current_option == LOGIN_OPTION::USER_NAME) _current_option = LOGIN_OPTION::CREATE_NEW_ACCOUNT;
            else _current_option = static_cast<LOGIN_OPTION>(static_cast<uint8_t>(_current_option)-1);
            return STATE_NOCHANGE;
        }
        STATE_TYPE down() override {
            if (_current_option == LOGIN_OPTION::CREATE_NEW_ACCOUNT) _current_option = LOGIN_OPTION::USER_NAME;
            else _current_option = static_cast<LOGIN_OPTION>(static_cast<uint8_t>(_current_option)+1);
            return STATE_NOCHANGE;
        }
        STATE_TYPE select() override {
            std::string res;
            switch (_current_option) {
            case LOGIN_OPTION::USER_NAME:
                // move edit position to usrname region
                // wait user to input usrname
                return STATE_NOCHANGE;
            case LOGIN_OPTION::PASSWORD:
                // move edit position to pass region
                // wait user to input pass
                return STATE_NOCHANGE;
            case LOGIN_OPTION::SUBMIT:
                _client->_transporter->send_request(LOGIN " " + _user_name + " " + _password);
                res = _client->_transporter->receive_response();
                return (res == LOGIN_SUCCEED) ? STATE_MENU : STATE_NOCHANGE;
            case LOGIN_OPTION::CREATE_NEW_ACCOUNT:
                return STATE_REGISTATION;
            default:
                return STATE_NOCHANGE;
            }
        }
        STATE_TYPE execute_specific_request(const req_t& _request) override {
            REQ_TYPE req_type = _request.first;
            switch (req_type) {
            case REQ_BACK:
                return back();
            case REQ_NEXT:
                return next();
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

    };

    /*
    ---------------------------------------
    |              register               |
    |                                     |
    |     user name: abcdef               |
    |   >  password: *********            |
    |                                     |
    |               submit                |
    |           i had an account          |
    | < back                       next > |
    ---------------------------------------
    */
    class Register_State : public State {
    private:
        enum class REGISTER_OPTION {
            USER_NAME,
            PASSWORD,
            SUBMIT,
            HAD_ACCOUNT
        };
        REGISTER_OPTION _current_option;
        std::string _user_name;
        std::string _password;
    public:
        Register_State(Client_Ctrl* _target) : State(_target), _current_option(REGISTER_OPTION::USER_NAME) {}
        ~Register_State() = default;
        void show() override {

        }
        STATE_TYPE back() override {
            return STATE_NOCHANGE;
        }
        STATE_TYPE next() override {
            return STATE_NOCHANGE;
        }
        STATE_TYPE up() override {
            if (_current_option == REGISTER_OPTION::USER_NAME) _current_option = REGISTER_OPTION::HAD_ACCOUNT;
            else _current_option = static_cast<REGISTER_OPTION>(static_cast<uint8_t>(_current_option)-1);
            return STATE_NOCHANGE;
        }
        STATE_TYPE down() override {
            if (_current_option == REGISTER_OPTION::HAD_ACCOUNT) _current_option = REGISTER_OPTION::USER_NAME;
            else _current_option = static_cast<REGISTER_OPTION>(static_cast<uint8_t>(_current_option)+1);
            return STATE_NOCHANGE;
        }
        STATE_TYPE select() override {
            std::string res;
            switch (_current_option) {
            case REGISTER_OPTION::USER_NAME:
                // move edit position to usrname region
                // wait user to input usrname
                return STATE_NOCHANGE;
            case REGISTER_OPTION::PASSWORD:
                // move edit position to pass region
                // wait user to input pass
                return STATE_NOCHANGE;
            case REGISTER_OPTION::SUBMIT:
                _client->_transporter->send_request(REGISTER " " + _user_name + " " + _password);
                res = _client->_transporter->receive_response();
                return (res == REGISTER_SUCCEED) ? STATE_MENU : STATE_NOCHANGE;
            case REGISTER_OPTION::HAD_ACCOUNT:
                return STATE_LOGIN;
            default:
                return STATE_NOCHANGE;
            }
        }
        STATE_TYPE execute_specific_request(const req_t& _request) override {
            REQ_TYPE req_type = _request.first;
            switch (req_type) {
            case REQ_BACK:
                return back();
            case REQ_NEXT:
                return next();
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

    };

    /*
    ---------------------------------------
    |             Select Menu             |
    |                                     |
    |              option 1               |
    |              option 2               |
    |            > option 3               |
    |              option 4               |
    |                                     |
    | < back                       next > |
    ---------------------------------------
    */
    class Menu_State : public State {
    private:
        enum class MENU_OPTION {
            
        };
        MENU_OPTION _current_option;
    public:
        Menu_State(Client_Ctrl* _target) : State(_target) {}
        ~Menu_State() = default;
        void show() override {

        }
        STATE_TYPE back() override {

        }
        STATE_TYPE next() override {

        }
        STATE_TYPE up() override {

        }
        STATE_TYPE down() override {

        }
        STATE_TYPE select() override {

        }
        STATE_TYPE execute_specific_request(const req_t& _request) override {

        }

    };

};

#endif /* __CLIENT_CTRL_H__ */