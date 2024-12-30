#ifndef __CLIENT_MANAGER_H__
#define __CLIENT_MANAGER_H__

#include "client_inc.h"
#include "client_graphic.h"
#include <memory>

class State;

class ClientManager {
private:
    std::shared_ptr<State> current_state;

    ClientManager();

public:
    static ClientManager* getInstance();

    void set_state(const std::shared_ptr<State>& _state);
    void back();
    void next();
    void up();
    void down();
    void select();

    ~ClientManager();

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

class State {
protected:
    ClientGraphic* _graphic;

public:
    State();
    virtual ~State();

    virtual void show() = 0;
    virtual void back() = 0;
    virtual void next() = 0;
    virtual void up() = 0;
    virtual void down() = 0;
    virtual void select() = 0;

};

class Login_State : public State {
private:
public:
    void show() override;
    void back() override;
    void next() override;
    void up() override;
    void down() override;
    void select() override;

};

class Registation_State : public State {
private:
public:
    void show() override;
    void back() override;
    void next() override;
    void up() override;
    void down() override;
    void select() override;

};

class Menu_State : public State {
private:
public:
    void show() override;
    void back() override;
    void next() override;
    void up() override;
    void down() override;
    void select() override;

};

class Sent_Mailbox_State : public State {
private:
public:
    void show() override;
    void back() override;
    void next() override;
    void up() override;
    void down() override;
    void select() override;

};

class Received_Mailbox_State : public State {
private:
public:
    void show() override;
    void back() override;
    void next() override;
    void up() override;
    void down() override;
    void select() override;

};

class Sent_Mail_State : public State {
private:
public:
    void show() override;
    void back() override;
    void next() override;
    void up() override;
    void down() override;
    void select() override;

};

class Received_Mail_State : public State {
private:
public:
    void show() override;
    void back() override;
    void next() override;
    void up() override;
    void down() override;
    void select() override;

};

class Editor_State : public State {
private:
public:
    void show() override;
    void back() override;
    void next() override;
    void up() override;
    void down() override;
    void select() override;

};

#endif /* __CLIENT_MANAGER_H__ */