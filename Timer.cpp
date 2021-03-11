#include "Timer.h"

namespace Snake {
    Timer::Timer(int s) {
        Reset(s);
    }

    Timer::~Timer() {
    }

    void Timer::Start() {
        counting = true;
        timeout = false;
        thread t([&]() {
            while(counting && !timeout) {
                unique_lock<mutex> lock(mux);
                cv.wait_for(lock, chrono::seconds(1));
                if(counting)
                    seconds--;
                if(counting && seconds == 0) {
                    timeout = true;
                    TimeOutEvent();
                }
            }
        });
        t.detach();
    }

    void Timer::Stop() {
        counting = false;
        timeout = false;
        cv.notify_one();
    }

    bool Timer::IsCounting() {
        return counting;
    }

    void Timer::Reset(int s) {
        seconds = s;
        timeout = false;
        counting = false;
        cv.notify_one();
    }

    int Timer::GetRemainingTime() {
        if(timeout)
            return 0;
        return seconds;
    }

    bool Timer::TimeOut() {
        return timeout;
    }

    void Timer::TimeOutEvent() {
        OnTimeOutEvent();
    }
}