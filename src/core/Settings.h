#pragma once
#include <string>

namespace Settings {
    struct Window
    {
        static constexpr char* TITLE = "Some title";
        static constexpr int WIDTH{800};
        static constexpr int HEIGHT{600};
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