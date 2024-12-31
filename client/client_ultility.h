#ifndef __CLIENT_ULTILITY_H__
#define __CLIENT_ULTILITY_H__

#include "client_inc.h"
#include <string>

INPUT_TYPE get_user_input();
req_t parseRequest(const std::string& _message);
std::string getWord(const std::string& _sentence, uint8_t _wordpos);

#endif /* __CLIENT_ULTILITY_H__ */