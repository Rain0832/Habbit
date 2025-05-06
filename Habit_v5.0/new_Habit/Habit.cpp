//
// Created by Lenovo on 25-3-13.
//
#include "Habit.h"

// 构造函数实现
Habit::Habit(int id, int user_id, std::string name, int target_count,
             Date start_date, Date end_date, bool is_active, bool is_deleted)
    : id(id), user_id(user_id), name(name), target_count(target_count),
      start_date(start_date), end_date(end_date), is_active(is_active),
      is_deleted(is_deleted) {}
