#ifndef POMODORO_H
#define POMODORO_H

#include <string>
#include "Times.h"

struct Pomodoro
{
    int id;
    Time pomodoro_time;         // 番茄钟时间
    std::string record;         // 番茄钟记录

    Pomodoro(int id = 0, Time pomodoro_time = Time(), std::string record = "");
};

#endif // POMODORO_H