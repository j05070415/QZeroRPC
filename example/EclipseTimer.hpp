#ifndef ECLIPSETIMER_HPP
#define ECLIPSETIMER_HPP

#include <chrono>
class EclipseTimer
{
public:
    EclipseTimer() {
        reset();
    }
    void reset() {
        _time = std::chrono::steady_clock::now();
    }
    long eclipsed() const {
        auto current = std::chrono::steady_clock::now();
        auto dur = current - _time;
        return dur.count()/1000;
    }

private:
    std::chrono::steady_clock::time_point _time;
};

#endif
