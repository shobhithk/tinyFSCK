#pragma once
#include <string>

std::string get_fs_type(std::string& path);
bool is_fs_mounted(std::string& path, std::string& mount_point);