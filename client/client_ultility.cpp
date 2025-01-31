#include "client_inc.h"
#include "client_ultility.h"
#include <sstream>
#include <memory>
#include <ctime>
#include <iomanip>

/**
 * @brief parse a request message into corresponding type and content
 * @param _message: request message
 * @return a data structure which contains request type and additional infomation
 */
req_t parseRequest(const std::string& _message) {
    req_t req;
    std::string req_type = getWord(_message, 1);

    if (//(req_type == LOGIN) ||
        //(req_type == REGISTER) ||
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
    else if (req_type == RELOAD) {
        req.first = REQ_RELOAD;
        req.second = std::make_shared<std::string>(getWord(_message, 2)); // message to be sent to server
    }
    else if (req_type == SHOWCHAT) {
        req.first = REQ_SHOWCHAT;
        req.second = std::make_shared<std::string>(getWord(_message, 2)); // chat history between you and this user
    }
    else if (req_type == INPUT_LEFT) {
        req.first = REQ_LEFT;
        req.second = nullptr;
    }
    else if (req_type == INPUT_RIGHT) {
        req.first = REQ_RIGHT;
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
    else if (req_type == LOGIN) {
        req.first = REQ_LOGIN;
        req.second = std::make_shared<std::pair<std::string, std::string>>(getWord(_message, 2), getWord(_message, 3));
    }
    else if (req_type == REGISTER) {
        req.first = REQ_REGISTER;
        req.second = std::make_shared<std::pair<std::string, std::string>>(getWord(_message, 2), getWord(_message, 3));
    }
    else if (req_type == LOGOUT) {
        req.first = REQ_LOGOUT;
        req.second = nullptr;
    }
    else if (req_type == GIVEMEUSERLIST) {
        req.first = REQ_GIVEMEUSERLIST;
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
        if (_word[0] == '\'') {
            std::string tmp(_word);
            while (tmp.back() != '\'') {
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

/**
 * @brief get standard time from unix timestamp
 * @param unix_timestamp: epoch time since 1970
 * @return current date/time... from epoch time
 */
std::string get_std_time(uint64_t unix_timestamp) {
    std::time_t _time = static_cast<std::time_t>(unix_timestamp);
    std::tm* local_time = std::localtime(&_time);
    std::ostringstream oss;
    oss << std::put_time(local_time, "%d/%m/%Y %H:%M:%S");
    return oss.str();
}

/**
 * @brief convert a json string into json type
 * @param _jsonstring: json string to be converted
 * @return json-type value
 */
Json::Value string_to_json(const std::string& _jsonstring) {
    Json::Value ret;
    Json::Reader jsonrd;
    jsonrd.parse(_jsonstring, ret);
    return ret;
}

std::vector<std::pair<std::string, USER_STATUS>> get_user_list(const Json::Value& _usrlist) {
    std::vector<std::pair<std::string, USER_STATUS>> ret;
    for (const auto& item : _usrlist) {
        if (item.isObject()) {
            for (const auto& usrname : item.getMemberNames()) {
                if (item[usrname].isInt()) {
                    ret.emplace_back(usrname, static_cast<USER_STATUS>(item[usrname].asInt()));
                }
            }
        }
    }
    return ret;
}

/**
 * @brief get a standard mail-form message from given content and info
 * @param _sender: the sender of the mail
 * @param _content: mail content
 * @param _sent_time: sent time
 * @return standard mail-form string
 */
std::string mail_form(const std::string& _receiver, const std::string& _content, const std::string& _sent_time) {
    std::string ret("");
    ret += "\'";
    ret += _receiver; ret += "\' \'";
    ret += _content; ret += "\' ";
    ret += _sent_time;

    return ret;
}

std::string getHeader(const chat_line& _chatline) {
    return std::string(get_std_time(_chatline.chat_time) + " @" + _chatline.chat_owner + ": ");
}