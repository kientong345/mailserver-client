#include "mailclient_ctrl.h"
#include <iostream>
#include <signal.h>

static std::string THIS_CLIENT_NAME;

Client::Client()
: client_fd(-1) {
    send_buf[0] = '\0';
    rcv_buf[0] = '\0';
}

Client::~Client() {
    if (send_thread.joinable()) send_thread.join();
    if (recv_thread.joinable()) recv_thread.join();
}

ERROR_CODE Client::setup(const std::string& _ipaddress, const uint16_t& _port) {
    /* create socket */
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        return E_SOCKET_FAILED;
    }
    if (_ipaddress != "") {
        /* setup client */
        struct sockaddr_in client_addr;
        uint32_t client_ip = 0;
        inet_pton(AF_INET, _ipaddress.c_str(), &client_ip); /* change client ip in need */
        client_addr.sin_family = AF_INET;
        client_addr.sin_addr.s_addr = client_ip;
        client_addr.sin_port = htons(_port);

        if (bind(client_fd, (struct sockaddr*)&client_addr, (socklen_t)sizeof(client_addr)) < 0) {
            return E_BIND_FAILED;
        }
    }
    return E_SUCCESS;
}

ERROR_CODE Client::connect(const std::string& _ipaddress, const uint16_t& _port) {
    /* indicate server */
    struct sockaddr_in server_addr;
    uint32_t server_ip;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, _ipaddress.c_str(), &server_ip);
    server_addr.sin_addr.s_addr = server_ip;
    server_addr.sin_port = htons(_port);
    /* connect to that server */
    if (::connect(client_fd, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr)) < 0) {
        return E_CONNECT_FAILED;
    }
    std::cout << "connected to " + _ipaddress + "-port:" + std::to_string(_port) + "\n";
    return E_SUCCESS;
}

ERROR_CODE Client::sendRequest(const std::string& _message) {
    {
        std::lock_guard<std::mutex> glock(send_mut);
        std::copy(_message.begin(), _message.end(), send_buf);
        send_buf[_message.size()] = '\0';
    }
    send_cv.notify_one();
    WAIT_MS(10);
    if (send_buf[0] != '\0') return E_SEND_FAILED;
    return E_SUCCESS;
}

ERROR_CODE Client::receiveResponse(std::string& _res) {
    if (rcv_buf[0] == '\0') return E_RCV_FAILED;
    _res = rcv_buf;
    rcv_buf[0] = '\0';
    return E_SUCCESS;
}

void Client::start() {
    send_thread = std::thread(&Client::send_thread_func, this);
    recv_thread = std::thread(&Client::recv_thread_func, this);
    std::cout << "enter user name: ";
    std::string reg_name;
    std::getline(std::cin, reg_name);
    sendRequest(reg_name);
    updateMyName();
    gui.displayMainScreen();

    while(1) {
        std::string request;
        std::getline(std::cin, request);
        auto req = parseRequest(request);
        executeRequest(req);
    }
}

void Client::shutdown() {

}

const sent_mail* Client::checkmail(std::vector<sent_mail> _sent_mailbox, uint16_t _mail_position) {
    sent_mail* _mail = nullptr;

    std::shared_lock<std::shared_mutex> slock(sent_mailbox_mut);
    if ((_mail_position-1 > _sent_mailbox.size()) && (_mail_position != NEWEST_MAIL)) return nullptr;
    switch (_mail_position) {
    case OLDEST_MAIL:
        _mail = &_sent_mailbox.at(0);
        break;
    case NEWEST_MAIL:
        _mail = &_sent_mailbox.back();
        break;
    default:
        _mail = &_sent_mailbox.at(_mail_position-1);
        break;
    }
    slock.unlock();

    return _mail;
}

const received_mail* Client::checkmail(std::vector<received_mail> _rcv_mailbox, uint16_t _mail_position) const {
    if ((_mail_position > _rcv_mailbox.size()) && (_mail_position != NEWEST_MAIL)) return nullptr;
    received_mail* _mail;
    switch (_mail_position) {
    case OLDEST_MAIL:
        _mail = &_rcv_mailbox.at(0);
        break;
    case NEWEST_MAIL:
        _mail = &_rcv_mailbox.back();
        break;
    default:
        _mail = &_rcv_mailbox.at(_mail_position-1);
        break;
    }
    return _mail;
}

