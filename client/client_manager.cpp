#include "client_manager.h"

ClientManager::ClientManager()
: current_state(std::make_shared<Registation_State>()) {

}

ClientManager::~ClientManager() {

}

ClientManager* ClientManager::getInstance() {
    static ClientManager __instance;
    return &__instance;
}

void ClientManager::set_state(const std::shared_ptr<State>& _state) {
    current_state = _state;
    current_state->show();
}

void ClientManager::back() {
    current_state->back();
}

void ClientManager::next() {
    current_state->next();
}

void ClientManager::up() {
    current_state->up();
}

void ClientManager::down() {
    current_state->down();
}

void ClientManager::select() {
    return current_state->select();
}

/**/

State::State()
: _graphic(ClientGraphic::getInstance()) {
    
}

State::~State() {

}

/**/

void Registation_State::show() {
    _graphic->displayRegistation();
}

void Registation_State::back() {

}

void Registation_State::next() {

}

void Registation_State::up() {

}

void Registation_State::down() {

}

void Registation_State::select() {

}

/**/

void Menu_State::show() {
    _graphic->displayMenu();
}

void Menu_State::back() {

}

void Menu_State::next() {

}

void Menu_State::up() {

}

void Menu_State::down() {

}

void Menu_State::select() {

}