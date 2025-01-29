#ifndef __CLIENT_ULTILITY_H__
#define __CLIENT_ULTILITY_H__

#include "client_inc.h"
#include <string>
#include <vector>
#include <jsoncpp/json/json.h>

req_t parseRequest(const std::string& _message);
std::string getWord(const std::string& _sentence, uint8_t _wordpos);
std::string get_std_time(uint64_t unix_timestamp);
Json::Value string_to_json(const std::string& _data);
std::vector<std::pair<std::string, USER_STATUS>> get_user_list(const Json::Value& _usrlist);
std::string mail_form(const std::string& _receiver, const std::string& _content, const std::string& _sent_time);
std::string getHeader(const chat_line& _chatline);

#endif /* __CLIENT_ULTILITY_H__ */