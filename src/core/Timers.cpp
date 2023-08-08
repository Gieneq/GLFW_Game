#include "Timers.h"
#include <iostream>

void Timer::update(int dtMs) {
    if(!running) {
        return;
    }

    recentInterval += dtMs;
    if(recentInterval >= intervalMs) {
        recentInterval -= intervalMs;
        onTick();
    }
}

void Timer::onTick() {
    /* Emit signal to all listeners */
    for(auto listener : listeners) {
        listener->onTimerTick(recentTick);
    }

    /* Update */
    ++recentTick;
    if(recentTick >= maxTicks) {
        recentTick = minTick;
    }
}

void Timer::addListener(TimerListener* listener) {
    listeners.push_back(listener);
}

void Timer::removeListener(TimerListener* listener) {
    for(auto it = listeners.begin(); it != listeners.end(); ++it) {
        if(*it == listener) {
            listeners.erase(it);
            return;
        }
    }
}

void TimersRegister::update(int dtMs) {
    for(auto& timer : timers) {
        timer.update(dtMs);
    }
}

Timer& TimersRegister::getTimer(int interval, int minTick, int maxTicks) {
    /* Find timer, if not create one */
    for(auto& timer : timers) {
        if(timer.intervalMs == interval && timer.minTick == minTick && timer.maxTicks == maxTicks) {
            return timer;
        }
    }

    /* Not found, create new timer */
    timers.push_back(Timer(interval, minTick, maxTicks));
    return timers.back();
}
