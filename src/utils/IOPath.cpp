#include "IOPath.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>

std::string IOPath::get_absolute_path(std::string relative_path) {
    char full_path[MAX_PATH];
    GetFullPathName(relative_path.c_str(), MAX_PATH, full_path, NULL);
    return std::string(full_path);
}

std::string IOPath::get_containing_dir(std::string abs_path_with_filename) {
    //PathRemoveFileSpec(dir); // remove the file name and extension
    auto last_slash = abs_path_with_filename.find_last_of("\\/");
    if (std::string::npos != last_slash)
    {
        return abs_path_with_filename.substr(0, last_slash);
    }
    return "";
}
