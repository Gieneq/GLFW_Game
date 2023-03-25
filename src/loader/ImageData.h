#pragma once
#include <GLFW/glfw3.h>
#include <string>

struct ImageData {
    ImageData() : width(1), height(1), texture_id(0) {}
    ImageData(unsigned width, unsigned height) : width(width), height(height), texture_id(0) {}
    GLuint texture_id;
    unsigned width;
    unsigned height;
    //todo subdivisions
};

class ImageId {
public:
    ImageId() = default;
    ImageId(const std::string& id) : id(id) {}
    std::string id{""};

    size_t operator()(const ImageId& id) const {
        return std::hash<std::string>()(id.id);
    }

    bool operator==(const ImageId& other) const {
        return id == other.id;
    }

    //ImageId::ImageId(const ImageId &)

    // bool operator==(const ImageId& other) const {
    //     return id == other.id;
    // }

    // bool operator!=(const ImageId& other) const {
    //     return id != other.id;
    // }

    // bool operator<(const ImageId& other) const {
    //     return id < other.id;
    // }

    // bool operator>(const ImageId& other) const {
    //     return id > other.id;
    // }

    // bool operator<=(const ImageId& other) const {
    //     return id <= other.id;
    // }

    // bool operator>=(const ImageId& other) const {
    //     return id >= other.id;
    // }

    // bool operator==(const std::string& other) const {
    //     return id == other;
    // }

    // bool operator!=(const std::string& other) const {
    //     return id != other;
    // }

    // bool operator<(const std::string& other) const {
    //     return id < other;
    // }

    // bool operator>(const std::string& other) const {
    //     return id > other;
    // }

    // bool operator<=(const std::string& other) const {
    //     return id <= other;
    // }

    // bool operator>=(const std::string& other) const {
    //     return id >= other;
    // }

    // bool operator==(const char* other) const {
    //     return id == other;
    // }

    // bool operator!=(const char* other) const {
    //     return id != other;
    // }

    // bool operator<(const char* other) const {
    //     return id < other;
    // }

    // bool operator>(const char* other) const {
    //     return id > other;
    // }

    // bool operator<=(const char* other) const {
    //     return id <= other;
    // }

    // bool operator>=(const char* other) const {
    //     return id >= other;
    // }

    // ImageId& operator=(const std::string& other) {
    //     id = other;
    //     return *this;
    // }

    // ImageId& operator=(const char* other) {
    //     id = other;
    //     return *this;
    // }

    // ImageId& operator=(const ImageId& other) {
    //     id = other.id;
    //     return *this;
    // }

    // ImageId& operator=(ImageId&& other) {
    //     id = std::move(other.id);
    //     return *this;
    // }

    // ImageId& operator=(const ImageId&& other) {
    //     id = std::move(other.id);
    //     return *this;
    // }

    // ImageId& operator=(const char&& other) {
    //     id = other;
    //     return *this;
    // }

    // ImageId& operator=(const std::string&& other) {
    //     id = std::move(other);
    //     return *this;
    // }

    // ImageId& operator=(const char*&& other) {
    //     id = std::move(other);
    //     return *this;
    // }

    // ImageId& operator=(const ImageId* other) {
    //     id = other->id;
    //     return *this;
    // }

    // ImageId& operator=(const ImageId*&& other) {
    //     id = std::move(other->id);
    //     return *this;
    // }

    // ImageId& operator=(const ImageId* const other) {
    //     id = other->id;
    //     return *this;
    // }

    // ImageId& operator=(const ImageId* const&& other) {
    //     id = std::move(other->id);
    //     return *this;
    // }

    friend std::ostream& operator<<(std::ostream& os, const ImageId& id) {
        os << id.id;
        return os;
    }


};