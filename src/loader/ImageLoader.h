#pragma once
#include "lodepng.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "ImageData.h"


class ImageLoader {
private:
    ImageLoader() = default;
    ~ImageLoader() = default;
    // ImageLoader(const ImageLoader&) = delete;
    // ImageLoader& operator=(const ImageLoader&) = delete;
    // ImageLoader(ImageLoader&&) = delete;
    // ImageLoader& operator=(ImageLoader&&) = delete;
public:
    static ImageLoader& get_loader();
    bool load_image(ImageId key_name, std::string relative_path);
    bool has_image(ImageId key_name);
    ImageData& get_image(ImageId key_name);
private:
    std::unordered_map<ImageId, ImageData, ImageId> images{};
};