#ifndef __SERVER_ULTILITY_H__
#define __SERVER_ULTILITY_H__

#include "server_inc.h"
#include <jsoncpp/json/json.h>
#include <string>
#include <map>

int getKeyfromName(const std::string& client_name);
req_t parseRequest(const std::string& _message);
std::string getWord(const std::string& _sentence, uint8_t _wordpos);
std::string mail_form(const std::string& _sender, const std::string& _content, const std::string& _sent_time);
std::string json_mail_form(const std::string& _sender, const Json::Value& _content, const std::string& _sent_time);
Json::Value data_to_json(const std::vector<std::pair<std::string, USER_STATUS>>& _userlist);

#endif /* __SERVER_ULTILITY_H__ */