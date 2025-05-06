#include "DBLayer.h"
#include<iostream>
#include <format>
#include <chrono>

void DBLayer::createHabitDb() {
    // 打开数据库
    sqlite3* db = NULL;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);

    // SQL
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Habits ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "UserID INTEGER, "
        "Name TEXT, "
        "TargetCount INTEGER, "
        "StartDate TEXT, "
        "EndDate TEXT, "
        "IsActive INTEGER, "
        "IsDeleted INTEGER);", NULL, NULL, NULL);

    // 关闭数据库
    sqlite3_close(db);
}

// 将 UTF-8 字符串转换为 GBK 字符串
std::string UTF8ToGBK(const std::string& utf8Str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, wstr, len);

    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    char* gbkStr = new char[len + 1];
    memset(gbkStr, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, gbkStr, len, nullptr, nullptr);

    std::string result(gbkStr);
    delete[] wstr;
    delete[] gbkStr;
    return result;
}
// getHabit_callback 回调函数
static int getHabit_callback(void* data, int argc, char** argv, char** azColName) {
    std::vector<Habit>* habitList = static_cast<std::vector<Habit>*>(data);
    int ID = std::stoi(argv[0]);
    int UserID = std::stoi(argv[1]);
    std::string Name = argv[2] ? argv[2] : "";
    Name = UTF8ToGBK(Name);  // 进行编码转换
    int TargetCount = std::stoi(argv[3]);
    std::string startDateStr = argv[4] ? argv[4] : "";
    std::string endDateStr = argv[5] ? argv[5] : "";
    bool IsActive = std::stoi(argv[6]) != 0;
    bool IsDeleted = std::stoi(argv[7]) != 0;

    Date startDate = Date(startDateStr);
    Date endDate = Date(endDateStr);

    habitList->emplace_back(ID, UserID, Name, TargetCount, startDate, endDate, IsActive, IsDeleted);
    return 0;
}

std::vector<Habit> DBLayer::getHabit() {
    // 打开数据库
    sqlite3* db = NULL;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);
    std::vector<Habit> habitList;
    char* errMsg;
    rc = sqlite3_exec(db, "PRAGMA encoding;", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to execute PRAGMA encoding: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    // SQL 查询
    rc = sqlite3_exec(db, "SELECT * FROM Habits WHERE IsDeleted = 0;", getHabit_callback, &habitList, &errMsg);

    // 关闭数据库
    sqlite3_close(db);
    return habitList;
}

void DBLayer::insertHabitDb(Habit habit) {
    // 打开数据库
    sqlite3* db = nullptr;
    int rc = sqlite3_open("Habbit.db", &db);

    // id 生成
    int newId = getHabitIDMax() + 1;
    habit.id = newId;

    // SQL 语句
    // 格式化日期
    std::string sqlInsert = std::format("INSERT INTO Habits (ID, UserID, Name, TargetCount, StartDate, EndDate, IsActive, IsDeleted) "
        "VALUES ({}, {}, '{}', {}, '{:04d}-{:02d}-{:02d}', '{:04d}-{:02d}-{:02d}', {}, {});",
        habit.id, habit.user_id, habit.name, habit.target_count,
        habit.start_date.year, habit.start_date.month, habit.start_date.day,
        habit.end_date.year, habit.end_date.month, habit.end_date.day,
        habit.is_active ? 1 : 0, habit.is_deleted ? 1 : 0);

    rc = sqlite3_exec(db, sqlInsert.c_str(), nullptr, nullptr, nullptr);

    // 关闭数据库
    sqlite3_close(db);
}

std::string toUTF8(const std::string& str) {
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);

    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    char* utf8Str = new char[len + 1];
    memset(utf8Str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8Str, len, nullptr, nullptr);

    std::string result(utf8Str);
    delete[] wstr;
    delete[] utf8Str;
    return result;
}