void Client::send_thread_func() {
    auto get_req_type = [this](){
        std::string _type = "";
        const char* tmp_buf = send_buf;
        while (*tmp_buf == ' ') {
            if (*tmp_buf == '\0') return static_cast<std::string>("");
            ++tmp_buf;
        }
        while (*tmp_buf != ' ') {
            if (*tmp_buf == '\0') return static_cast<std::string>("");
            _type += *tmp_buf;
            ++tmp_buf;
        }
        return _type;
    };
    auto get_receiver = [this](){
        std::string _receiver = "";
        const char* tmp_buf = send_buf;
        while (*tmp_buf == ' ') {
            if (*tmp_buf == '\0') return static_cast<std::string>("");
            ++tmp_buf;
        }
        while (*tmp_buf != ' ') {
            if (*tmp_buf == '\0') return static_cast<std::string>("");
            ++tmp_buf;
        }
        while (*tmp_buf == ' ') {
            if (*tmp_buf == '\0') return static_cast<std::string>("");
            ++tmp_buf;
        }
        while (*tmp_buf != ' ') {
            if (*tmp_buf == '\0') return static_cast<std::string>("");
            _receiver += *tmp_buf;
            ++tmp_buf;
        }
        return _receiver;
    };
    auto get_content = [this](){
        std::string _content;
        const char* tmp_buf = send_buf;
        while ((*tmp_buf != '\"') && (*tmp_buf != '\'')) {
            if (*tmp_buf == '\0') return static_cast<std::string>("");
            ++tmp_buf;
        }
        ++tmp_buf;
        while ((*tmp_buf != '\"') && (*tmp_buf != '\'')) {
            if (*tmp_buf == '\0') return _content;
            _content += *tmp_buf;
            ++tmp_buf;
        }
        return _content;
    };
    while(1) {
        std::unique_lock<std::mutex> ulock(send_mut);
        send_cv.wait(ulock, [this](){return send_buf[0] != '\0';});
        if (send(client_fd, send_buf, sizeof(send_buf), 0) < 0) {
            std::cerr << "sent failed\n";
            continue;
        }
        if (get_req_type() == "sendto") {
            std::lock_guard<std::shared_mutex> glock(sent_mailbox_mut);
            sent_mailbox.emplace_back<sent_mail>({get_receiver(), get_content()});
        }
        send_buf[0] = '\0';
    }
}

void Client::recv_thread_func() {
    auto get_sender = [this](){
        std::string _sender = "";
        const char* tmp_buf = rcv_buf;
        while (*tmp_buf != '[') {
            if (*tmp_buf == '\0') return static_cast<std::string>("");
            ++tmp_buf;
        }
        ++tmp_buf;
        while (*tmp_buf != ']') {
            if (*tmp_buf == '\0') return static_cast<std::string>("");
            _sender += *tmp_buf;
            ++tmp_buf;
        }
        return _sender;
    };
    auto get_content = [this](){
        std::string _content = "";
        const char* tmp_buf = rcv_buf;
        while (*tmp_buf != ']') {
            if (*tmp_buf == '\0') return static_cast<std::string>("");
            ++tmp_buf;
        }
        _content += (tmp_buf+1);
        return _content;
    };

    while(1) {
        int bytercv = recv(client_fd, rcv_buf, BUF_SIZE, 0);
        rcv_buf[bytercv] = '\0';
        std::lock_guard<std::shared_mutex> slock(rcv_mailbox_mut);
        received_mailbox.emplace_back<received_mail>({get_sender(), get_content()});
        rcv_buf[0] = '\0';
    }
}

