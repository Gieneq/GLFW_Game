#pragma once

class Core {
public:
    virtual ~Core() = default;
    virtual void init();
    void loop();

    virtual bool input() {
        return false;
    }
    virtual void update(float dt) {}
    virtual void render() {}
    double getFPS() const;

private:
    double lastTime{0.0};
    double accumulatedTime{0.0};
    int frameCount{0};
    double calculatedFPS{0.0};
};