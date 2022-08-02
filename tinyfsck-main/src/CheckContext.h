#pragma once

#include "Filesystem.h"
#include <string>
#include <vector>

class Error {
public:
    virtual std::string what();
    virtual bool can_recover();
    virtual bool recover();
};

struct CheckContext {
    // Singleton context
    
    static CheckContext& instance(std::string fs_path = "") {
        static CheckContext context(fs_path);
        return context;
    }

    CheckContext(std::string path)
        : fs(path) {}

    ~CheckContext() {
        for (auto &e : errors)
            delete e;
    }

    Filesystem fs;
    std::vector<Error*> errors;  
};