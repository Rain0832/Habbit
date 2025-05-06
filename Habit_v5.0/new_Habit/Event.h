#ifndef EVENT_H
#define EVENT_H

#include <string>
#include "Times.h"

struct Event
{
    int id;     //id，对应数据库中的id字段，是一个索引
    int user_id;            //用户名称
    std::string title;      //事项名称
    Date event_date;        //事项到期的时间
    Time event_time;        //事项到期
    bool remind_flag;     // 是否提醒？
    Time remind_time;     // 提醒的时间
    bool is_expired_flag; // 过期标记（默认 false）
    bool is_deleted;      // 软删除标记（默认 false）

    // 构造函数
    explicit Event(int id = 0, int user_id = 0, std::string title = "", Date event_date = Date(),
        Time event_time = Time(), bool remind_flag = false, Time remind_time = Time(),
        bool is_expired_flag = false, bool is_deleted = false);
};

#endif // EVENT_H