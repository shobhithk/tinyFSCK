#pragma once

#include "CheckContext.h"
#include "./Check/Check.h"

class CheckRunner {
public:
    void add_check(Check* check);

    void run_checks();
    void print_report();
    void run_fixes();

    ~CheckRunner();
private:
    CheckContext& ctx = CheckContext::instance();
    std::vector<Check*> checks;
};