void DBLayer::updateHabitDb(Habit habit) {
    // 打开数据库
    sqlite3* db;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);

    // name 设置为 UTF-8 编码
    habit.name = toUTF8(habit.name);

    // SQL
    std::string sqlUpdate = "UPDATE Habits SET "
        "ID = " + std::to_string(habit.id) + ", "
        "UserID = " + std::to_string(habit.user_id) + ", "
        "Name = '" + habit.name + "', "
        "TargetCount = " + std::to_string(habit.target_count) + ", "
        "StartDate = '" + std::to_string(habit.start_date.year) + "-"
        + std::to_string(habit.start_date.month) + "-"
        + std::to_string(habit.start_date.day) + "', "
        "EndDate = '" + std::to_string(habit.end_date.year) + "-"
        + std::to_string(habit.end_date.month) + "-"
        + std::to_string(habit.end_date.day) + "', "
        "IsActive = " + (habit.is_active ? "1" : "0") + ", "
        "IsDeleted = " + (habit.is_deleted ? "1" : "0") + " "
        "WHERE ID = " + std::to_string(habit.id) + ";";

    rc = sqlite3_exec(db, sqlUpdate.c_str(), nullptr, nullptr, nullptr);

    // 关闭数据库
    sqlite3_close(db);
}

void DBLayer::deleteHabitDb(int habitId) 
{
    // 打开数据库
    sqlite3* db;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);

    // SQL
    std::string sqlDelete = "UPDATE Habits SET IsDeleted = 1 WHERE ID = " + std::to_string(habitId) + ";";
    rc = sqlite3_exec(db, sqlDelete.c_str(), NULL, NULL, NULL);

    // 关闭数据库
    sqlite3_close(db);
    return;
}


void DBLayer::createEventDb()
{
    sqlite3* db;
    int rc = sqlite3_open("Habbit.db", &db);
    handleSqliteError(rc, db);

    std::string sql = "CREATE TABLE IF NOT EXISTS Events ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER,"
        "title TEXT,"
        "event_date_year INTEGER,"
        "event_date_month INTEGER,"
        "event_date_day INTEGER,"
        "event_time_hour INTEGER,"
        "event_time_minute INTEGER,"
        "event_time_second INTEGER,"
        "remind_flag INTEGER,"
        "remind_time_hour INTEGER,"
        "remind_time_minute INTEGER,"
        "remind_time_second INTEGER,"
        "is_expired_flag INTEGER,"
        "is_deleted INTEGER"
        ");";

    char* errMsg = nullptr;
    rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        exit(1);
    }
    else {
        std::cout << "Table created successfully!" << std::endl;
    }

    sqlite3_close(db);
}


std::vector<Event> DBLayer::getEvent()
{
    std::vector<Event> Events;
    sqlite3* db;
    int rc = sqlite3_open("Habbit.db", &db);
    handleSqliteError(rc, db);

    std::string sql = "SELECT id, user_id, title, event_date_year, event_date_month, event_date_day, "
        "event_time_hour, event_time_minute, event_time_second, remind_flag, "
        "remind_time_hour, remind_time_minute, remind_time_second, "
        "is_expired_flag, is_deleted FROM Events;";

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    handleSqliteError(rc, db);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int user_id = sqlite3_column_int(stmt, 1);
        std::string title(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        title = UTF8ToGBK(title);   // 进行编码转换

        int event_date_year = sqlite3_column_int(stmt, 3);
        int event_date_month = sqlite3_column_int(stmt, 4);
        int event_date_day = sqlite3_column_int(stmt, 5);
        Date event_date(event_date_year, event_date_month, event_date_day);

        int event_time_hour = sqlite3_column_int(stmt, 6);
        int event_time_minute = sqlite3_column_int(stmt, 7);
        int event_time_second = sqlite3_column_int(stmt, 8);
        Time event_time(event_time_hour, event_time_minute, event_time_second);

        bool remind_flag = static_cast<bool>(sqlite3_column_int(stmt, 9));

        int remind_time_hour = sqlite3_column_int(stmt, 10);
        int remind_time_minute = sqlite3_column_int(stmt, 11);
        int remind_time_second = sqlite3_column_int(stmt, 12);
        Time remind_time(remind_time_hour, remind_time_minute, remind_time_second);

        bool is_expired_flag = static_cast<bool>(sqlite3_column_int(stmt, 13));
        bool is_deleted = static_cast<bool>(sqlite3_column_int(stmt, 14));

        Events.emplace_back(id, user_id, title, event_date, event_time, remind_flag, remind_time, is_expired_flag, is_deleted);
    }

    if (rc != SQLITE_DONE) {
        handleSqliteError(rc, db);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return Events;
}

// 格式化整数为带前导零的字符串
std::string formatNumber(int num) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << num;
    return oss.str();
}

