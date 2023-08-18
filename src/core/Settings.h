#pragma once
#include <string>
#include "Maths.h"

namespace Settings {
    struct Window {
        static constexpr char* TITLE = "Some title";
        static constexpr int WIDTH{800}; //1200
        static constexpr int HEIGHT{600}; //800
        static constexpr float ASPECT_RATIO{static_cast<float>(WIDTH) / static_cast<float>(HEIGHT)};
        static constexpr bool ENABLE_VSYNC{true};

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
        static constexpr float ZOOM = 0.125F; //*4
    };

    struct Resources {
        static constexpr char* ASSETS_PATH  = "../res/assets/";
        static constexpr char* DATA_PATH  = "../res/data/";
        static constexpr char* MAPS_DIR  = "maps/";
    };

    struct Player {
        static constexpr float INITIAL_SPEED = 4.5F;
    };
    
    struct General {
        static constexpr double FPS_MEASURE_INTERVAL{0.5};
        static constexpr double LOG_STATE_INTERVAL{1.0};
        static constexpr bool LOG_STATE{true};
        struct Version {
            static constexpr int MAJOR{1};
            static constexpr int MINOR{0};
            static constexpr int PATCH{0};
            static inline const std::string to_string() {
                return std::to_string(MAJOR) + "." + std::to_string(MINOR) + "." + std::to_string(PATCH);
            }
        };
    };

    struct Rendering {
        struct Brightness {
            static constexpr float FRONT = 1.0F;
            static constexpr float BOTTOM = 0.8F;
            static constexpr float BACK = 0.4F;
            static constexpr float TOP = 0.2F;
        };
    };
}