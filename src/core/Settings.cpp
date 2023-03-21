#include "Settings.h"

// constexpr char Settings::Window::TITLE[] = "Some title";

// #include <iostream>
// #include <fstream>
// #include "rapidjson/document.h"

// int main() {
//     std::ifstream file("data.json");
//     if (!file.is_open()) {
//         std::cerr << "Failed to open file." << std::endl;
//         return 1;
//     }

//     rapidjson::IStreamWrapper stream(file);
//     rapidjson::Document doc;
//     doc.ParseStream(stream);

//     if (doc.HasParseError()) {
//         std::cerr << "Failed to parse JSON." << std::endl;
//         return 1;
//     }

//     const auto& myObject = doc["myObject"];
//     const auto& myArray = doc["myArray"];

//     std::cout << "myObject.name: " << myObject["name"].GetString() << std::endl;
//     std::cout << "myObject.value: " << myObject["value"].GetInt() << std::endl;

//     for (const auto& item : myArray.GetArray()) {
//         std::cout << "myArray item: " << item.GetInt() << std::endl;
//     }

//     return 0;
// }
