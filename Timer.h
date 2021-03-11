#pragma once

#include "stdHeaders.h"
#include "Event.h"

namespace Snake {
    class Timer {
    private:
        bool timeout;
        bool counting;
        int seconds;
        mutex mux;
        condition_variable cv;
    public:
        Timer(int s);
        ~Timer();
        void Start();
        void Stop();
        bool IsCounting();
        void Reset(int s);
        int GetRemainingTime();
        bool TimeOut();
        void TimeOutEvent();
        Event<> OnTimeOutEvent;
    };
}