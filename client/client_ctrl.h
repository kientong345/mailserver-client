#ifndef __CLIENT_CTRL_H__
#define __CLIENT_CTRL_H__

#include "client_inc.h"
#include "client_transport.h"
#include "client_mailbox.h"
// #include "client_graphic.h"
#include "client_cli.h"

class Client_Ctrl {
protected:
    ClientTransporter* _transporter;
    Mailbox<sent_mail>* _sent_mailbox;
    Mailbox<received_mail>* _received_mailbox;
    // ClientGraphic* _graphic;
    ClientCLI* _cli;

    CTRL_MODE _current_mode;
    bool _login_succeed;

    void user_handler();
    void ui_handler();
    void command_handler();
    std::string send_request_wait_response(const std::string& _request);
    
public:
    Client_Ctrl();
    ~Client_Ctrl();

    void client_init();
    void client_main();
    void client_end();

    /* nested class State */
    /* form of a screen
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
    class State {
    protected:
        Client_Ctrl* _client;
        virtual STATE_TYPE execute_specific_request(const req_t& _request) = 0; // including user special key input from keyboard
    public:
        State(Client_Ctrl* _target);
        virtual ~State() = default;
        virtual void show() = 0;
        virtual STATE_TYPE left() = 0;
        virtual STATE_TYPE right() = 0;
        virtual STATE_TYPE up() = 0;
        virtual STATE_TYPE down() = 0;
        virtual STATE_TYPE select() = 0;
        STATE_TYPE execute_request(const req_t& _request);
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
        void update_indicator();
        void clear_indicator();
    public:
        Login_State(Client_Ctrl* _target);
        ~Login_State() = default;
        void show() override;
        STATE_TYPE left() override;
        STATE_TYPE right() override;
        STATE_TYPE up() override;
        STATE_TYPE down() override;
        STATE_TYPE select() override;
        STATE_TYPE execute_specific_request(const req_t& _request) override;
    };

    /*
    ---------------------------------------
    |              register               |
    |                                     |
    |     user name: abcdef               |
    |   >  password: *********            |
    |                                     |
    |               submit                |
    |          i had an account           |
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
        void update_indicator();
        void clear_indicator();
    public:
        Register_State(Client_Ctrl* _target);
        ~Register_State() = default;
        void show() override;
        STATE_TYPE left() override;
        STATE_TYPE right() override;
        STATE_TYPE up() override;
        STATE_TYPE down() override;
        STATE_TYPE select() override;
        STATE_TYPE execute_specific_request(const req_t& _request) override;
    };

    /*
    ---------------------------------------
    |             Select Menu             |
    |                                     |
    |             friendlist              |
    |              setting                |
    |          > instructions             |
    |                info                 |
    |                exit                 |
    | < logout                     next > |
    ---------------------------------------
    */
    class Menu_State : public State {
    private:
        enum class MENU_OPTION {
            FRIENDLIST,
            SETTING,
            INSTRUCTIONS,
            INFO,
            EXIT
        };
        MENU_OPTION _current_option;
        void update_indicator();
        void clear_indicator();
    public:
        Menu_State(Client_Ctrl* _target);
        ~Menu_State() = default;
        void show() override;
        STATE_TYPE left() override;
        STATE_TYPE right() override;
        STATE_TYPE up() override;
        STATE_TYPE down() override;
        STATE_TYPE select() override;
        STATE_TYPE execute_specific_request(const req_t& _request) override;
    };

    /*
    ---------------------------------------
    |             Friendlist              |
    |                 /\                  |
    |   5. abcd                           |
    |   6. piggy                          |
    | > 7. liu bei                        |
    |   8. adolf                          |
    |                 \/                  |
    | < back                       next > |
    ---------------------------------------
    */
    class FriendList_State : public State {
    private:
        uint16_t _current_option;
        uint16_t _friend_num;
    public:
        FriendList_State(Client_Ctrl* _target);
        ~FriendList_State() = default;
        void show() override;
        STATE_TYPE left() override;
        STATE_TYPE right() override;
        STATE_TYPE up() override;
        STATE_TYPE down() override;
        STATE_TYPE select() override;
        STATE_TYPE execute_specific_request(const req_t& _request) override;
    };

    /*
    ---------------------------------------
    |               Setting               |
    |                                     |
    |             change info             |
    |             change pass             |
    |             > manage                |
    |                                     |
    |                                     |
    | < back                       next > |
    ---------------------------------------
    */
    class Setting_State : public State {
    public:
        Setting_State(Client_Ctrl* _target);
        ~Setting_State() = default;
        void show() override;
        STATE_TYPE left() override;
        STATE_TYPE right() override;
        STATE_TYPE up() override;
        STATE_TYPE down() override;
        STATE_TYPE select() override;
        STATE_TYPE execute_specific_request(const req_t& _request) override;
    };

    class Instructions_State : public State {
    public:
        Instructions_State(Client_Ctrl* _target);
        ~Instructions_State() = default;
        void show() override;
        STATE_TYPE left() override;
        STATE_TYPE right() override;
        STATE_TYPE up() override;
        STATE_TYPE down() override;
        STATE_TYPE select() override;
        STATE_TYPE execute_specific_request(const req_t& _request) override;
    };

    class Info_State : public State {
    public:
        Info_State(Client_Ctrl* _target);
        ~Info_State() = default;
        void show() override;
        STATE_TYPE left() override;
        STATE_TYPE right() override;
        STATE_TYPE up() override;
        STATE_TYPE down() override;
        STATE_TYPE select() override;
        STATE_TYPE execute_specific_request(const req_t& _request) override;
    };

    /*
    ---------------------------------------
    |              Chat box               |
    | 24/7/2004-16:24:32                  |
    | @johnwick01: are you killed my dog? |
    | 24/7/2004-16:26:14                  |
    | @dogslayer: no what?                |
    | 24/7/2004-16:26:40                  |
    | @johnwick01: cuz ur name look sus af|
    | < back [@: fk u            ] send > |
    ---------------------------------------
    */
    class Chat_State : public State {
    private:
        std::string _friendname;
        std::string _message;
    public:
        Chat_State(Client_Ctrl* _target);
        ~Chat_State() = default;
        void show() override;
        STATE_TYPE left() override;
        STATE_TYPE right() override;
        STATE_TYPE up() override;
        STATE_TYPE down() override;
        STATE_TYPE select() override;
        STATE_TYPE execute_specific_request(const req_t& _request) override;
    };

    /* nested class ClientManager */
    class ClientManager {
    private:
        Client_Ctrl* _client;
        std::shared_ptr<Client_Ctrl::State> current_state;
    public:
        ClientManager(Client_Ctrl* _target);
        ~ClientManager() = default;
        void set_state(STATE_TYPE _state);
        void left();
        void right();
        void up();
        void down();
        void select();
        void execute_request(const req_t& _request);
    };
    ClientManager _manager;
};

#endif /* __CLIENT_CTRL_H__ */