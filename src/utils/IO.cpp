#include "IO.h"
#include <windows.h>

std::string get_absolute_path(std::string relative_path) {
    char full_path[MAX_PATH];
    GetFullPathName(relative_path.c_str(), MAX_PATH, full_path, NULL);
    return std::string(full_path);
}
