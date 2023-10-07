#include "cJSON.h"
#include <iostream>

class CJSONWrapper {
public:
    CJSONWrapper(cJSON* node) : node(node) {}

    CJSONWrapper(CJSONWrapper&& other) : node(other.node) {
        other.node = nullptr;
    }

    ~CJSONWrapper() {
        if(node == nullptr) {
            std::cerr << "CJSONWrapper::~CJSONWrapper() - node is nullptr" << std::endl;
            return;
        }

        // std::cout << "CJSONWrapper::~CJSONWrapper() - deleting node" << std::endl;

        cJSON_Delete(node);
        // *node = nullptr;
    }

    public:
    cJSON* value() {
        return node;
    }

    private:
        cJSON* node;
};
