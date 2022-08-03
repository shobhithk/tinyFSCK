#pragma once

#include <string>
#include "../CheckContext.h"

#define CHECK_INFO(i) \
    public: \
    virtual std::string info() { \
        return "[" + name + "]" + i; \
    }

class Check {
public:
    virtual ~Check() {  }
    virtual std::string info() { return ""; }
    virtual bool run() { return false; }
protected:
    CheckContext& ctx = CheckContext::instance();
    std::string name;
};