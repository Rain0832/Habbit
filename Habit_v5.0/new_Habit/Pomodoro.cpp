//
// Created by Lenovo on 25-3-13.
//
#include "Pomodoro.h"

// 构造函数实现
Pomodoro::Pomodoro(int _id,Time pomodoro_time,  std::string record)
    : id(_id), pomodoro_time(pomodoro_time),  record(record) {}