void DBLayer::insertEventDb(Event event) {
    // 打开数据库
    sqlite3* db = nullptr;
    int rc = sqlite3_open("Habbit.db", &db);
    //handleSqliteError(rc, db) ;

    // id 生成
    int newId = getEventIDMax() + 1;
    event.id = newId;

    // SQL
    std::string sqlInsert = "INSERT INTO Events (id, user_id, title, event_date_year, event_date_month, event_date_day, "
        "event_time_hour, event_time_minute, event_time_second, remind_flag, "
        "remind_time_hour, remind_time_minute, remind_time_second, "
        "is_expired_flag, is_deleted) "
        "VALUES (" + std::to_string(event.id) + ", " + std::to_string(event.user_id) + ", '"
        + event.title + "', " + std::to_string(event.event_date.year) + ", "
        + formatNumber(event.event_date.month) + ", "
        + formatNumber(event.event_date.day) + ", "
        + formatNumber(event.event_time.hour) + ", "
        + formatNumber(event.event_time.minute) + ", "
        + formatNumber(event.event_time.second) + ", "
        + (event.remind_flag ? "1" : "0") + ", "
        + formatNumber(event.remind_time.hour) + ", "
        + formatNumber(event.remind_time.minute) + ", "
        + formatNumber(event.remind_time.second) + ", "
        + (event.is_expired_flag ? "1" : "0") + ", "
        + (event.is_deleted ? "1" : "0") + ");";

    rc = sqlite3_exec(db, sqlInsert.c_str(), nullptr, nullptr, nullptr);
    handleSqliteError(rc, db);

    // 关闭数据库
    sqlite3_close(db);
}

void DBLayer::updateEventDb(Event event) { 
    sqlite3* db;
    int rc = sqlite3_open("Habbit.db", &db);
    handleSqliteError(rc, db);

    std::string sql = "UPDATE Events SET user_id = ?, title = ?, event_date_year = ?, event_date_month = ?, event_date_day = ?, "
        "event_time_hour = ?, event_time_minute = ?, event_time_second = ?, remind_flag = ?, "
        "remind_time_hour = ?, remind_time_minute = ?, remind_time_second = ?, "
        "is_expired_flag = ?, is_deleted = ? WHERE id = ?;";

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    handleSqliteError(rc, db);

    sqlite3_bind_int(stmt, 1, event.user_id);
    sqlite3_bind_text(stmt, 2, event.title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, event.event_date.year);
    sqlite3_bind_int(stmt, 4, event.event_date.month);
    sqlite3_bind_int(stmt, 5, event.event_date.day);
    sqlite3_bind_int(stmt, 6, event.event_time.hour);
    sqlite3_bind_int(stmt, 7, event.event_time.minute);
    sqlite3_bind_int(stmt, 8, event.event_time.second);
    sqlite3_bind_int(stmt, 9, static_cast<int>(event.remind_flag));
    sqlite3_bind_int(stmt, 10, event.remind_time.hour);
    sqlite3_bind_int(stmt, 11, event.remind_time.minute);
    sqlite3_bind_int(stmt, 12, event.remind_time.second);
    sqlite3_bind_int(stmt, 13, static_cast<int>(event.is_expired_flag));
    sqlite3_bind_int(stmt, 14, static_cast<int>(event.is_deleted));
    sqlite3_bind_int(stmt, 15, event.id);


    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        handleSqliteError(rc, db);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

//设置软字段为1
void DBLayer::deleteEventDb(int EventID) {

    // 打开数据库
    sqlite3* db;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);

    // SQL
    std::string sqlDelete = "UPDATE Events SET is_deleted = 1 WHERE ID = " + std::to_string(EventID) + ";";
    rc = sqlite3_exec(db, sqlDelete.c_str(), NULL, NULL, NULL);

    // 关闭数据库
    sqlite3_close(db);
    return;
}

void DBLayer::createPomodoroDb()
{
    sqlite3* db;
    int rc;
    char* zErrMsg = 0;
    rc = sqlite3_open("Habbit.db", &db);
    if (rc) {
        return;
    }
    const char* sql =
        "CREATE TABLE IF NOT EXISTS Pomo("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "last_time TEXT NOT NULL,"
        "name TEXT NOT NULL);";
    rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);

    sqlite3_close(db);
    return;
}

void DBLayer::createDateRecordDb()
{
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);
    if (rc)return;

    const char* sql =
        "CREATE TABLE IF NOT EXISTS DateRecord("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "date TEXT NOT NULL,"
        "time TEXT NOT NULL,"
        "pomo_id INTEGER,"
        "habit_id INTEGER,"
        "FOREIGN KEY(pomo_id) REFERENCES Pomo(id),"
        "FOREIGN KEY(habit_id) REFERENCES Habits(id));";

    rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
    sqlite3_close(db);
    return;
}


