#pragma once
#include <vector>
#include <string>

class Core {
public:
    Core() = default;
    virtual ~Core() = default;
    virtual bool init();
    void loop();

    virtual bool input() {
        return false;
    }
    virtual void update(float dt) {}
    virtual void render() {}
    double getFPS() const;
    double getTime() const;
    void appendLogMessage(std::string && str);

private:
    void logState();
    // double loopStartTime{0.0};
    // double loopStopTime{0.0};
    // double loopUsagePercentage{0.0};
    double lastTime{0.0};
    double FPSAccumulatedTime{0.0};
    double logAccumulatedTime{0.0};
    int frameCount{0};
    double calculatedFPS{0.0};
    std::vector<std::string> logMessages;
};