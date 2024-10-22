#pragma once
#include <string>

namespace IOPath {
    std::string get_absolute_path(std::string relative_path);
    std::string get_containing_dir(std::string abs_path_with_filename);
}