void DBLayer::insertHabitRecord(Habit habit) {
    sqlite3* db;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);
    rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
    const char* sql =
        "INSERT INTO DateRecord (date, time, pomo_id,habit_id)"
        "VALUES (?, ?, NULL, ?);";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return;
    }

    auto now = std::chrono::system_clock::now();

    // 将时间点转换为本地时间
    auto local_time = std::chrono::current_zone()->to_local(now);

    // 提取年月日
    auto year_month_day = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(local_time)
    };

    // 提取时分秒
    auto time_of_day = std::chrono::hh_mm_ss{
        local_time - std::chrono::floor<std::chrono::days>(local_time)
    };

    // 获取年、月、日
    int temp_year = static_cast<int>(year_month_day.year());
    unsigned temp_month = int(static_cast<unsigned>(year_month_day.month()));
    unsigned temp_day = static_cast<unsigned>(year_month_day.day());

    // 获取时、分、秒
    auto temp_hour = time_of_day.hours().count();
    auto temp_minute = time_of_day.minutes().count();
    auto temp_second = time_of_day.seconds().count();

    std::string hb_date = std::format("{:04}-{:02}-{:02}", temp_year, temp_month, temp_day);
    std::string hb_time = std::format("{:02}:{:02}:{:02}", temp_hour, temp_minute, temp_second);
    sqlite3_bind_text(stmt, 1, hb_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hb_time.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, habit.id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
}

void DBLayer::insertPomoRecord(Pomodoro pomo) {
    sqlite3* db;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);
    const char* sql =
        "INSERT INTO Pomo (last_time,name)"
        "VALUES (?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return;
    }
    std::string potime = std::format("{:02}:{:02}:{:02}", pomo.pomodoro_time.hour, pomo.pomodoro_time.minute, pomo.pomodoro_time.second);
    sqlite3_bind_text(stmt, 1, potime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pomo.record.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        return;
    }

    sqlite3_finalize(stmt);
    sql =
        "INSERT INTO DateRecord (date,time,pomo_id,habit_id)"
        "VALUES (date('now', 'localtime'),time('now', 'localtime'),?,NULL);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return;
    }
    sqlite3_bind_int(stmt, 1, getPomoIDMax());
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        return;
    }


    sqlite3_finalize(stmt);
    sqlite3_close(db);

}


