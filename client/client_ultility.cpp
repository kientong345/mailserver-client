#include "client_inc.h"
#include "client_ultility.h"
#include <sstream>
#include <memory>

/**
 * @brief wait key input from user
 * @return the inputted key
 */
INPUT_TYPE get_user_input() {
    INPUT_TYPE ret;
    return ret;
}

/**
 * @brief parse a request message into corresponding type and content
 * @param _message: request message
 * @return a data structure which contains request type and additional infomation
 */
req_t parseRequest(const std::string& _message) {
    req_t req;
    std::string req_type = getWord(_message, 1);

    if ((req_type == LOGIN) ||
        (req_type == REGISTER) ||
        (req_type == SENDTO) ||
        (req_type == DELETEMAIL) ||
        (req_type == CHANGENAME) ||
        (req_type == SEARCH) ||
        (req_type == TERMINATE) ||
        (req_type == WHATISMYNAME) ||
        (req_type == CHANGEPASSWORD)) {
        req.first = REQ_TOSERVER;
        req.second = std::make_shared<std::string>(_message); // forward the request directly without parsing it!
    }
    else if (req_type == TOSERVER) {
        req.first = REQ_TOSERVER;
        req.second = std::make_shared<std::string>(getWord(_message, 2)); // message to be sent to server
    }
    else if (req_type == SHOWCHAT) {
        req.first = REQ_SHOWCHAT;
        req.second = std::make_shared<std::string>(getWord(_message, 2)); // chat history between you and this user
    }
    else if (req_type == INPUT_BACK) {
        req.first = REQ_BACK;
        req.second = nullptr;
    }
    else if (req_type == INPUT_NEXT) {
        req.first = REQ_NEXT;
        req.second = nullptr;
    }
    else if (req_type == INPUT_UP) {
        req.first = REQ_UP;
        req.second = nullptr;
    }
    else if (req_type == INPUT_DOWN) {
        req.first = REQ_DOWN;
        req.second = nullptr;
    }
    else if (req_type == INPUT_SELECT) {
        req.first = REQ_SELECT;
        req.second = nullptr;
    }
    else if (req_type == INPUT_SWITCH) {
        req.first = REQ_SWITCH;
        req.second = nullptr;
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