#pragma once

#include <unordered_map>
#include <string>

class ArgParser {
public:
    struct ResultWrapper {
        operator int() {
            return thisArgParser->args[key].get_int_val();
        }

        operator std::string() {
            return thisArgParser->args[key].get_string_val();
        }

        operator bool() {
            return thisArgParser->args[key].get_bool_val();
        }

        ArgParser* thisArgParser;
        std::string& key;
    };

    ArgParser(int argc, char** argv);
    ArgParser::ResultWrapper get_arg(std::string arg_key);
    size_t num();
    bool is_insufficient();

private:
    class ArgVal {
    public:
        ArgVal() = default;

        ArgVal(int val) 
            :int_val {val} {}

        ArgVal(std::string val) 
            :string_val {val} {}

        ArgVal(bool val)
            :bool_val {val} {}

        inline int get_int_val() { return int_val; }
        inline std::string get_string_val() { return string_val; }
        inline bool get_bool_val() { return bool_val; }

    private:
        std::string string_val;
        int int_val;
        bool bool_val;
    };

    std::unordered_map<std::string, ArgVal> args;
    bool insufficient = false;
};
