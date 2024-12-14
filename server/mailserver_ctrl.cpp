#include "mailserver_ctrl.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <functional>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <thread>

Server::Server()
: server_fd(-1) {
    shared_memory_object::remove("client_table");
    segment = managed_shared_memory(open_or_create, "client_table", 1024);
    rcv_buf[0] = '\0';
}

Server::~Server() {

}

Server* Server::getInstance() {
    static Server _instance;
    return &_instance;
}

ERROR_CODE Server::setup() {
    struct sockaddr_in server_addr;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        return E_SOCKET_FAILED;
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t)sizeof(opt)) < 0) {
        return E_OPT_FAILED;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr)) < 0) {
        return E_BIND_FAILED;
    }

    /* make client_table be common between processes */
    client_table = segment.construct<shm_map>("client_table")(std::less<key_type>(), segment.get_segment_manager());
    
    return E_SUCCESS;
}

ERROR_CODE Server::start() {
    if (listen(server_fd, MAXIMUM_CONNECTED_CLIENT) < 0) {
        return E_LISTEN_FAILED;
    }
    std::cout << "server is listening ...\n";

    while (1) {
        /* accept client */
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            std::cerr << "accept failed\n";
            continue;
        }
        std::cout << "connected to " << inet_ntoa(client_addr.sin_addr) << " - port " << std::to_string(ntohs(client_addr.sin_port)) << std::endl;
        int client_key = saveClientInfo(client_addr);
        
        int pid = fork();
        if (pid < 0) {
            std::cerr << "fork failed\n";
            continue;
        }
        if (pid == 0) {
            client_handler(client_fd, client_key); /* in child process */
        }
        else {
            close(client_fd); /* in parent process */
        }
    }
    segment.destroy<shm_map>("client_table");
    return E_SUCCESS;
}

void Server::shutdown() {
    close(server_fd);
}

ERROR_CODE Server::restart() {
    return E_SUCCESS;
}

int Server::saveClientInfo(const struct sockaddr_in& _client_addr, const std::string& _client_name) {
    std::string __client_name = _client_name;
    auto no_duplicate_name = [this](const std::string& _name){
        auto it = client_table->begin();
        while (it != client_table->end()) {
            if (_name == it->second.client_name) return false;
            ++it;
        }
        return true;
    };
    /* auto generate a name if name is not specified */
    if (__client_name == "") {
        __client_name += "client_";
        uint8_t index = 1;
        while (!no_duplicate_name(__client_name + std::to_string(index))) ++index;
        // while (client_table->find(client_name + std::to_string(index)) != client_table->end()) {
        //     ++index;
        // }
        __client_name += std::to_string(index);
    }
    /* check if _client_name has already existed? */
    else {
        if (!no_duplicate_name(_client_name)) return NOT_FOUND; // duplicated name mean unique_key is not found :)
    }

    /* generate unique key for each client registed name */
    int unique_key = getKeyfromName(__client_name);
    while ((client_table->find(unique_key) != client_table->end()) || (unique_key == NOT_FOUND)) ++unique_key;

    struct client_info_t _client_info = {__client_name, _client_addr};

    /* register client to client table */
    while (!client_table->insert({unique_key, _client_info}).second) {
        client_table->erase(unique_key);
    }

    return unique_key;
}

std::pair<REQ_TYPE, std::shared_ptr<void>> Server::parseRequest(const std::string& _message) const {
    /*  protocol syntax:
    sendto <client_name>
    search <client_name>
    changename <new_name>
    term
    */
    std::pair<REQ_TYPE, std::shared_ptr<void>> req;
    std::string req_type;
    auto it = _message.begin();
    /* move it pass blank region to next word */
    auto it_jmp_to_word = [&it](){
        uint8_t _limit = 0;
        while((*it == ' ') && (*it != '\0')) {
            ++it;
        }
        return true;
    };
    /* get the next word in the message */
    auto get_next_word = [&it, &it_jmp_to_word](){
        std::string _word;
        bool is_quote = false;
        if (it_jmp_to_word()) {
            while(((*it != ' ')||(is_quote)) && (*it != '\0')){
                if ((*it == '\'') || (*it == '\"')) is_quote = ~is_quote;
                else _word += *it;
                ++it;
            }
        }
        return _word;
    };
    /* get the client key according to client name */
    auto get_client_key = [this](const std::string& _name){
        auto __it = client_table->begin();
        while (__it != client_table->end()) {
            if (__it->second.client_name == _name) return __it->first;
            ++__it;
        }
        return NOT_FOUND;
    };

    /* get req_type */
    req_type = get_next_word();
    /* analyze */
    if (req_type == "sendto") {
        req.first = REQ_SENDTO;
        std::string content1 = get_next_word();
        std::string content2 = get_next_word();
        int key_content = get_client_key(content1);
        req.second = std::move(std::make_shared<std::pair<int, std::string>>(key_content, content2));
    }
    else if (req_type == "changename") {
        req.first = REQ_CHANGENAME;
        req.second = std::move(std::make_shared<std::string>(get_next_word()));
    }
    else if (req_type == "search") {
        req.first = REQ_SEARCH;
        req.second = std::move(std::make_shared<std::string>(get_next_word()));
    }
    else if (req_type == "term") {
        req.first = REQ_TERMINATE;
        req.second = nullptr;
    }
    else if (req_type == "whatismyname") {
        req.first = REQ_WHATISMYNAME;
        req.second = nullptr;
    }
    else { // unidentify request
        req.first = REQ_UNIDENTIFY;
        req.second = std::move(std::make_shared<std::string>(_message));
    }

    return req;
}

