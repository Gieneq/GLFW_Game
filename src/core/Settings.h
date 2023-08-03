#pragma once
#include <string>
#include "Maths.h"

namespace Settings {
    struct Window
    {
        static constexpr char* TITLE = "Some title";
        static constexpr int WIDTH{800};
        static constexpr int HEIGHT{600};
        static constexpr float ASPECT_RATIO{static_cast<float>(WIDTH) / static_cast<float>(HEIGHT)};
        static constexpr bool ENABLE_VSYNC{false};

        struct ClearColor {
            static constexpr float r{0.0};
            static constexpr float g{0.0};
            static constexpr float b{1.0};
            static constexpr float a{1.0};
        };
    };

    struct Map {
        static constexpr int TILE_SIZE{20};
        static constexpr int WIDTH{32};
        static constexpr int HEIGHT{16};
        static constexpr int ORIGIN_X{0};
        static constexpr int ORIGIN_Y{0};
        static constexpr int MAX_GID{(2<<16)-1};
    };

    struct Camera {
        static constexpr float ZOOM = 0.25f;
    };

    struct Resources {
        static constexpr char* ASSETS_PATH  = "res/assets/";
    };
    
    struct General {
        static constexpr double FPS_MEASURING_INTERVAL{1.0};
        struct Version
        {
            static constexpr int MAJOR{1};
            static constexpr int MINOR{0};
            static constexpr int PATCH{0};
            static inline const std::string to_string() {
                return std::to_string(MAJOR) + "." + std::to_string(MINOR) + "." + std::to_string(PATCH);
            }
        };
        
    };
}