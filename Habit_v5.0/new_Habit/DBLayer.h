#ifndef DBLAYER_H
#define DBLAYER_H

#include <vector>
#include "graphics.h"// For MOUSEMSG
#include "Habit.h"
#include "Event.h"
#include "DateRecord.h"
#include"sqlite3/sqlite3.h"

class DBLayer
{
public:
    /*
    * 负责人： Rain
    * 功能：
    *   创建习惯数据表，用于存储用户的习惯数据。
    *   表头：
    *       |习惯ID (int)|名称 (string)|每日打卡次数 (int)|开始日期 (Date)|结束日期 (Date)|是否活跃 (bool)|软删除 (bool)|
    *   逻辑：
    *       1. 如果数据库中不存在该表，则创建表结构
    *       2. 确保习惯ID是主键且自增
    * 参数：void
    * 返回值：void
    */
    static void createHabitDb(void);

    /*
    * 负责人： Rain
    * 功能：
    *   查询数据库，获取所有未被标记为删除的习惯记录。
    *   逻辑：
    *       1. 执行 SQL 语句，查询 HabitTable
    *       2. 过滤掉软删除字段（deleted = 1）的记录
    *       3. 将查询结果转换为 Habit 对象列表
    * 参数：void
    * 返回值：
    *   vector<Habit> - 所有未删除的习惯对象的列表
    */
    std::vector<Habit> getHabit();

    /*
    * 负责人： Rain
    * 功能：
    *   将 Habit 对象的数据插入到数据库的习惯表中。
    *   逻辑：
    *       1. 计算数据库中的 Habit ID 最大值，并生成新 ID
    *       2. 插入新的 Habit 记录到 HabitTable
    * 参数：
    *   habit - 需要插入数据库的 Habit 对象，包含名称、开始日期、结束日期、每日打卡次数
    * 返回值：void
    * 伪代码：
    */
    void insertHabitDb(Habit habit);

    /*
    * 负责人： Rain
    * 功能：
    *   根据 Habit 对象的 ID，在数据库中更新对应的习惯记录。
    *   逻辑：
    *       1. 通过 Habit ID 定位习惯记录
    *       2. 更新习惯的相关字段
    * 参数：
    *   habit - 需要修改的 Habit 对象
    * 返回值：void
    */
    void updateHabitDb(Habit habit);

    /*
    * 负责人： Rain
    * 功能：
    *   软删除 Habit 记录，仅修改删除标记字段，而不真正删除数据。
    * 参数：
    *   habitID - 需要删除的习惯记录的 ID
    * 返回值：void
    */
    void deleteHabitDb(int habitID);

    /*
    * 负责人： 阿浪
    * 功能：
    *   创建事项数据表
    *   表头：
    *       |事项ID (int)|名称 (string)|日期 (Date)|时间 (Time)|提醒 (bool)|软删除 (bool)|
    * 参数：void
    * 返回值：void
    */
    void createEventDb();

    /*
    * 负责人： 阿浪
    * 功能：
    *   查询数据库，获取所有未被标记为删除的事项记录。
    * 参数：void
    * 返回值：
    *   vector<Event> - 所有未删除的事项对象的列表
    */
    std::vector<Event> getEvent();

    /*
    * 负责人： 阿浪
    * 功能：
    *   将 Event 对象的数据插入到数据库的事项表中。
    * 参数：
    *   event - 需要插入数据库的 Event 对象
    * 返回值：void
    */
    void insertEventDb(Event event);

    /*
    * 负责人： 阿浪
    * 功能：
    *   更新数据库中的事项记录
    * 参数：
    *   event - 修改后的 Event 对象
    * 返回值：void
    */
    void updateEventDb(Event event);

    /*
    * 负责人： 阿浪
    * 功能：
    *   软删除事项记录
    * 参数：
    *   eventID - 需要删除的事项记录的 ID
    * 返回值：void
    */
    void deleteEventDb(int eventID);

    /*
    * 负责人： Dearing
    * 功能：
    *   创建番茄钟数据表
    * 参数：void
    * 返回值：void
    */
    void createPomodoroDb();

    /*
    * 负责人： Dearing
    * 功能：
    *   创建每日记录表 DateRecord
    * 参数：void
    * 返回值：void
    */
    void createDateRecordDb();

    /*
    * 负责人： Dearing
    * 功能：
    *   在每日记录表中插入习惯打卡记录
    * 参数：
    *   habit - 进行打卡的 Habit 对象
    * 返回值：void
    */
    void insertHabitRecord(Habit habit);

    /*
    * 负责人： Dearing
    * 功能：
    *   在番茄钟表中创建新的番茄钟记录
    * 参数：
    *   pomo - 新建的番茄钟记录对象
    * 返回值：void
    */
    void insertPomoRecord(Pomodoro pomo);

    /*
    * 负责人： Dearing
    * 功能：
    *   查询数据库，获取指定日期的所有记录
    * 参数：
    *   date - 指定查询的日期
    * 返回值：
    *   DateRecord - 该日期的所有记录
    */
    DateRecord getRecordbyDate(Date date);

    /*
    * 负责人： Rain
    * 功能：
    *   获取数据库中当前最大的 Habit ID
    * 返回值：
    *   int - 数据库中的最大 Habit ID
    */
    int getHabitIDMax();

    /*
    * 负责人： Rain
    * 功能：
    *   获取数据库中当前最大的 Event ID
    * 返回值：
    *   int - 数据库中的最大 Event ID
    */
    int getEventIDMax();


    /*
    * 负责人： Rain
    * 功能：
    *   获取数据库中当前最大的 Pomo ID
    * 返回值：
    *   int - 数据库中的最大 Pomo ID
*/
    int getPomoIDMax();


    /*
    * 负责人： 十七
    * 功能：
    *   获取当前鼠标事件信息。
    *   用于 GUI 交互检测鼠标点击事件。
    * 参数：void
    * 返回值：
    *   MOUSEMSG - 包含鼠标事件的详细信息。
    */
    MOUSEMSG getmouse();


    void handleSqliteError(int rc, sqlite3* db);
};

#endif // DBLAYER_H
