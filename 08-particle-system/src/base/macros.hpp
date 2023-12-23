#pragma once 

#include <iostream>

#define LOG_DEBUG "debug"
#define LOG_WARN "warning"
#define LOG_ERR "error"

#define LOG(level, ...) std::cout << "[" << __TIME__ << " " << level << "]: " << __VA_ARGS__ << std::endl;

#define RET_OK 0 
#define RET_ERR -1 