DateRecord DBLayer::getRecordbyDate(Date date) {
    sqlite3* db1;
    int rc1;
    sqlite3_stmt* stmt1 = nullptr, * stmt2 = nullptr;
    DateRecord dr;
    Habit hb;
    Time hb_time;
    Pomodoro po;
    Time po_time;

    //打开数据库
    rc1 = sqlite3_open("Habbit.db", &db1);


    std::string str_date = std::format("{:04}-{:02}-{:02}", date.year, date.month, date.day);



    const char* sql1 = "SELECT * FROM DateRecord WHERE date = ?;";

    if (sqlite3_prepare_v2(db1, sql1, -1, &stmt1, nullptr) != SQLITE_OK) {
        return dr;
    }
    const char* target_date = str_date.c_str();
    sqlite3_bind_text(stmt1, 1, target_date, -1, SQLITE_STATIC);
    while (sqlite3_step(stmt1) == SQLITE_ROW) {
        int habit_id = sqlite3_column_int(stmt1, 4);
        int pomo_id = sqlite3_column_int(stmt1, 3);


        //在习惯库中查找
        if (habit_id != 0) {


            const char* sql2 = "SELECT * FROM Habits where id = ?;";
            if (sqlite3_prepare_v2(db1, sql2, -1, &stmt2, nullptr) != SQLITE_OK) {
                return dr;
            }
            sqlite3_bind_int(stmt2, 1, habit_id);
            while (sqlite3_step(stmt2) == SQLITE_ROW) {

                hb.id = sqlite3_column_int(stmt2, 0);
                hb.user_id = sqlite3_column_int(stmt2, 1);
                hb.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 2));
                hb.name = UTF8ToGBK(hb.name);
                hb.target_count = sqlite3_column_int(stmt2, 3);

                const char* startdate = reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 4));
                std::string str1(startdate);
                hb.start_date.day = std::stoi(str1.substr(8, 2));
                hb.start_date.month = std::stoi(str1.substr(5, 2));
                hb.start_date.year = std::stoi(str1.substr(0, 4));


                const char* enddate = reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 5));
                std::string str2(enddate);
                hb.end_date.day = std::stoi(str2.substr(8, 2));
                hb.end_date.month = std::stoi(str2.substr(5, 2));
                hb.end_date.year = std::stoi(str2.substr(0, 4));

                hb.is_active = sqlite3_column_int(stmt2, 6);
                hb.is_deleted = sqlite3_column_int(stmt2, 7);

                const char* hbtime = reinterpret_cast<const char*>(sqlite3_column_text(stmt1, 2));
                std::string hbtime1(hbtime);
                hb_time.hour = std::stoi(hbtime1.substr(0, 2));
                hb_time.minute = std::stoi(hbtime1.substr(3, 2));
                hb_time.second = std::stoi(hbtime1.substr(6, 2));


                dr.Habit_Record.push_back({ hb_time,hb });
            }
        }

        if (pomo_id != 0) {


            const char* sql2 = "SELECT * FROM Pomo where id = ?;";
            if (sqlite3_prepare_v2(db1, sql2, -1, &stmt2, nullptr) != SQLITE_OK) {
                return dr;
            }
            sqlite3_bind_int(stmt2, 1, pomo_id);
            while (sqlite3_step(stmt2) == SQLITE_ROW) {
                po.id = pomo_id;
                const char* lasttime = reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 1));
                std::string str1(lasttime);
                po.pomodoro_time.hour = std::stoi(str1.substr(0, 2));
                po.pomodoro_time.minute = std::stoi(str1.substr(3, 2));
                po.pomodoro_time.second = std::stoi(str1.substr(6, 2));

                const char* record = reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 2));
                po.record.assign(record);
                po.record = UTF8ToGBK(po.record);

                const char* potime = reinterpret_cast<const char*>(sqlite3_column_text(stmt1, 2));
                std::string potime1(potime);
                po_time.hour = std::stoi(potime1.substr(0, 2));
                po_time.minute = std::stoi(potime1.substr(3, 2));
                po_time.second = std::stoi(potime1.substr(6, 2));


                dr.Pomodoro_Record.push_back({ po_time,po });
            }
        }

    }
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db1);

    return dr;
}


// 回调函数，用于处理查询结果
static int getHabitIDMax_callback(void* data, int argc, char** argv, char** azColName) {
    int* maxId = static_cast<int*>(data);
    if (argv[0]) {
        *maxId = std::stoi(argv[0]);
    }
    return 0;
}

int DBLayer::getHabitIDMax()
{
    // 打开数据库
    int maxId = 0;
    sqlite3* db;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);

    // SQL
    rc = sqlite3_exec(db, "SELECT MAX(ID) FROM Habits;", getHabitIDMax_callback, &maxId, NULL);

    // 关闭数据库
    sqlite3_close(db);
    return maxId;
}

// 回调函数，用于处理查询结果
static int getEventIDMax_callback(void* data, int argc, char** argv, char** azColName) {
    int* maxId = static_cast<int*>(data);
    if (argv[0]) {
        *maxId = std::stoi(argv[0]);
    }
    return 0;
}

int DBLayer::getEventIDMax()
{
    // 打开数据库
    int maxId = 0;
    sqlite3* db;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);

    // SQL
    rc = sqlite3_exec(db, "SELECT MAX(ID) FROM Events;", getEventIDMax_callback, &maxId, NULL);

    // 关闭数据库
    sqlite3_close(db);
    return maxId;
}

MOUSEMSG DBLayer::getmouse() {
    MOUSEMSG msg = GetMouseMsg();
    FlushMouseMsgBuffer();
    return msg;
}


// 实现 handleSqliteError 函数
void DBLayer::handleSqliteError(int rc, sqlite3* db) {
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(1);
    }
}

// 回调函数，用于处理查询结果
static int getPomoIDMax_callback(void* data, int argc, char** argv, char** azColName) {
    int* maxId = static_cast<int*>(data);
    if (argv[0]) {
        *maxId = std::stoi(argv[0]);
    }
    return 0;
}

int DBLayer::getPomoIDMax()
{
    // 打开数据库
    int maxId = 0;
    sqlite3* db;
    int rc;
    rc = sqlite3_open("Habbit.db", &db);

    // SQL
    rc = sqlite3_exec(db, "SELECT MAX(ID) FROM Pomo;", getPomoIDMax_callback, &maxId, NULL);

    // 关闭数据库
    sqlite3_close(db);
    return maxId;
}
