//
// Created by Lenovo on 25-3-13.
//
#include "Event.h"

// 构造函数实现
Event::Event(int id, int user_id, std::string title, Date event_date,
             Time event_time, bool remind_flag, Time remind_time,
             bool is_expired_flag, bool is_deleted)
    : id(id), user_id(user_id), title(title), event_date(event_date),
      event_time(event_time), remind_flag(remind_flag), remind_time(remind_time),
      is_expired_flag(is_expired_flag), is_deleted(is_deleted) {}