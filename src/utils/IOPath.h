#pragma once
#include <string>
#include <vector>

namespace IOPath {
    std::string get_absolute_path(std::string relative_path);
    std::string get_containing_dir(std::string abs_path_with_filename);
//     std::vector<std::string> get_dirs_content(std::string dir_abs_path);
//     std::vector<std::string> get_dirs_content(const std::string& dir_abs_path, const std::string& extension);
}
