//
// Created by Lenovo on 25-3-13.
//
#include "DateRecord.h"

DateRecord::DateRecord(const std::vector<std::pair<Time, Habit>>& _HabitRecord,
    const std::vector<std::pair<Time, Pomodoro>>& _PomodoroRecord)
:    Habit_Record(_HabitRecord), Pomodoro_Record(_PomodoroRecord)
{}

