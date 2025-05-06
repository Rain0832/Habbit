#ifndef SERVICELAYER_H
#define SERVICELAYER_H

#include <vector>
#include <string>
#include "Habit.h"
#include "Event.h"
#include "Pomodoro.h"
#include "DateRecord.h"
#include "DBLayer.h"
#include "sqlite3/sqlite3.h"

class ServiceLayer
{

public:
    DBLayer db_layer;
    /*
    负责人：十七
    功能：
        检测鼠标是否与该矩形框内的区域发生碰撞
        如是则返回true
    参数：
        x: 左上角x坐标
        y: 左上角y坐标
        w: 宽度
        h：高度
    */
    bool IfMouseHit(int x, int y, int w, int h, MOUSEMSG msg);

    /*
  负责人:冰柠、十七
    功能:
        还需要调用getHabitIDMax()找到应该插入的主键
        验证习惯创建界面的用户输入数据是否合法
        例如日期不是一个合法区间等等
        不合法则返回false
        合法将数据封装为Habit对象并传递给数据层
    参数:
        name          - 习惯名称
        start_date    - 习惯开始日期
        end_date      - 习惯结束日期
        times_per_day - 每日打卡次数

    返回值: bool 表示输入是否有效
    */
    bool insertHabit(std::string, Date, Date, int);

    /*
    负责人:冰柠、十七
    功能
        验证习惯修改界面的用户输入数据是否合法
        不合法则返回false
        合法将数据封装为Habit对象并传递给数据层
    参数:
        habitID       - 要修改的习惯ID
        beginDate     - 修改后的开始日期
        endDate       - 修改后的结束日期
        times_per_day - 修改后的每日打卡次数
        active_flag      - 是否启用该习惯
    返回值: bool 表示修改输入是否有效


    */
    bool updateHabit(int HabitID, Date, Date, int, bool);

    /*
    负责人:冰柠、十七
    功能:
        删除习惯，调用数据层的删除函数
    参数: habitID 要删除的习惯ID
    返回值: bool 表示删除是否成功


    */
    bool deleteHabit(int HabitID);

    /*
    负责人:冰柠、十七
    功能:
    还需要调用getEventIDMax()找到应该插入的主键
    验证事项创建界面的用户输入数据是否合法
    不合法则返回false
    合法将数据封装为Event对象并传递给数据层
    参数:
    name         - 事项名称
    event_date   - 事项日期
    event_time   - 事项时间
    remind_flag  - 是否开启提醒
    remind_time  - 提前提醒的时间
    返回值: bool 表示输入是否有效
    */
    bool insertEvent(std::string, Date, Time, bool, Time);

    /*
    负责人:冰柠、十七
    功能:
        验证事项修改界面的用户输入数据是否合法
        不合法则返回false
        合法将数据封装为Event对象并传递给数据层
    参数:
        eventID     - 要修改的事项ID
        event_date  - 修改后的事项日期
        event_time  - 修改后的事项时间
        remind_flag - 是否开启提醒
        remind_time - 修改后的提醒时间
    返回值: bool 表示修改输入是否有效
    */
    bool updateEvent(int EventID, std::string, Date, Time, bool, Time);

    /*
    负责人:冰柠、十七
    功能:
        删除事项，调用数据层的删除函数
    参数: eventID 要删除的事项ID
    返回值: bool 表示删除是否成功
    */
    bool deleteEvent(int EventID);

    /*
    负责人: Dearing
    功能:
        习惯打卡，如果能够成功打卡则调用数据层函数插入到每日记录中
        调用insertHabitRecord(Habit)插入对应的打卡记录表
    参数: habit 打卡的习惯对象
    返回值: bool 表示打卡是否成功
    */
    bool checkinHabit(Habit); // 习惯打卡

    /*
    负责人：阿浪
    功能：
        通过 getHabit()获取习惯列表
        获取习惯列表中活跃字段为1的
    参数：无
    返回值：vector<Habit>
    */
    std::vector<Habit> getActiveHabit();

    /*
    负责人：阿浪
    功能：
        通过 getHabit()获取习惯列表
        获取习惯列表中活跃字段为0的
    参数：无
    返回值：vector<Habit>
    */
    std::vector<Habit> getInactiveHabit();

    /*
    负责人：阿浪
    功能：
        通过 getEvent()获取事项列表
        获取事项列表中过期字段为0的
    参数：无
    返回值：vector<Event>
    */
    std::vector<Event> getActiveEvent();

    /*
    负责人：阿浪
    功能：
        通过 getEvent()获取事项列表
        获取事项列表中过期字段为1的
    参数：无
    返回值：vector<Event>
    */
    std::vector<Event> getExpiredEvent();

    /*
     负责人：Rain

     这个功能用来判断番茄钟是否到时，通过传入的Pomodoro对象中的开始时间和倒计时时间相加得到番茄钟结束时间
     时间未到前返回true
     到达后返回false
    并把对应的番茄钟记录插入数据库insertPomoRecord(Pomodoro)函数
    
     参数：番茄钟结构体
     返回值：bool
   
     */
    bool pomodoroTick(Pomodoro, Time);


    /*
    负责人：Rain

    这个功能获取某个月每一天打卡的习惯的名称和次数
    这里的参数Date对象是需要使用到其中的Year和Month部分
    调用DateRecord getRecordbyDate(Date)函数

    参数：Date    某个日期
    返回值：std::vector<std::pair<int, int>>     某个月每天打卡的实际打卡数和应当打卡数
    */
    std::vector<std::pair<int, int>> getHabitRecordbyDate(Date);

    /*
     负责人：Rain

    这个功能用来获取对应日期的记录
    调用DateRecord getRecordbyDate(Date)函数

     参数：Date    某个日期
     返回值：DateRecord     对应日期的记录

     */
    DateRecord getAllRecordbyDate(Date);

    std::pair<Date, Time> getCurrentTime() const;

    Habit getHabitByID(int habit_id);

    Event getEventByID(int event_id);


    void init();

    
};

#endif // SERVICELAYER
