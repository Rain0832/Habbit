#include "ServiceLayer.h"
#include<iostream>
#include<chrono>
bool ServiceLayer::IfMouseHit(int x, int y, int w, int h, MOUSEMSG msg)
{
	// 判断是否为鼠标左键按下事件
	if (msg.uMsg == WM_LBUTTONDOWN)
	{
		// 判断鼠标点击位置是否在目标区域内
		bool inArea = msg.x >= x && msg.x <= x + w && msg.y >= y && msg.y <= y + h;
		const std::string b = inArea ? "true" : "false";
		std::cout << msg.x << " " << msg.y << " " << b << std::endl;

		return inArea;
	}
	return false; // 如果没有点击，返回 false
}

bool ServiceLayer::insertHabit(std::string name, Date start_date, Date end_date, int times_per_day)
{
	auto times = this->getCurrentTime();
	Date cur_date = times.first;
	Time cur_time = times.second;
	if (name.size() == 0 || times_per_day <= 0 || start_date > end_date || end_date < cur_date)
	{
		return false;
	}
	this->db_layer.insertHabitDb(Habit{ this->db_layer.getHabitIDMax() + 1, 0, name, times_per_day,
										start_date, end_date, true, false });

	return true;
}

bool ServiceLayer::updateHabit(int HabitID, Date beginDate, Date endDate, int times_per_day, bool active_flag)
{
	auto times = this->getCurrentTime();
	Date cur_date = times.first;
	Time cur_time = times.second;
	if (times_per_day <= 0 || beginDate > endDate || endDate < cur_date)
	{
		return false;
	}
	Habit habit = this->getHabitByID(HabitID);
	habit.start_date = beginDate;
	habit.end_date = endDate;
	habit.target_count = times_per_day;
	habit.is_active = active_flag;
	this->db_layer.updateHabitDb(habit);

	return true;
}

bool ServiceLayer::deleteHabit(int HabitID)
{
	this->db_layer.deleteHabitDb(HabitID);
	return true;
}

bool ServiceLayer::insertEvent(std::string name, Date event_date, Time event_time, bool remind_flag, Time remind_time)
{
	auto times = this->getCurrentTime();
	Date cur_date = times.first;
	Time cur_time = times.second;
	if (name.size() == 0 || remind_time > event_time || cur_date > event_date)
	{
		return false;
	}
	this->db_layer.insertEventDb(Event{ this->db_layer.getEventIDMax() + 1, 0, name,
		event_date, event_time, remind_flag, remind_time });

	return true;
}

bool ServiceLayer::updateEvent(int EventID, std::string title, Date event_date, Time event_time, bool remind_flag, Time remind_time)
{
	auto times = this->getCurrentTime();
	Date cur_date = times.first;
	Time cur_time = times.second;
	if (remind_time > event_time || cur_date > event_date)
	{
		return false;
	}
	Event event = this->getEventByID(EventID);
	event.title = title;
	event.event_date = event_date;
	event.event_time = event_time;
	event.remind_flag = remind_flag;
	event.remind_time = remind_time;
	this->db_layer.updateEventDb(event);

	return true;
}

bool ServiceLayer::deleteEvent(int EventID)
{
	this->db_layer.deleteEventDb(EventID);
	return true;
}

bool ServiceLayer::checkinHabit(Habit habit)
{
	db_layer.DBLayer::insertHabitRecord(habit);
	return true;
}

std::vector<Habit> ServiceLayer::getActiveHabit()
{
	std::vector<Habit> allHabits = db_layer.getHabit();
	std::vector<Habit> activeHabits;

	for (const auto& habit : allHabits) {
		if (habit.is_active && !habit.is_deleted) {
			activeHabits.push_back(habit);
		}
	}

	return activeHabits;
}

std::vector<Habit> ServiceLayer::getInactiveHabit()
{
	std::vector<Habit> allhabits = db_layer.getHabit();
	std::vector<Habit> inactiveHabits;

	for (const auto& habit : allhabits) {
		if (!habit.is_active || habit.is_deleted) {
			inactiveHabits.push_back(habit);
		}
	}

	return inactiveHabits;
}

std::vector<Event> ServiceLayer::getActiveEvent()
{
	std::vector<Event> allEvents = db_layer.getEvent();
	std::vector<Event> activeEvents;

	for (const auto& event : allEvents) {
		if (!event.is_expired_flag && !event.is_deleted) {
			activeEvents.push_back(event);
		}
	}

	return activeEvents;
}

std::vector<Event> ServiceLayer::getExpiredEvent()
{
	std::vector<Event> allEvents = db_layer.getEvent();
	std::vector<Event> ExpiredEvents;

	for (const auto& event : allEvents) {
		if (event.is_expired_flag || event.is_deleted) {
			ExpiredEvents.push_back(event);
		}
	}

	return ExpiredEvents;
}

bool ServiceLayer::pomodoroTick(Pomodoro pomodoro, Time count_time)
{
	if ((pomodoro.pomodoro_time + count_time) < getCurrentTime().second)
	{
		// 番茄钟到时
		db_layer.insertPomoRecord(pomodoro);
		return false;
	}
	return true;
}

std::vector<std::pair<int, int>> ServiceLayer::getHabitRecordbyDate(Date date)
{
	// 返回值
	std::vector<std::pair<int, int>> record;

	// 循环取值 遍历 date.year, date.month 的每天的习惯记录
	Date d(date.year, date.month, 1);

	static int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (date.month == 2 && (date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0))
	{
		days[1] = 29;
	}

	while (d.day <= days[d.month - 1])
	{
		int should_count = 0;
		int actual_count = 0;
		std::vector<Habit> habit = db_layer.getHabit();
		for (auto h : habit)
		{
			// Date 考虑实现 >= 和 <= 运算符重载
			if (h.start_date <= d && h.end_date >= d)
			{
				should_count += h.target_count;
			}
		}
		actual_count = db_layer.getRecordbyDate(d).Habit_Record.size();
		record.push_back(std::make_pair(actual_count, should_count));
		d.day++;
	}
	return record;
}

DateRecord ServiceLayer::getAllRecordbyDate(Date date)
{
	return db_layer.getRecordbyDate(date);
}

std::pair<Date, Time> ServiceLayer::getCurrentTime() const
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);

	struct tm local_tm {};
	errno_t result = localtime_s(&local_tm, &now_time);
	if (result != 0)
	{
		throw std::runtime_error("Failed to convert time.");
	}

	Date date(local_tm.tm_year + 1900, local_tm.tm_mon + 1, local_tm.tm_mday);
	Time time(local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec);

	return { date, time };
}

Habit ServiceLayer::getHabitByID(int habit_id)
{
	std::vector<Habit> habits = db_layer.getHabit();
	for (auto h : habits)
	{
		if (h.id == habit_id)
			return h;
	}
}

Event ServiceLayer::getEventByID(int event_id)
{
	std::vector<Event> events = db_layer.getEvent();
	for (auto e : events)
	{
		if (e.id == event_id)return e;
	}
}

void ServiceLayer::init()
{
	db_layer.createHabitDb();
	db_layer.createEventDb();
	db_layer.createPomodoroDb();
	db_layer.createDateRecordDb();
}