int Server::getKeyfromName(const std::string& client_name) const {
    std::hash<std::string> hasher;
    int key = static_cast<int>(hasher(client_name) % INT_MAX);
    return (key != NOT_FOUND) ? key : INT_MAX; /* NOT_FOUND(404) is reserved */
}

void Server::client_handler(const int& client_fd, const int& client_key) {
    key_t msg_key = client_key;
    int msg_id = msgget(msg_key, 0666|IPC_CREAT);
    std::thread msgque_handle_thread(&Server::msgque_handler, this, std::ref(client_fd), std::ref(msg_id));
    while (1) {
        /* receive request */
        int bytercv = recv(client_fd, rcv_buf, BUF_SIZE, 0);
        rcv_buf[bytercv] = '\0';
        std::string _message(rcv_buf);
        std::cout << "server received: " + _message + "\n";

        /* parse request */
        auto req = parseRequest(_message);
        REQ_TYPE req_type = req.first;
        /* execute task for each type of request */
        if (req_type == REQ_SENDTO) {
            auto content = static_cast<std::pair<int, std::string>*>(req.second.get());
            key_t rcver_key = content->first;
            if (rcver_key == NOT_FOUND) {
                while (send(client_fd, "[server]client not found", 17, 0) < 0);
                continue;
            }
            message_t sent_message;
            sent_message.mtype = 1;
            strcpy(sent_message.mtext, ("[" + client_table->at(client_key).client_name + "]" + content->second).c_str());
            int dest_msg_id = msgget(rcver_key, 0666|IPC_CREAT);
            msgsnd(dest_msg_id, &sent_message, sizeof(sent_message.mtext), 0);
        }
        else if (req_type == REQ_CHANGENAME) {
            std::string new_name = *(static_cast<std::string*>(req.second.get()));
            /* check if that name has already existed */
            auto it = client_table->begin();
            while (it != client_table->end()) {
                if (new_name == it->second.client_name) break;
                ++it;
            }
            if (it != client_table->end()) while (send(client_fd, "[server]that name has already existed", 38, 0) < 0);
            else client_table->at(client_key).client_name = new_name;
        }
        else if (req_type == REQ_SEARCH) {
            std::string msg_back = "[server]";
            std::string client_name = *(static_cast<std::string*>(req.second.get()));
            if (client_name == "") {
                for (auto elm : *client_table) {
                    msg_back += " > ";
                    msg_back += elm.second.client_name; msg_back += ": ";
                    msg_back += "IP Address: "; msg_back += inet_ntoa(elm.second.client_addr.sin_addr);
                    msg_back += " - Port: "; msg_back += std::to_string(ntohs(elm.second.client_addr.sin_port));
                    msg_back += "\n";
                }
            }
            else {
                auto it = client_table->begin();
                while (it != client_table->end()) {
                    if (it->second.client_name == client_name) break;
                    ++it;
                }  
                if (it == client_table->end()) {
                    msg_back += "client not found";
                }
                else {
                    msg_back += client_name;    msg_back += ": ";
                    msg_back += "IP Address: "; msg_back += inet_ntoa(it->second.client_addr.sin_addr);
                    msg_back += " - Port: ";    msg_back += std::to_string(ntohs(it->second.client_addr.sin_port));
                }
            }
            while (send(client_fd, msg_back.c_str(), msg_back.size()+1, 0) < 0);
        }
        else if (req_type == REQ_TERMINATE) {
            shutdown();
            exit(0);
        }
        else if (req_type == REQ_WHATISMYNAME) {
            std::string msg_back = "[server]" + client_table->at(client_key).client_name;
            while (send(client_fd, msg_back.c_str(), msg_back.size()+1, 0) < 0);
        }
        else { // req_type == REQ_UNIDENTIFY
            std::string this_req = *(static_cast<std::string*>(req.second.get()));
            std::cout << "unidentified request: \'" + this_req + "\'\n";
        }
    }
    if (msgque_handle_thread.joinable()) msgque_handle_thread.join();
    close(client_fd);
    msgctl(msg_id, IPC_RMID, NULL);
    segment.destroy<shm_map>("client_table");
}

void Server::msgque_handler(const int& client_fd, const int& msg_id) {
    while (1) {
        /* receive message */
        message_t rcv_message;
        if (msg_id < 0) continue;
        if (msgrcv(msg_id, &rcv_message, sizeof(rcv_message.mtext), 1, 0) > 0) {
            while (send(client_fd, rcv_message.mtext, sizeof(rcv_message.mtext), 0) < 0);
        }
    }
}