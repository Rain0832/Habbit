#include "Times.h"
#include <iomanip> // 用于 std::setw 和 std::setfill
#include <sstream> // 用于 std::ostringstream
// Date 构造函数实现
Date::Date(int year, int month, int day) 
    : year(year), month(month), day(day) {}


// Time 构造函数实现
Time::Time(int hour, int minute, int second) 
    : hour(hour), minute(minute), second(second) {}


// 从字符串构造的构造函数
Date::Date(const std::string& dateStr) {
    std::istringstream iss(dateStr);
    char dash1, dash2;
    iss >> year >> dash1 >> month >> dash2 >> day;
}


std::string Date::to_String()
{
    std::ostringstream oss;
    // 将 year 转换为实际年份（1900 + year）
    oss << std::setw(4) << std::setfill('0') << (year) << "-"
        << std::setw(2) << std::setfill('0') << (month) << "-"
        << std::setw(2) << std::setfill('0') << day;
    return oss.str();
}

bool Date::operator>=(const Date& other) const
{
    if (this->year >= other.year) {
        return true;
    }
    if (this->month >= other.month) {
        return true;
    }

    return this->day >= other.day;
}

bool Date::operator<=(const Date& other) const
{
    if (this->year <= other.year) {
        return true;
    }
    if (this->month <= other.month) {
        return true;
    }

    return this->day <= other.day;
}

bool Time::operator<(const Time& other) const
{
    if (this->hour <= other.hour)return true;   
    else if (this->hour > other.hour)return false;

    if (this->minute <= other.minute)return true;
    else if (this->minute > other.minute)return false;

    return this->second <= other.second;
}

bool Time::operator>(const Time& other) const
{
    if (this->hour > other.hour)return true;
    else if (this->hour < other.hour)return false;

    if (this->minute > other.minute)return true;
    else if (this->minute < other.minute)return false;

    return this->second > other.second;
}

// Time 结构体 + 运算符重载
Time Time::operator+(const Time& other) const {
    int totalSeconds = this->second + other.second;
    int totalMinutes = this->minute + other.minute + totalSeconds / 60;
    int totalHours = this->hour + other.hour + totalMinutes / 60;

    int finalSeconds = totalSeconds % 60;
    int finalMinutes = totalMinutes % 60;

    return Time(totalHours, finalMinutes, finalSeconds);
}

// Date 相等运算符重载
bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}

bool Date::operator<(const Date& other) const
{
    if (this->year < other.year) {
        return true;
    }
    else if (this->year > other.year) {
        return false;
    }

    // 年份相等时，比较月份
    if (this->month < other.month) {
        return true;
    }
    else if (this->month > other.month) {
        return false;
    }

    // 年份和月份相等时，比较日期
    return this->day < other.day;
}

bool Date::operator>(const Date& other) const
{
    if (this->year > other.year) {
        return true;
    }
    else if (this->year < other.year) {
        return false;
    }

    // 年份相等时，比较月份
    if (this->month > other.month) {
        return true;
    }
    else if (this->month < other.month) {
        return false;
    }

    // 年份和月份相等时，比较日期
    return this->day > other.day;
}

std::string Time::to_String()
{
    std::ostringstream oss;

    oss << std::setw(2) << std::setfill('0') << (hour) << ":"
        << std::setw(2) << std::setfill('0') << (minute) << ":"
        << std::setw(2) << std::setfill('0') << second;
    return oss.str();
}