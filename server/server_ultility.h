#ifndef __SERVER_ULTILITY_H__
#define __SERVER_ULTILITY_H__

#include "server_inc.h"
#include <string>

int getKeyfromName(const std::string& client_name);
req_t parseRequest(const std::string& _message);
std::string getWord(const std::string& _sentence, uint8_t _wordpos);
std::string mail_form(const std::string& _sender, const std::string& _content, const std::string& _sent_time);

#endif /* __SERVER_ULTILITY_H__ */