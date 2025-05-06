#ifndef DATERECORD_H
#define DATERECORD_H

#include <unordered_map>
#include<vector>
#include "Times.h"
#include "Habit.h"
#include "Pomodoro.h"


struct DateRecord
{
    // 打卡时间-习惯
    std::vector<std::pair<Time, Habit>> Habit_Record;
    
    // 开始使用的时间-番茄钟
    std::vector<std::pair<Time, Pomodoro>> Pomodoro_Record;
   

    DateRecord() = default;


    explicit DateRecord(const std::vector<std::pair<Time, Habit>>&,
        const std::vector<std::pair<Time, Pomodoro>>&);
};

#endif // DATERECORD_H