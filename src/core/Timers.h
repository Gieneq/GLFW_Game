#pragma once
#include <vector>

class TimerListener {
public:
    virtual void onTimerTick() = 0;
};

class Timer {
public:
    Timer(int intervalMs, int minTick, int maxTicks) : intervalMs(intervalMs), minTick(minTick), maxTicks(maxTicks) {}
    void start() {
        running = true;
        recentTick = minTick;
    }

    void stop() {
        running = false;
    }

    void update(float dtMs) {
        if(!running) {
            return;
        }

        intervalMs += dtMs;
        if(recentInterval >= intervalMs) {
            recentInterval -= intervalMs;
            onTick();
        }
    }

    void onTick() {
        /* Emit signal to all listeners */
        for(auto listener : listeners) {
            listener->onTimerTick();
        }

        /* Update */
        ++recentTick;
        if(recentTick >= maxTicks) {
            recentTick = minTick;
        }
    }

    void addListener(TimerListener* listener) {
        listeners.push_back(listener);
    }

private:
    bool running{false};
    int intervalMs{0};
    int recentInterval{0};
    int recentTick{0};
    int minTick{0};
    int maxTicks{2};
    std::vector<TimerListener*> listeners;
};