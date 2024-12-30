#include "server_ultility.h"
#include <sstream>
#include <tuple>

/**
 * @brief get a unique key from given name
 * @param client_name: a name to generate corresponding key
 * @return unique key
 */
int getKeyfromName(const std::string& client_name) {
    std::hash<std::string> hasher;
    int key = static_cast<int>(hasher(client_name) % INT_MAX);
    return (key != NOT_FOUND) ? key : INT_MAX; /* NOT_FOUND(404) is reserved */
}

/**
 * @brief parse a request message into corresponding type and content
 * @param _message: request message
 * @return a data structure which contains request type and additional infomation
 */
req_t parseRequest(const std::string& _message) {
    req_t req;
    std::string req_type = getWord(_message, 1);

    if (req_type == LOGIN) {
        req.first = REQ_LOGIN;
        req.second = std::make_shared<std::pair<std::string, std::string>>(getWord(_message, 2), getWord(_message, 3));
    }
    else if (req_type == REGISTER) {
        req.first = REQ_REGISTER;
        req.second = std::make_shared<std::pair<std::string, std::string>>(getWord(_message, 2), getWord(_message, 3));
    }
    else if (req_type == SENDTO) {
        req.first = REQ_SENDTO;
        req.second = std::make_shared<std::tuple<std::string, std::string, std::string>>(getWord(_message, 2), getWord(_message, 3), getWord(_message, 4));
        /* word 2: receiver name, word 3: sent content */
    }
    else if (req_type == DELETEMAIL) {
        req.first = REQ_DELETEMAIL;
        req.second = std::make_shared<std::pair<std::string, uint16_t>>(getWord(_message, 2), std::stoi(getWord(_message, 3)));
        /* word 2: sent/rcv mailbox, word 3: position of to-be-deleted mail */
    }
    else if (req_type == CHANGENAME) {
        req.first = REQ_CHANGENAME;
        req.second = std::make_shared<std::string>(getWord(_message, 2));
        /* word 2: new name */
    }
    else if (req_type == SEARCH) {
        req.first = REQ_SEARCH;
        req.second = std::make_shared<std::string>(getWord(_message, 2));
        /* word 2: name of client to find */
    }
    else if (req_type == TERMINATE) {
        req.first = REQ_TERMINATE;
        req.second = nullptr;
    }
    else if (req_type == WHATISMYNAME) {
        req.first = REQ_WHATISMYNAME;
        req.second = nullptr;
    }
    else if (req_type == CHANGEPASSWORD) {
        req.first = REQ_CHANGEPASSWORD;
        req.second = std::make_shared<std::string>(getWord(_message, 2));
    }
    else { // unidentify request
        req.first = REQ_UNIDENTIFY;
        req.second = std::move(std::make_shared<std::string>(_message));
    }
    
    return req;
}

/**
 * @brief get a single word in a sentence by position
 * @param _sentence: the sentence
 * @param _wordpos: position of the word in the sentence
 * @return the word in that position
 */
std::string getWord(const std::string& _sentence, uint8_t _wordpos) {
    if (_wordpos < 1) return "";

    std::istringstream iss(_sentence);
    std::string word;

    auto get_next_word = [&iss](std::string& _word){
        if(!(iss >> _word)) _word = "";
        if ((_word[0] == '\'') || (_word[0] == '\"')) {
            std::string tmp(_word);
            while ((tmp.back() != '\'') && (tmp.back() != '\"')) {
                if (!(iss >> tmp)) {
                    _word += '\'';
                    break;
                }
                _word += (" " + tmp);
            }
            _word.erase(0, 1);
            _word.pop_back();
        }
    };

    while (_wordpos) {
        get_next_word(word);
        --_wordpos;
    }

    return word;
}