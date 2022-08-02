#include "ArgParser.h"

ArgParser::ArgParser(int argc, char** argv) {
    args["bin"] = std::string(argv[0]);
    if (argc >= 2) {
        // The last argument is always the path
        args["path"] = std::string(argv[argc - 1]);

        for (int i = 1; i < argc - 1; i++) {
            // TODO: Parse the remaining arguments here, if any
        }
    } else {
        insufficient = true;
    }
}

ArgParser::ResultWrapper ArgParser::get_arg(std::string arg_key) {
    return ArgParser::ResultWrapper { this, arg_key };
}

size_t ArgParser::num() {
    return args.size();
}

bool ArgParser::is_insufficient() {
    return num() < 2;
}
