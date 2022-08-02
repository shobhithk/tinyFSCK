#include <iostream>
#include <string>

#include "ArgParser.h"
#include "CheckContext.h"
#include "CheckRunner.h"
#include "Filesystem.h"
#include "utils.h"
#include "exitcode.h"

int main(int argc, char **argv) {
    ArgParser args(argc, argv);
    std::string bin = args.get_arg("bin");

    if (args.is_insufficient()) {
        std::cerr << "Not enough arguments\n";
        std::cout << "Usage\n\t" << bin << " <path>" << "\n";
        return TF_INCORRECT_USAGE;
    }

    std::string path = args.get_arg("path");
    std::string fstype = get_fs_type(path);
    std::string mount_point;

    if (fstype == "") {
        std::cerr << "There is no filesystem here! Try a block device like /dev/sda or "
                     "a partition like /dev/sda1\n";
        std::cout << "Usage\n\t" << bin << " <path>" << "\n";

        return TF_INCORRECT_USAGE;
    } else if (fstype != "ext2") {
        std::cerr << "This tool is designed to only check ext2 file systems.\n";

        return TF_NOT_EXT2;
    } else if (is_fs_mounted(path, mount_point)) {
        std::cerr << "The file system is mounted at " << mount_point << ". Running this "
                     "tool on a mounted file system could currupt it. Please unmount it.\n";
        
        return TF_FS_MOUNTED;
    }

    try {
        CheckContext::instance(path);
        CheckRunner runner;

        runner.run_checks();
        runner.print_report();
        if ((bool)args.get_arg("run_fixes") == true) {
            runner.run_fixes();
        }
    } catch(std::runtime_error& err) {
        std::cout << "CRITICAL ERROR : " << err.what() << "\n";
        return TF_CRITCAL_ERROR;
    }

    return TF_SUCCESS;
}