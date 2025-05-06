#ifndef HABIT_H
#define HABIT_H

#include <string>
#include "Times.h"
struct Habit
{
    int id;     //习惯id
    int user_id;    //用户id
    std::string name;       //习惯名称
    int target_count;       //每日打卡数
    Date start_date;        //开始日期
    Date end_date;          //结束日期
    bool is_active;     //是否启用
    bool is_deleted;        //是否删除

    explicit Habit(int id = 0, int user_id = 0, std::string name = "", int target_count = 0,
        Date start_date = Date(), Date end_date = Date(), bool is_active = false,
        bool is_deleted = false);
};

#endif // HABIT_H