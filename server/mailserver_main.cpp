#include "mailserver_ctrl.h"
#include <iostream>

int main() {
    Server* my_server = Server::getInstance();

    my_server->setup();
    my_server->start();

    return 0;
}