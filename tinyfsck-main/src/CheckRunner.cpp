#include "CheckRunner.h"

CheckRunner::~CheckRunner() {
    for (auto& c : checks)
        delete c;
}

void CheckRunner::add_check(Check* check) {
    checks.push_back(check);
}

void CheckRunner::run_checks() {
    for (auto& c : checks)
        c->run();
}

void CheckRunner::print_report() {

}

void CheckRunner::run_fixes() {
    
}