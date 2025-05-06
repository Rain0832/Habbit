#ifndef TIMES_H
#define TIMES_H
#include <functional>
#include<string>
//用来存储年月日
struct Date
{
    int year;  // 自 1900 年起的年数
    int month; // 月，范围从 0 到 11
    int day;   // 一月中的第几天，范围从 1 到 31

    explicit Date(int year = 0, int month = 0, int day = 0);

    explicit Date(const std::string& dateStr);

    std::string to_String();

    bool operator==(const Date& other) const;

    bool operator<(const Date& other) const;

    bool operator>(const Date& other) const;

    // Date 考虑实现 >= 和 <= 运算符重载
    bool operator>=(const Date& other) const;

    bool operator<=(const Date& other) const;
};


//存储时分秒
struct Time
{
    int hour;       //时
    int minute;     //分
    int second;     //秒（仅番茄钟需要）

    explicit Time(int hour = 0, int minute = 0, int second = 0);

    std::string to_String();

    bool operator==(const Time& other) const;

    bool operator<(const Time& other) const;

    bool operator>(const Time& other) const;

    Time operator+(const Time& other) const;
};
#endif // DATETIME_H