std::pair<REQ_TYPE, std::shared_ptr<void>> Client::parseRequest(const std::string& _message) const {
    /*  protocol syntax:
    toserver <message>
    showmail <sent/received> <mail_position>
    deletemail <sent/received> <mail_position>
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

    /* get req_type */
    req_type = get_next_word();
    /* analyze */
    if (req_type == "sendto") {
        req.first = REQ_SENDTO;
        req.second = std::move(std::make_shared<std::string>(_message));
    }
    else if (req_type == "changename") {
        req.first = REQ_CHANGENAME;
        req.second = std::move(std::make_shared<std::string>(_message));
    }
    else if (req_type == "search") {
        req.first = REQ_SEARCH;
        req.second = std::move(std::make_shared<std::string>(_message));
    }
    else if (req_type == "term") {
        req.first = REQ_TERM;
        req.second = std::move(std::make_shared<std::string>(_message));
    }
    else if (req_type == "whatismyname") {
        req.first = REQ_WHATISMYNAME;
        req.second = std::move(std::make_shared<std::string>(_message));
    }
    else if (req_type == "toserver") {
        req.first = REQ_TOSERVER;
        req.second = std::move(std::make_shared<std::string>(get_next_word()));
    }
    else if (req_type == "showmail") {
        req.first = REQ_SHOWMAIL;
        std::string content1 = get_next_word();
        uint16_t content2 = std::stoi(get_next_word());
        req.second = std::move(std::make_shared<std::pair<std::string, uint16_t>>(content1, content2));
    }
    else if (req_type == "deletemail") {
        req.first = REQ_DELETEMAIL;
        std::string content1 = get_next_word();
        uint16_t content2 = std::stoi(get_next_word());
        req.second = std::move(std::make_shared<std::pair<std::string, uint16_t>>(content1, content2));
    }
    else { // unidentify request
        req.first = REQ_UNIDENTIFY;
        req.second = std::move(std::make_shared<std::string>(_message));
    }

    return req;
}

void Client::executeRequest(const std::pair<REQ_TYPE, std::shared_ptr<void>>& _req) {
    REQ_TYPE req_type = _req.first;
    if (req_type == REQ_CHANGENAME) updateMyName();

    if ((req_type == REQ_TOSERVER) ||
        (req_type == REQ_SENDTO) ||
        (req_type == REQ_CHANGENAME) ||
        (req_type == REQ_SEARCH) ||
        (req_type == REQ_TERM) ||
        (req_type == REQ_WHATISMYNAME)) {
        std::string _message = *(static_cast<std::string*>(_req.second.get()));
        while(sendRequest(_message) != E_SUCCESS);
    }
    else if (req_type == REQ_SHOWMAIL) {
        auto content = static_cast<std::pair<std::string, uint16_t>*>(_req.second.get());
        std::string type_ = content->first;
        uint16_t pos_ = content->second;
        if (type_ == "sent_mailbox") {
            std::shared_lock<std::shared_mutex> slock(sent_mailbox_mut);
            if (pos_ == 0) gui.displaySentMailbox(sent_mailbox);
            else {
                if (pos_ > sent_mailbox.size()) std::cout << "mail out of range\n";
                else gui.displaySentMail(sent_mailbox, pos_);
            }
            slock.unlock();
        }
        else if (type_ == "received_mailbox") {
            std::shared_lock<std::shared_mutex> slock(rcv_mailbox_mut);
            if (pos_ == 0) gui.displayReceivedMailbox(received_mailbox);
            else {
                if (pos_ > received_mailbox.size()) std::cout << "mail out of range\n";
                else gui.displayReceivedMail(received_mailbox, pos_);
            }
            slock.unlock();
        }
    }
    else if (req_type == REQ_DELETEMAIL) {
        auto content = static_cast<std::pair<std::string, uint16_t>*>(_req.second.get());
        std::string type_ = content->first;
        uint16_t pos_ = content->second;
        if (type_ == "sent_mailbox") {
            std::lock_guard<std::shared_mutex> slock(sent_mailbox_mut);
            if (pos_ > sent_mailbox.size()) std::cout << "mail out of range\n";
            else sent_mailbox.erase(sent_mailbox.begin() + pos_-1);
        }
        else if (type_ == "received_mailbox") {
            std::lock_guard<std::shared_mutex> slock(rcv_mailbox_mut);
            if (pos_ > received_mailbox.size()) std::cout << "mail out of range\n";
            else received_mailbox.erase(received_mailbox.begin() + pos_-1);
        }
    }
    else { /* REQ_UNIDENTIFY */
        std::string this_req = *(static_cast<std::string*>(_req.second.get()));
        std::cout << "unidentified request: \'" + this_req + "\'\n";
    }
}

void Client::updateMyName() {
    auto update_my_name = [this](){
        for (int i = 0; i < 2; ++i) {  
            WAIT_MS(5);
            sendRequest("whatismyname");
            WAIT_MS(5);
            const received_mail* _mail = nullptr;
            uint8_t cnt = 0;
            while ((_mail == nullptr)) {
                std::shared_lock<std::shared_mutex> slock(rcv_mailbox_mut);
                _mail = checkmail(received_mailbox, NEWEST_MAIL);
                slock.unlock();
            }
            THIS_CLIENT_NAME = _mail->content;
        }
    };
    std::thread updatename_thread(update_my_name);
    updatename_thread.detach();
}