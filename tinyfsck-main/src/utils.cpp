#include "utils.h"

#include <blkid/blkid.h>
#include <ext2fs/ext2fs.h>

std::string get_fs_type(std::string& path) {
    blkid_probe probe;
    const char* type_cstr;

    probe = blkid_new_probe_from_filename(path.c_str());
    if (!probe)
        return "";

    blkid_do_probe(probe);
    blkid_probe_lookup_value(probe, "TYPE", &type_cstr, NULL);

    std::string fs_type(type_cstr);

    blkid_free_probe(probe);

    return fs_type;
}

bool is_fs_mounted(std::string& path, std::string& mount_point) {
    int mount_flags;
    char mount_point_cstr[1024];
    errcode_t retval = ext2fs_check_mount_point(
        path.c_str(),
        &mount_flags,
        mount_point_cstr,
        sizeof(mount_point)
    );

    if (retval == 0) {
        if (mount_flags & EXT2_MF_MOUNTED) {
            mount_point = std::string(mount_point_cstr);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}