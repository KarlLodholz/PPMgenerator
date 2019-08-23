#ifndef TIMER_H
#define TIMER_H
#include <iostream>
#include <chrono>
#include <array>

//this isn't my code.  All credit goes to TheChernoProject.
//link to his channel: https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw

class Timer {
public:
    Timer() {
        m_timeStart = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        stop();
    }

    void stop() {
        auto timeEnd = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_timeStart).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(timeEnd).time_since_epoch().count();
        auto time = end - start;
        double ms = time * 0.001;
        std::cout<<time<<"us ("<<ms<<"ms)"<<std::endl;
    };
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_timeStart;
};

#endif