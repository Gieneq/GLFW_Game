#include "IO.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>

std::string IO::get_absolute_path(std::string relative_path) {
    char full_path[MAX_PATH];
    GetFullPathName(relative_path.c_str(), MAX_PATH, full_path, NULL);
    return std::string(full_path);
}

std::string IO::get_containing_dir(std::string abs_path_with_filename) {
    //PathRemoveFileSpec(dir); // remove the file name and extension
    auto last_slash = abs_path_with_filename.find_last_of("\\/");
    if (std::string::npos != last_slash)
    {
        return abs_path_with_filename.substr(0, last_slash);
    }
    return "";
}

// string load_some_xml() {

//     //read the xml file into vector
//     auto path = get_absolute_path("res/Exchange.xml");
//     cout << path << endl;
//     ifstream xmlFile(path.c_str());
//     xmlFile.unsetf(ios::skipws);
//     vector<char> buffer ((istreambuf_iterator<char>(xmlFile)), istreambuf_iterator<char>());
//     buffer.push_back('\0');

//     string xml_as_string = string(buffer.begin(), buffer.end());
//     return xml_as_string;
// }


// void test_xml() {
//     cout << endl;
//     cout << endl;
//     // auto xml_string = load_some_xml();
//     // cout << xml_string << endl;


//     auto path = get_absolute_path("res/Exchange.xml");
//     pugi::xml_document doc;
//     pugi::xml_parse_result result = doc.load_file(path.c_str());
//     if (!result)
//         return ;
//     //////////////////////////////////////////
        
//     // for (pugi::xml_node tool: doc.child("Profile").child("Tools").children("Tool"))
//     // {
//     //     int timeout = tool.attribute("Timeout").as_int();
        
//     //     if (timeout > 0)
//     //         std::cout << "Tool " << tool.attribute("Filename").value() << " has timeout " << timeout << "\n";
//     // }

//     std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
//     cout << endl;
//     cout << endl;
// }
