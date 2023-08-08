#pragma once
#include <vector>

class TimerListener {
public:
    virtual void onTimerTick(const int recentTick) = 0;
};

class Timer {
public:
    Timer() = default;
    Timer(int intervalMs, int minTick, int maxTicks) : intervalMs(intervalMs), minTick(minTick), maxTicks(maxTicks) {}
    
    void start() {
        running = true;
        recentTick = minTick;
    }

    void stop() {
        running = false;
    }

    void update(int dtMs);
    void onTick();

    void addListener(TimerListener* listener);
    void removeListener(TimerListener* listener);

private:
    friend class TimersRegister;
    bool running{true};
    int intervalMs{1000};
    int recentInterval{0};
    int recentTick{0};
    int minTick{0};
    int maxTicks{1};
    std::vector<TimerListener*> listeners;
};

class TimersRegister {
public:
    TimersRegister() = default;
    static TimersRegister& getTimersRegister() {
        static TimersRegister timersRegister;
        return timersRegister;
    }

    void addTimer(const Timer& timer) {
        timers.push_back(timer);
    }

    void update(int dtMs);
    Timer& getTimer(int interval, int minTick, int maxTicks);

private:
    std::vector<Timer> timers;
};