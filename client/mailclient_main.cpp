#include "mailclient_ctrl.h"
#include <iostream>

Client my_client;

int main() {
    my_client.setup();
    my_client.connect(SERVER_IP, SERVER_PORT);
    my_client.start();

    return 0;
}