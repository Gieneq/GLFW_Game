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
    auto last_slash = abs_path_with_filename.find_last_of("\\/");
    if (std::string::npos != last_slash) {
        return abs_path_with_filename.substr(0, last_slash);
    }
    return "";
}

// std::vector<std::string> IOPath::get_dirs_content(const std::string& dir_abs_path, const std::string& extension) {
//     std::vector<std::string> files;
//     std::string search_path = dir_abs_path + "/*." + extension;
//     WIN32_FIND_DATA fd;
//     HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
//     if(hFind != INVALID_HANDLE_VALUE) {
//         do {
//             if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
//                 files.push_back(fd.cFileName);
//             }
//         } while(::FindNextFile(hFind, &fd));
//         ::FindClose(hFind);
//     }
//     return files;
// }