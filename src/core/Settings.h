#pragma once
#include <string>
#include "Maths.h"

namespace Settings {
    struct Window {
        static constexpr char* TITLE = "Some title";
        static constexpr int WIDTH{1400}; // (1400 - record) (800 - photo)
        static constexpr int HEIGHT{900}; // (900  - record) (600 - photo)
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
        // static constexpr int TILE_SIZE{32};
        static constexpr int TILE_DEPTH{32}; //20
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
        static constexpr char* ASSETS_PATH  = "../../res/assets/";
        static constexpr char* DATA_PATH  = "../../res/data/";
        static constexpr char* MAPS_DIR  = "maps/";
        static constexpr char* RES_ROOT_DIRPATH  = "../../res/";
        static constexpr char* WORLD_DIRPATH  = "data/generated/";
        static constexpr char* WORLD_FILENAME  = "world.json";
        static constexpr char* GENERATED_DIRPATH  = "../../res/data/generated/";
        static constexpr char* CHUNKS_DIRPATH  = "../../res/data/generated/chunks/";
        static constexpr bool USE_VERTICAL_SHIFT{false};
    };

    struct Player {
        static constexpr float INITIAL_SPEED = 4.5F;
        static constexpr float INITIAL_X = 37.0F;
        static constexpr float INITIAL_Y = 12.0F;
        static constexpr int INITIAL_ELEVATION_IDX = 0;
        static constexpr float INITIAL_Z = 0.0F;
    };

    struct Debug {
        static constexpr int NONE = 0;
        static constexpr int SOME = 1;
        static constexpr int MORE = 2;
        static constexpr int ALL = 3;
        static constexpr int VERBOSE_LEVEL = SOME;
        static constexpr bool VERBOSE_IS_NONE = VERBOSE_LEVEL <= NONE;
        static constexpr bool VERBOSE_IS_SOME = VERBOSE_LEVEL >= SOME;
        static constexpr bool VERBOSE_IS_MORE = VERBOSE_LEVEL >= MORE;
        static constexpr bool VERBOSE_IS_ALL = VERBOSE_LEVEL >= ALL;
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
        struct Bleeding {
            static constexpr int MARGIN = 1;
            static constexpr int SPACING = 2;
        };
        static constexpr bool USE_Y_PERSPECTIVE{true};
        static constexpr float Y_PERSPECTIVE_SHIFT{1.0F};
    };
    
    struct Systems {
        struct Collisions {
            static constexpr float MAX_WALKABLE_DEPTH{0.5F};
            static constexpr int STAIRS_STEPS_COUNT{2};
        };
    };
}