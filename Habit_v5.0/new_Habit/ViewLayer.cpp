#include "ViewLayer.h"
#include<easyx.h>
#include<graphics.h>
#include <windows.h>
#include<iostream>
#include<queue>
#include<algorithm>
#include <math.h>
#define	 PI 3.14159


// 习惯管理界面
void ViewLayer::habitManageView(MOUSEMSG &msg)
{
    settextstyle(30, 0, _T("三极萌萌简体"));
    // 获取活跃和不活跃习惯
    std::vector<Habit> ActiveHabit = Sv_Layer.getActiveHabit();
    std::vector<Habit> InactiveHabit = Sv_Layer.getInactiveHabit();

    // 窗口尺寸
    int windowWidth = 1200;
    int windowHeight = 900;


    // 1. 绘制活跃习惯
    int activeX = 50; // 起始 x 坐标
    int activeY = 50; // 起始 y 坐标
    int habitWidth = 300; // 每个习惯框的宽度
    int habitHeight = 250; // 每个习惯框的高度
    int buttonWidth = 80; // 按钮的宽度
    int buttonHeight = 35; // 按钮的高度
    int buttonSpacing = 10; // 按钮之间的间距
    int habitsPerRow = 4; // 每行显示的习惯数量

    settextcolor(BLACK);
    outtextxy(activeX, activeY - 30, _T("活跃习惯")); // 标题

    for (size_t i = 0; i < ActiveHabit.size(); i++) {
        settextcolor(RGB(237, 137, 204));
        int habitX = activeX + (i % habitsPerRow) * (habitWidth + 20); // 计算每个习惯框的 x 坐标
        int habitY = activeY + (i / habitsPerRow) * (habitHeight + 20); // 计算每个习惯框的 y 坐标

        // 绘制习惯框背景
        setfillcolor(RGB(255,255,168)); 
        solidrectangle(habitX, habitY, habitX + habitWidth, habitY + habitHeight);

        // 绘制习惯名称
        std::wstring name = ANSIToUnicode(ActiveHabit[i].name);
        outtextxy(habitX + 10, habitY + 10, (L"名称: " + name).c_str());

        // 绘制打卡次数
        std::wstring targetCount = std::to_wstring(ActiveHabit[i].target_count);
        outtextxy(habitX + 10, habitY + 40, (L"打卡次数: " + targetCount).c_str());

        // 绘制开始时间
        std::wstring startDate = ANSIToUnicode(ActiveHabit[i].start_date.to_String());
        outtextxy(habitX + 10, habitY + 70, (L"开始时间: " + startDate).c_str());

        // 绘制结束时间
        std::wstring endDate = ANSIToUnicode(ActiveHabit[i].end_date.to_String());
        outtextxy(habitX + 10, habitY + 100, (L"结束时间: " + endDate).c_str());

        // 绘制修改按钮
        int modifyButtonX = habitX + 10;
        int modifyButtonY = habitY + habitHeight - buttonHeight - 10;
        DrawButton(modifyButtonX, modifyButtonY, buttonWidth, buttonHeight, "修改", RGB(154, 255, 154), RGB(237,137,204));
        if (Sv_Layer.IfMouseHit(modifyButtonX, modifyButtonY, buttonWidth, buttonHeight, msg)) {
            habitUpdateView(ActiveHabit[i]);
        }

        // 绘制删除按钮
        int deleteButtonX = modifyButtonX + buttonWidth + buttonSpacing;
        int deleteButtonY = modifyButtonY;
        DrawButton(deleteButtonX, deleteButtonY, buttonWidth, buttonHeight, "删除", RGB(154, 255, 154), RGB(237, 137, 204));
        if (Sv_Layer.IfMouseHit(deleteButtonX, deleteButtonY, buttonWidth, buttonHeight, msg)) {
            habitDeleteView(ActiveHabit[i]);
        }

        // 绘制打卡按钮
        int checkButtonX = deleteButtonX + buttonWidth + buttonSpacing;
        int checkButtonY = modifyButtonY;
        DrawButton(checkButtonX, checkButtonY, buttonWidth, buttonHeight, "打卡", RGB(154, 255, 154), RGB(237, 137, 204));
        if (Sv_Layer.IfMouseHit(checkButtonX, checkButtonY, buttonWidth, buttonHeight, msg)) {
            if(Sv_Layer.checkinHabit(ActiveHabit[i]));
                MessageBox(NULL, _T("打卡成功！"), 
                    ANSIToUnicode(ActiveHabit[i].name).c_str()
                    , MB_OK);
            
        }
    }

    // 2. 绘制不活跃习惯
    int inactiveY = activeY + ((ActiveHabit.size() + habitsPerRow - 1) / habitsPerRow) * (habitHeight + 20) + 50; // 计算不活跃习惯的起始 y 坐标

    settextcolor(BLACK);
    outtextxy(activeX, inactiveY - 30, _T("不活跃习惯")); // 标题

    for (size_t i = 0; i < InactiveHabit.size(); i++) {
        settextcolor(RGB(88, 112, 159));
        int habitX = activeX + (i % habitsPerRow) * (habitWidth + 20); // 计算每个习惯框的 x 坐标
        int habitY = inactiveY + (i / habitsPerRow) * (habitHeight + 20); // 计算每个习惯框的 y 坐标

        // 绘制习惯框背景
        setfillcolor(RGB(211, 211, 211)); // 浅灰色背景
        solidrectangle(habitX, habitY, habitX + habitWidth, habitY + habitHeight);

        // 绘制习惯名称
        std::wstring name = ANSIToUnicode(InactiveHabit[i].name);
        outtextxy(habitX + 10, habitY + 10, (L"名称: " + name).c_str());

   
        // 绘制打卡次数
        std::wstring targetCount = std::to_wstring(InactiveHabit[i].target_count);
        outtextxy(habitX + 10, habitY + 40, (L"打卡次数: " + targetCount).c_str());

        // 绘制开始时间
        std::wstring startDate = ANSIToUnicode(InactiveHabit[i].start_date.to_String());
        outtextxy(habitX + 10, habitY + 70, (L"开始时间: " + startDate).c_str());

        // 绘制结束时间
        std::wstring endDate = ANSIToUnicode(InactiveHabit[i].end_date.to_String());
        outtextxy(habitX + 10, habitY + 100, (L"结束时间: " + endDate).c_str());

        // 绘制修改按钮
        int modifyButtonX = habitX + 10;
        int modifyButtonY = habitY + habitHeight - buttonHeight - 10;
        DrawButton(modifyButtonX, modifyButtonY, buttonWidth, buttonHeight, "修改", RGB(211, 211, 211), RGB(88, 112, 159));
        if (Sv_Layer.IfMouseHit(modifyButtonX, modifyButtonY, buttonWidth, buttonHeight, msg)) {
            habitUpdateView(ActiveHabit[i]);
        }

        // 绘制删除按钮
        int deleteButtonX = modifyButtonX + buttonWidth + buttonSpacing;
        int deleteButtonY = modifyButtonY;
        DrawButton(deleteButtonX, deleteButtonY, buttonWidth, buttonHeight, "删除", RGB(211, 211, 211), RGB(88, 112, 159));
        if (Sv_Layer.IfMouseHit(deleteButtonX, deleteButtonY, buttonWidth, buttonHeight, msg)) {
            habitDeleteView(ActiveHabit[i]);
        }
    }

    // 3. 在界面下方中央绘制“添加习惯”按钮
    int addButtonWidth = 150; // 添加习惯按钮的宽度
    int addButtonHeight = 40; // 添加习惯按钮的高度
    int addButtonX = (windowWidth - addButtonWidth) / 2; // 按钮水平居中
    int addButtonY = windowHeight - 80; // 按钮位于窗口底部上方 80 像素处

    DrawButton(addButtonX, addButtonY, addButtonWidth, addButtonHeight, "添加习惯", RGB(255, 255, 204), RGB(173, 238, 232));
    if (Sv_Layer.IfMouseHit(addButtonX, addButtonY, addButtonWidth, addButtonHeight, msg)) {
        habitInsertView();
    }

    //在界面右上角绘制“退出”按钮
    returnButtomView(170, 50, msg);
}

// 新建习惯界面
void ViewLayer::habitInsertView() {
    wchar_t buf[256]; // 用于存储用户输入的宽字符缓冲区
    // 1. 输入习惯名称
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入习惯名称:", L"新建习惯")) {
        habitNameInput = WideCharToUTF8(buf); // 将 wchar_t* 转换为 std::string
    }
    else {
        MessageBox(NULL, _T("习惯名称不正确！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }

    // 2. 输入目标打卡次数
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入目标打卡次数:", L"新建习惯")) {
        habitTargetCountInput = std::stoi(WideCharToUTF8(buf)); // 将 wchar_t* 转换为 int
    }
    else {
        MessageBox(NULL, _T("打卡次数非法！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }

    // 3. 输入开始日期
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入开始日期 (YYYY-MM-DD):", L"新建习惯")) {
        std::string startDate = WideCharToUTF8(buf); // 将 wchar_t* 转换为 std::string
        size_t dash1 = startDate.find('-');
        size_t dash2 = startDate.find('-', dash1 + 1);
        if (dash1 != std::string::npos && dash2 != std::string::npos) {
            start_year_input = std::stoi(startDate.substr(0, dash1));
            start_month_input = std::stoi(startDate.substr(dash1 + 1, dash2 - dash1 - 1));
            start_day_input = std::stoi(startDate.substr(dash2 + 1));
        }
    }
    else {
        MessageBox(NULL, _T("日期格式不正确！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }

    // 4. 输入结束日期
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入结束日期 (YYYY-MM-DD):", L"新建习惯")) {
        std::string endDate = WideCharToUTF8(buf); // 将 wchar_t* 转换为 std::string
        size_t dash1 = endDate.find('-');
        size_t dash2 = endDate.find('-', dash1 + 1);
        if (dash1 != std::string::npos && dash2 != std::string::npos) {
            end_year_input = std::stoi(endDate.substr(0, dash1));
            end_month_input = std::stoi(endDate.substr(dash1 + 1, dash2 - dash1 - 1));
            end_day_input = std::stoi(endDate.substr(dash2 + 1));
        }
    }
    else {
        MessageBox(NULL, _T("日期格式不正确！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }
    Sv_Layer.insertHabit(habitNameInput, Date(start_year_input, start_month_input, start_day_input),
        Date(end_year_input, end_month_input, end_day_input), habitTargetCountInput);
}

// 修改习惯界面
void ViewLayer::habitUpdateView(Habit h) {
    wchar_t buf[256]; // 用于存储用户输入的宽字符缓冲区
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入开始日期 (YYYY-MM-DD):",
        ANSIToUnicode("修改习惯：" + h.name).c_str())) {
        std::string startDate = WideCharToUTF8(buf); // 将 wchar_t* 转换为 std::string
        size_t dash1 = startDate.find('-');
        size_t dash2 = startDate.find('-', dash1 + 1);
        if (dash1 != std::string::npos && dash2 != std::string::npos) {
            start_year_input = std::stoi(startDate.substr(0, dash1));
            start_month_input = std::stoi(startDate.substr(dash1 + 1, dash2 - dash1 - 1));
            start_day_input = std::stoi(startDate.substr(dash2 + 1));
        }
    }
    else {
        MessageBox(NULL, _T("日期格式不正确！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }
    //输入结束日期
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入结束日期 (YYYY-MM-DD):",
        ANSIToUnicode("修改习惯：" + h.name).c_str())) {
        std::string endDate = WideCharToUTF8(buf); // 将 wchar_t* 转换为 std::string
        size_t dash1 = endDate.find('-');
        size_t dash2 = endDate.find('-', dash1 + 1);
        if (dash1 != std::string::npos && dash2 != std::string::npos) {
            end_year_input = std::stoi(endDate.substr(0, dash1));
            end_month_input = std::stoi(endDate.substr(dash1 + 1, dash2 - dash1 - 1));
            end_day_input = std::stoi(endDate.substr(dash2 + 1));
        }
    }
    else {
        return; // 用户取消输入
    }
    //输入目标打卡次数
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入目标打卡次数:", 
        ANSIToUnicode("修改习惯：" + h.name).c_str())) {
        habitTargetCountInput = std::stoi(WideCharToUTF8(buf)); // 将 wchar_t* 转换为 int
    }
    else {
        MessageBox(NULL, _T("打卡次数非法！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }
    //输入目标打卡次数
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入是否启用（0为不启用，1为启用）:",
        ANSIToUnicode("修改习惯：" + h.name).c_str())) {
        flag = std::stoi(WideCharToUTF8(buf)); // 将 wchar_t* 转换为 int
    }
    else {
        MessageBox(NULL, _T("标志设置不正确！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }
    Sv_Layer.updateHabit(h.id, Date(start_year_input, start_month_input, start_day_input),
        Date(end_year_input, end_month_input, end_day_input), habitTargetCountInput, flag);
}
// 删除习惯界面
void ViewLayer::habitDeleteView(Habit h) {
    wchar_t buf[256]; // 用于存储用户输入的宽字符缓冲区
    //是否确认删除
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入是否删除（0为不删除，1为删除）:",
        ANSIToUnicode("删除习惯习惯：" + h.name).c_str())) {
        flag = std::stoi(WideCharToUTF8(buf)); // 将 wchar_t* 转换为 int
    }
    else {
        MessageBox(NULL, _T("标志设置不正确！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }
    if (flag)Sv_Layer.deleteHabit(h.id);
}

// 事项管理界面
void ViewLayer::eventManageView(MOUSEMSG &msg) {

    settextstyle(30, 0, _T("三极萌萌简体"));
    // 获取活跃事项和过期事项
    std::vector<Event> activeEvents = Sv_Layer.getActiveEvent();  // 调用 ServiceLayer 获取活跃事项
    std::vector<Event> expiredEvents = Sv_Layer.getExpiredEvent();  // 获取过期事项

    // 窗口尺寸
    int windowWidth = 1200;
    int windowHeight = 900;

    // 1. 绘制活跃事项
    int activeX = 50;      // 起始 x 坐标
    int activeY = 50;      // 起始 y 坐标
    int eventWidth = 300;  // 每个事项框的宽度
    int eventHeight = 250; // 每个事项框的高度
    int buttonWidth = 80;  // 按钮的宽度
    int buttonHeight = 30; // 按钮的高度
    int buttonSpacing = 10; // 按钮之间的间距
    int eventsPerRow = 3;  // 每行显示的事项数量

    settextcolor(BLACK);
    outtextxy(activeX, activeY - 30, _T("活跃中")); // 标题

    // 获取当前时间，用于计算剩余时间
    auto currentPair = Sv_Layer.getCurrentTime();
    Date currentDate = currentPair.first;
    Time currentTime = currentPair.second;
    std::tm currentTm = {};
    currentTm.tm_year = currentDate.year - 1900;
    currentTm.tm_mon = currentDate.month - 1;
    currentTm.tm_mday = currentDate.day;
    currentTm.tm_hour = currentTime.hour;
    currentTm.tm_min = currentTime.minute;
    currentTm.tm_sec = currentTime.second;
    time_t currentTime_t = mktime(&currentTm);

    // 绘制活跃事项
    for (size_t i = 0; i < activeEvents.size(); i++) {
        settextcolor(RGB(237, 137, 204));
        int eventX = activeX + (i % eventsPerRow) * (eventWidth + 20); // 计算每个事项框的 x 坐标
        int eventY = activeY + (i / eventsPerRow) * (eventHeight + 20); // 计算每个事项框的 y 坐标

        // 绘制事项框背景
        setfillcolor(RGB(255, 255, 168));
        solidrectangle(eventX, eventY, eventX + eventWidth, eventY + eventHeight);

        // 绘制事项名称
        std::wstring name = ANSIToUnicode(activeEvents[i].title);
        outtextxy(eventX + 10, eventY + 10, name.c_str());

        // 计算事项到期时间（结合 event_date 与 event_time）
        std::tm eventTm = {};
        eventTm.tm_year = activeEvents[i].event_date.year - 1900;
        eventTm.tm_mon = activeEvents[i].event_date.month - 1;
        eventTm.tm_mday = activeEvents[i].event_date.day;
        eventTm.tm_hour = activeEvents[i].event_time.hour;
        eventTm.tm_min = activeEvents[i].event_time.minute;
        eventTm.tm_sec = activeEvents[i].event_time.second;
        time_t eventTime_t = mktime(&eventTm);

        double diffSec = difftime(eventTime_t, currentTime_t);
        std::wstring remainingTimeWStr;
        if (diffSec >= 86400) {  // 大于等于24小时
            int days = diffSec / 86400;
            remainingTimeWStr = std::to_wstring(days) + L"天";
        }
        else if (diffSec >= 3600) {  // 小于24小时但大于等于1小时
            int hours = diffSec / 3600;
            remainingTimeWStr = std::to_wstring(hours) + L"小时";
        }
        else {  // 小于1小时
            int minutes = diffSec / 60;
            remainingTimeWStr = std::to_wstring(minutes) + L"分钟";
        }
        // 绘制剩余时间
        std::wstring remainingText = L"剩余: " + remainingTimeWStr;
        outtextxy(eventX + 10, eventY + 40, remainingText.c_str());

        // 绘制“修改”按钮
        int modifyButtonX = eventX + 10;
        int modifyButtonY = eventY + eventHeight - buttonHeight - 10;
        DrawButton(modifyButtonX, modifyButtonY, buttonWidth, buttonHeight, "修改", RGB(154,255,154), RGB(237, 137, 204));
        if (Sv_Layer.IfMouseHit(modifyButtonX, modifyButtonY, buttonWidth, buttonHeight, msg)) {
            // 传入整个 Event 对象
            eventUpdateView(activeEvents[i]);
        }

        // 绘制“删除”按钮
        int deleteButtonX = modifyButtonX + buttonWidth + buttonSpacing;
        int deleteButtonY = modifyButtonY;
        DrawButton(deleteButtonX, deleteButtonY, buttonWidth, buttonHeight, "删除", RGB(154,255,154), RGB(237, 137, 204));
        if (Sv_Layer.IfMouseHit(deleteButtonX, deleteButtonY, buttonWidth, buttonHeight, msg)) {
            // 传入整个 Event 对象
            eventDeleteView(activeEvents[i]);
        }
    }

    // 2. 绘制过期事项
    int expiredX = 50;
    int expiredY = activeY + ((activeEvents.size() + eventsPerRow - 1) / eventsPerRow) * (eventHeight + 20) + 40; // 计算过期事项的起始 y 坐标

    settextcolor(BLACK);
    outtextxy(expiredX, expiredY - 30, _T("已过期")); // 标题

    // 绘制过期事项
    for (size_t i = 0; i < expiredEvents.size(); i++) {
        settextcolor(RGB(88, 112, 159));
        int eventX = expiredX + (i % eventsPerRow) * (eventWidth + 20); // 计算每个事项框的 x 坐标
        int eventY = expiredY + (i / eventsPerRow) * (eventHeight + 20); // 计算每个事项框的 y 坐标

        // 绘制事项框背景
        setfillcolor(RGB(240, 240, 240));
        solidrectangle(eventX, eventY, eventX + eventWidth, eventY + eventHeight);

        // 绘制事项名称
        std::wstring name = ANSIToUnicode(expiredEvents[i].title);
        outtextxy(eventX + 10, eventY + 10, name.c_str());

        // 对于过期事项，直接显示到期日期
        std::wstring eventDate = ANSIToUnicode(expiredEvents[i].event_date.to_String());
        outtextxy(eventX + 10, eventY + 40, eventDate.c_str());

        // 绘制“删除”按钮
        int deleteButtonX = eventX + 10;
        int deleteButtonY = eventY + eventHeight - buttonHeight - 10;
        DrawButton(deleteButtonX, deleteButtonY, buttonWidth, buttonHeight, "删除", RGB(211, 211, 211), RGB(88, 112, 159));
        if (Sv_Layer.IfMouseHit(deleteButtonX, deleteButtonY, buttonWidth, buttonHeight, msg)) {
            // 传入整个 Event 对象
            eventDeleteView(expiredEvents[i]);
        }
    }

    settextstyle(40, 0, _T("三极萌萌简体"));

    // 3. 在界面下方中央绘制“添加事项”按钮
    int addButtonWidth = 150;
    int addButtonHeight = 50;
    int addButtonX = (windowWidth - addButtonWidth) / 2;
    int addButtonY = windowHeight - 80;

    DrawButton(addButtonX, addButtonY, addButtonWidth, addButtonHeight, "添加事项", RGB(255, 255, 204), RGB(121, 238, 227));
    if (Sv_Layer.IfMouseHit(addButtonX, addButtonY, addButtonWidth, addButtonHeight, msg)) {
        eventInsertView();
    }

    //在界面右上角绘制“退出”按钮
    returnButtomView(170, 50, msg);
}

// 新建事项界面
void ViewLayer::eventInsertView() {
    wchar_t buf[256]; // 用于存储用户输入的宽字符缓冲区

    // 1. 输入事项名称
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入事项名称:", L"新建事项")) {
        eventNameInput = WideCharToUTF8(buf);  // 将 wchar_t* 转换为 std::string
    }
    else {
        MessageBox(NULL, _T("事项名称格式不正确！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }

    // 2. 输入事项到期日期 (YYYY-MM-DD)
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入事项到期日期 (YYYY-MM-DD):", L"新建事项")) {
        std::string eventDateStr = WideCharToUTF8(buf);
        size_t dash1 = eventDateStr.find('-');
        size_t dash2 = eventDateStr.find('-', dash1 + 1);
        if (dash1 != std::string::npos && dash2 != std::string::npos) {
            start_year_input = std::stoi(eventDateStr.substr(0, dash1));
            start_month_input = std::stoi(eventDateStr.substr(dash1 + 1, dash2 - dash1 - 1));
            start_day_input = std::stoi(eventDateStr.substr(dash2 + 1));
        }
    }
    else {
        MessageBox(NULL, _T("日期格式不正确！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }

    // 3. 输入事项到期时间 (HH:MM:SS)
    int event_hour, event_minute, event_second;
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入事项到期时间 (HH:MM:SS):", L"新建事项")) {
        std::string eventTimeStr = WideCharToUTF8(buf);
        size_t colon1 = eventTimeStr.find(':');
        size_t colon2 = eventTimeStr.find(':', colon1 + 1);
        if (colon1 != std::string::npos && colon2 != std::string::npos) {
            event_hour = std::stoi(eventTimeStr.substr(0, colon1));
            event_minute = std::stoi(eventTimeStr.substr(colon1 + 1, colon2 - colon1 - 1));
            event_second = std::stoi(eventTimeStr.substr(colon2 + 1));
        }
        else {
            MessageBox(NULL, _T("时间格式不正确！"), _T("错误"), MB_OK);
            return;
        }
    }


    // 4. 输入是否开启提醒（1: 是, 0: 否）
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"是否开启提醒? (1: 是, 0: 否):", L"新建事项")) {
        flag = std::stoi(WideCharToUTF8(buf)) == 1;
    }
    else {
        MessageBox(NULL, _T("标志设置不正确！"), _T("错误"), MB_OK);
        return;
    }

    // 5. 如果开启提醒，则输入提醒时长 (HH:MM:SS)，表示在截止时间前多少时间提醒
    Time remindTime;  // 默认提醒时长为 00:00:00
    if (flag) {
        if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入提前提醒时长 (HH:MM:SS):", L"新建事项")) {
            std::string remindTimeStr = WideCharToUTF8(buf);
            size_t colon1 = remindTimeStr.find(':');
            size_t colon2 = remindTimeStr.find(':', colon1 + 1);
            if (colon1 != std::string::npos && colon2 != std::string::npos) {
                int rHour = std::stoi(remindTimeStr.substr(0, colon1));
                int rMinute = std::stoi(remindTimeStr.substr(colon1 + 1, colon2 - colon1 - 1));
                int rSecond = std::stoi(remindTimeStr.substr(colon2 + 1));
                remindTime = Time(rHour, rMinute, rSecond);
            }
            else {
                MessageBox(NULL, _T("提前提醒时长格式不正确！"), _T("错误"), MB_OK);
                return;
            }
        }
    }

    // 调用服务层插入事项，将输入的事项名称、到期日期、到期时间、提醒标志和提醒时长传递给数据层
    Sv_Layer.insertEvent(eventNameInput,
        Date(start_year_input, start_month_input, start_day_input),
        Time(event_hour, event_minute, event_second),
        flag,
        remindTime);
}

// 修改事项界面
void ViewLayer::eventUpdateView(Event event) {
    wchar_t buf[256]; // 用于存储用户输入的宽字符缓冲区

    // 1. 修改事项名称（提示用户输入新的事项名称，如果用户取消，则保持原值）
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入修改后的事项名称:", ANSIToUnicode("修改事项：" + event.title).c_str())) {
        std::string newName = WideCharToUTF8(buf);
        if (!newName.empty()) {
            event.title = newName;
        }
    }

    // 2. 修改事项到期日期 (YYYY-MM-DD)
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入修改后的事项到期日期 (YYYY-MM-DD):", ANSIToUnicode("修改事项：" + event.title).c_str())) {
        std::string eventDateStr = WideCharToUTF8(buf);
        size_t dash1 = eventDateStr.find('-');
        size_t dash2 = eventDateStr.find('-', dash1 + 1);
        if (dash1 != std::string::npos && dash2 != std::string::npos) {
            int newYear = std::stoi(eventDateStr.substr(0, dash1));
            int newMonth = std::stoi(eventDateStr.substr(dash1 + 1, dash2 - dash1 - 1));
            int newDay = std::stoi(eventDateStr.substr(dash2 + 1));
            event.event_date = Date(newYear, newMonth, newDay);
        }
        else {
            MessageBox(NULL, _T("日期格式不正确！"), _T("错误"), MB_OK);
            return;
        }
    }
    else {
        return; // 用户取消输入时直接退出
    }

    // 3. 修改事项到期时间 (HH:MM:SS)
    int newHour, newMinute, newSecond;
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入修改后的事项到期时间 (HH:MM:SS):", ANSIToUnicode("修改事项：" + event.title).c_str())) {
        std::string eventTimeStr = WideCharToUTF8(buf);
        size_t colon1 = eventTimeStr.find(':');
        size_t colon2 = eventTimeStr.find(':', colon1 + 1);
        if (colon1 != std::string::npos && colon2 != std::string::npos) {
            newHour = std::stoi(eventTimeStr.substr(0, colon1));
            newMinute = std::stoi(eventTimeStr.substr(colon1 + 1, colon2 - colon1 - 1));
            newSecond = std::stoi(eventTimeStr.substr(colon2 + 1));
            event.event_time = Time(newHour, newMinute, newSecond);
        }
        else {
            MessageBox(NULL, _T("时间格式不正确！"), _T("错误"), MB_OK);
            return;
        }
    }
    else {
        return;
    }

    // 4. 修改是否开启提醒（1: 是, 0: 否）
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"是否开启提醒? (1: 是, 0: 否):", ANSIToUnicode("修改事项：" + event.title).c_str())) {
        bool newFlag = std::stoi(WideCharToUTF8(buf)) == 1;
        event.remind_flag = newFlag;
    }
    else {
        return;
    }

    // 5. 如果开启提醒，则修改提前提醒时长 (HH:MM:SS)
    if (event.remind_flag) {
        if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入修改后的提前提醒时长 (HH:MM:SS):", ANSIToUnicode("修改事项：" + event.title).c_str())) {
            std::string remindTimeStr = WideCharToUTF8(buf);
            size_t colon1 = remindTimeStr.find(':');
            size_t colon2 = remindTimeStr.find(':', colon1 + 1);
            if (colon1 != std::string::npos && colon2 != std::string::npos) {
                int rHour = std::stoi(remindTimeStr.substr(0, colon1));
                int rMinute = std::stoi(remindTimeStr.substr(colon1 + 1, colon2 - colon1 - 1));
                int rSecond = std::stoi(remindTimeStr.substr(colon2 + 1));
                event.remind_time = Time(rHour, rMinute, rSecond);
            }
            else {
                MessageBox(NULL, _T("提前提醒时长格式不正确！"), _T("错误"), MB_OK);
                return;
            }
        }
        else {
            return;
        }
    }

    // 调用 ServiceLayer 更新事项
    Sv_Layer.updateEvent(event.id, event.title, event.event_date, event.event_time, event.remind_flag, event.remind_time);
}

// 删除事项界面
void ViewLayer::eventDeleteView(Event event) {
    wchar_t buf[256];  // 用于存储用户输入的宽字符缓冲区

    // 检查是否已经删除
    if (event.is_deleted) {
        // 如果该事项已经被删除，直接返回
        return;
    }

    // 确认删除
    if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入是否删除（0为不删除，1为删除）:",
        ANSIToUnicode("删除事项：" + event.title).c_str())) {
        int flag = std::stoi(WideCharToUTF8(buf));  // 将 wchar_t* 转换为 int
        if (flag) {
            Sv_Layer.deleteEvent(event.id);  // 调用服务层删除事项
        }
    }
    else {
        MessageBox(NULL, _T("标志设置不正确！"), _T("错误"), MB_OK);
        return; // 用户取消输入
    }
}

void ViewLayer::pomodoroWorkView(MOUSEMSG& msg)
{

    // 消息处理
    if (msg.uMsg == WM_LBUTTONDOWN)
    {
        this->pomodoro_view.editing[0] = this->pomodoro_view.editing[1] = this->pomodoro_view.editing[2] = false;

        // 检测时间框点击，弹出输入框
        if (Sv_Layer.IfMouseHit(this->pomodoro_view.timeBoxes[0].x, this->pomodoro_view.timeBoxes[0].y, this->pomodoro_view.timeBoxes[0].w, this->pomodoro_view.timeBoxes[0].h, msg) && this->pomodoro_view.state != this->pomodoro_view.RUNNING) {
            this->pomodoro_view.editing[0] = true;
            TCHAR input[3] = _T("");
            if (InputBox(input, 3, _T("请输入小时 (0-23):"), _T("修改小时"), _T("00")) == IDOK) {
                int value = _ttoi(input);
                if (value >= 0 && value <= 23) _stprintf_s(this->pomodoro_view.hoursStr, _T("%02d"), value);
            }
        }
        else if (Sv_Layer.IfMouseHit(this->pomodoro_view.timeBoxes[1].x, this->pomodoro_view.timeBoxes[1].y, this->pomodoro_view.timeBoxes[1].w, this->pomodoro_view.timeBoxes[1].h, msg) && this->pomodoro_view.state != this->pomodoro_view.RUNNING) {
            this->pomodoro_view.editing[1] = true;
            TCHAR input[3] = _T("");
            if (InputBox(input, 3, _T("请输入分钟 (0-59):"), _T("修改分钟"), _T("00")) == IDOK) {
                int value = _ttoi(input);
                if (value >= 0 && value <= 59) _stprintf_s(this->pomodoro_view.minsStr, _T("%02d"), value);
            }
        }
        else if (Sv_Layer.IfMouseHit(this->pomodoro_view.timeBoxes[2].x, this->pomodoro_view.timeBoxes[2].y, this->pomodoro_view.timeBoxes[2].w, this->pomodoro_view.timeBoxes[2].h, msg) && this->pomodoro_view.state != this->pomodoro_view.RUNNING) {
            this->pomodoro_view.editing[2] = true;
            TCHAR input[3] = _T("");
            if (InputBox(input, 3, _T("请输入秒数 (0-59):"), _T("修改秒数"), _T("00")) == IDOK) {
                int value = _ttoi(input);
                if (value >= 0 && value <= 59) _stprintf_s(this->pomodoro_view.secsStr, _T("%02d"), value);
            }
        }

        // 控制按钮点击
        if (Sv_Layer.IfMouseHit(this->pomodoro_view.controlButtom.x, this->pomodoro_view.controlButtom.y, this->pomodoro_view.controlButtom.w, this->pomodoro_view.controlButtom.h, msg))
        {
            switch (this->pomodoro_view.state)
            {
            case this->pomodoro_view.IDLE:
                if (validateAndConvertTime(this->pomodoro_view.hoursStr, this->pomodoro_view.minsStr, this->pomodoro_view.secsStr, this->pomodoro_view.totalSeconds) && this->pomodoro_view.totalSeconds > 0)
                {
                    // 弹出输入框获取备注
                    TCHAR remarkBuffer[128] = _T("");
                    if (InputBox(remarkBuffer, 128, _T("请输入番茄钟备注:"), _T("番茄钟备注")) == IDOK)
                    {
                        // 番茄钟对象备注记录
                        this->pomodoro_view.completedPomo.record = WideCharToUTF8(remarkBuffer);

                        // 开始计时
                        this->pomodoro_view.timerStart = clock();
                        this->pomodoro_view.remainingSeconds = this->pomodoro_view.totalSeconds;
                        this->pomodoro_view.pomoStartTime = Sv_Layer.getCurrentTime().second;
                        this->pomodoro_view.state = this->pomodoro_view.RUNNING;
                        this->pomodoro_view.timeUpTriggered = false;
                    }
                }
                break;

            case this->pomodoro_view.RUNNING:
                this->pomodoro_view.pauseStart = clock(); // 记录暂停开始时间
                this->pomodoro_view.state = this->pomodoro_view.PAUSED;
                break;

            case this->pomodoro_view.PAUSED:
                this->pomodoro_view.timerStart += (clock() - this->pomodoro_view.pauseStart); // 调整开始时间以抵消暂停时长
                this->pomodoro_view.state = this->pomodoro_view.RUNNING;
                break;
            }
        }
    }

    // 状态更新
    if (this->pomodoro_view.state == this->pomodoro_view.RUNNING) {
        // 计算剩余时间
        int elapsed = static_cast<int>((clock() - this->pomodoro_view.timerStart) / CLOCKS_PER_SEC);
        int remaining = this->pomodoro_view.totalSeconds - elapsed;
        this->pomodoro_view.remainingSeconds = remaining;

        // 每秒更新显示
        static clock_t lastUpdate = 0;
        if (clock() - lastUpdate >= CLOCKS_PER_SEC) {
            formatTimeComponents(
                remaining,
                this->pomodoro_view.currentDisplayTime,
                this->pomodoro_view.hoursStr,
                this->pomodoro_view.minsStr,
                this->pomodoro_view.secsStr
            );
            lastUpdate = clock();
        }

        // 时间到处理
        if (remaining <= 0 && !this->pomodoro_view.timeUpTriggered) {
            this->pomodoro_view.timeUpTriggered = true;
            this->pomodoro_view.completedPomo.pomodoro_time = Time(
                this->pomodoro_view.totalSeconds / 3600,
                (this->pomodoro_view.totalSeconds % 3600) / 60,
                this->pomodoro_view.totalSeconds % 60
            );

            if (!Sv_Layer.pomodoroTick(this->pomodoro_view.completedPomo, this->pomodoro_view.pomoStartTime)) {
                this->pomodoro_view.state = this->pomodoro_view.IDLE;
                this->pomodoro_view.totalSeconds = 0;
                formatTimeComponents(
                    0,
                    this->pomodoro_view.currentDisplayTime,
                    this->pomodoro_view.hoursStr,
                    this->pomodoro_view.minsStr,
                    this->pomodoro_view.secsStr
                );
            }
        }
    }

    setlinestyle(PS_SOLID, 5);
    setlinecolor(RGB(255, 99, 71));
    circle(600, 400, 200);

    auto drawTimeBox = [](int x, int y, int h, int w, const TCHAR* text, bool editing) {
        setfillcolor(editing ? RGB(240, 240, 255) : WHITE);
        solidroundrect(x, y, x + h, y + w, 10, 10);
        setlinecolor(editing ? BLUE : DARKGRAY);
        setlinestyle(PS_SOLID, 2);
        roundrect(x, y, x + h, y + w, 10, 10);
        settextstyle(30, 0, _T("三极萌萌简体"));
        settextcolor(BLACK);
        setbkmode(TRANSPARENT);
        int tx = x + (h - textwidth(text)) / 2;
        int ty = y + (w - textheight(text)) / 2;
        outtextxy(tx, ty, text);
        };

    drawTimeBox(this->pomodoro_view.timeBoxes[0].x, this->pomodoro_view.timeBoxes[0].y, this->pomodoro_view.timeBoxes[0].h, this->pomodoro_view.timeBoxes[0].w, this->pomodoro_view.hoursStr, this->pomodoro_view.editing[0] && this->pomodoro_view.state != this->pomodoro_view.RUNNING);
    drawTimeBox(this->pomodoro_view.timeBoxes[1].x, this->pomodoro_view.timeBoxes[1].y, this->pomodoro_view.timeBoxes[1].h, this->pomodoro_view.timeBoxes[1].w, this->pomodoro_view.minsStr, this->pomodoro_view.editing[1] && this->pomodoro_view.state != this->pomodoro_view.RUNNING);
    drawTimeBox(this->pomodoro_view.timeBoxes[2].x, this->pomodoro_view.timeBoxes[2].y, this->pomodoro_view.timeBoxes[2].h, this->pomodoro_view.timeBoxes[2].w, this->pomodoro_view.secsStr, this->pomodoro_view.editing[2] && this->pomodoro_view.state != this->pomodoro_view.RUNNING);

    // 冒号
    settextstyle(40, 0, _T("三极萌萌简体"));
    settextcolor(BLACK);
    outtextxy(this->pomodoro_view.timeBoxes[0].x + (this->pomodoro_view.timeBoxes[1].x - this->pomodoro_view.timeBoxes[0].x) * 0.6, this->pomodoro_view.timeBoxes[0].y, _T(":"));
    outtextxy(this->pomodoro_view.timeBoxes[1].x + (this->pomodoro_view.timeBoxes[2].x - this->pomodoro_view.timeBoxes[1].x) * 0.6, this->pomodoro_view.timeBoxes[1].y, _T(":"));

    // 控制按钮
    setfillcolor(RGB(255, 99, 71));
    solidroundrect(this->pomodoro_view.controlButtom.x, this->pomodoro_view.controlButtom.y, this->pomodoro_view.controlButtom.x + this->pomodoro_view.controlButtom.w, this->pomodoro_view.controlButtom.y + this->pomodoro_view.controlButtom.h, 15, 15);

    // 动态按钮文字
    const TCHAR* btnText = (this->pomodoro_view.state == this->pomodoro_view.RUNNING) ? _T("暂停") : (this->pomodoro_view.state == this->pomodoro_view.PAUSED) ? _T("继续") : _T("开始");
    settextstyle(30, 0, _T("三极萌萌简体"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);

    // 计算文字宽高
    int textW = textwidth(btnText);
    int textH = textheight(btnText);

    // 计算文字绘制起点，使其居中
    int textX = this->pomodoro_view.controlButtom.x + (this->pomodoro_view.controlButtom.w - textW) / 2;
    int textY = this->pomodoro_view.controlButtom.y + (this->pomodoro_view.controlButtom.h - textH) / 2;

    //在界面右上角绘制“退出”按钮
    returnButtomView(170, 50, msg);

    // 绘制文字
    outtextxy(textX, textY, btnText);
}



// 日历视图界面
void ViewLayer::calendarView(MOUSEMSG& msg) {
    settextstyle(40, 0, _T("三极萌萌简体"));
    returnButtomView(200, 50, msg);
}

void ViewLayer::timelineView(MOUSEMSG& msg) {
    settextstyle(40, 0, _T("三极萌萌简体"));

    wchar_t buf[1024];
    static bool dateSelected = false; // 标志变量，表示用户是否已经选择了日期

    // 在界面左上角绘制“选择日期”按钮
    int ChooseButtonWidth = 150;
    int ChooseButtonHeight = 50;
    int ChooseButtonX = 0;
    int ChooseButtonY = 0;
    DrawButton(ChooseButtonX, ChooseButtonY, ChooseButtonWidth, ChooseButtonHeight, "选择日期", RGB(255, 255, 204), RGB(121, 238, 227));

    // 检查用户是否点击了“选择日期”按钮
    if (Sv_Layer.IfMouseHit(ChooseButtonX, ChooseButtonY, ChooseButtonWidth, ChooseButtonHeight, msg)) {
        if (InputBox(buf, sizeof(buf) / sizeof(wchar_t), L"请输入想要查看的日期(YYYY-MM-DD):", L"选择日期")) {
            std::string targetdate = WideCharToUTF8(buf);
            size_t dash1 = targetdate.find('-');
            size_t dash2 = targetdate.find('-', dash1 + 1);
            if (dash1 != std::string::npos && dash2 != std::string::npos) {
                start_year_input = std::stoi(targetdate.substr(0, dash1));
                start_month_input = std::stoi(targetdate.substr(dash1 + 1, dash2 - dash1 - 1));
                start_day_input = std::stoi(targetdate.substr(dash2 + 1));
                dateSelected = true; // 用户已选择日期
            }
            else {
                MessageBox(NULL, _T("日期格式不正确！"), _T("错误"), MB_OK);
                return;
            }
        }
        else {
            return; // 用户取消输入
        }
    }

    // 如果用户没有选择日期，则使用当前日期
    if (!dateSelected) {
        // 获取当前时间
        auto currentTime = Sv_Layer.getCurrentTime();
        start_year_input = currentTime.first.year;
        start_month_input = currentTime.first.month;
        start_day_input = currentTime.first.day;
    }

    // 默认显示当前日期
    std::wstring datetext = ANSIToUnicode(std::to_string(start_year_input) + " 年 "
        + std::to_string(start_month_input) + " 月 " + std::to_string(start_day_input) + " 日 "
    );

    // 在界面右上角绘制“返回”按钮
    returnButtomView(170, 50, msg);

    // 绘制时间线
    settextstyle(60, 0, _T("三极萌萌简体"));
    datetext = ANSIToUnicode(std::to_string(start_year_input) + " 年 "
        + std::to_string(start_month_input) + " 月 " + std::to_string(start_day_input) + " 日 "
    );
    outtextxy(600 - textwidth(datetext.c_str()) / 2, 10, datetext.c_str());
    settextstyle(40, 0, _T("三极萌萌简体"));
    DateRecord d = Sv_Layer.getAllRecordbyDate(Date(start_year_input, start_month_input, start_day_input));

    // 使用优先队列按照时间排序
    std::priority_queue < std::pair<Time, Habit>, std::vector<std::pair<Time, Habit>>,
        decltype([](const std::pair<Time, Habit>& a, const std::pair<Time, Habit>& b)
            { return a.first > b.first; }) > habitQueue;

    std::priority_queue < std::pair<Time, Pomodoro>, std::vector<std::pair<Time, Pomodoro>>,
        decltype([](const std::pair<Time, Pomodoro>& a, const std::pair<Time, Pomodoro>& b)
            { return a.first > b.first; }) > pomodoroQueue;

    // 将 Habit_Record 插入优先队列
    for (const auto& habit : d.Habit_Record) {
        habitQueue.push(habit);
    }

    // 将 Pomodoro_Record 插入优先队列
    for (const auto& pomodoro : d.Pomodoro_Record) {
        pomodoroQueue.push(pomodoro);
    }

    // 设置时间线起始位置（调整到按钮齐平）
    int centerX = 600;   // 时间线的中心 x 坐标
    int startY = 80;     // 初始 Y 坐标，向上调整，与按钮对齐
    int spacing = 80;    // 记录之间的间隔

    // 上一个绘制点的位置
    int lastX = centerX;
    int lastY = startY;

    // 设置线条样式
    setlinestyle(PS_SOLID, 8);

    // 控制左右交替
    bool leftSide = true;

    // 开始绘制时间线
    while (!habitQueue.empty() || !pomodoroQueue.empty()) {
        // 获取时间最小的记录
        bool isHabit = false;
        if (!habitQueue.empty() && (pomodoroQueue.empty() || habitQueue.top().first < pomodoroQueue.top().first)) {
            isHabit = true;
        }

        // 计算当前点的 X 坐标，左右交替绘制
        int offsetX = 100;  // 左右偏移量
        int currentX = leftSide ? (centerX - offsetX) : (centerX + offsetX);
        int currentY = lastY + spacing;

        line(lastX, lastY, currentX, currentY); // 直接连线

        // 绘制数据点（小圆圈）
        setfillcolor(RGB(255, 102, 102));
        fillcircle(currentX, currentY, 10);

        std::wstring text;
        // 绘制文本
        if (isHabit) {
            auto habit = habitQueue.top();
            text = ANSIToUnicode((habit.first.to_String() + habit.second.name + " 打卡"
                ));
            habitQueue.pop();
            settextcolor(RGB(66, 99, 31));
        }
        else {
            auto pomodoro = pomodoroQueue.top();
            std::string hourtext = pomodoro.second.pomodoro_time.hour == 0 ? "" :
                std::to_string(pomodoro.second.pomodoro_time.hour) + "小时";

            std::string minutetext = pomodoro.second.pomodoro_time.minute == 0 ? "" :
                std::to_string(pomodoro.second.pomodoro_time.minute) + "分";

            std::string secondtext = pomodoro.second.pomodoro_time.second == 0 ? "" :
                std::to_string(pomodoro.second.pomodoro_time.second) + "秒";
            text = ANSIToUnicode((pomodoro.first.to_String() + "  进行  " + pomodoro.second.record + "  "
                + hourtext + minutetext + secondtext));
            pomodoroQueue.pop();
            settextcolor(RGB(160, 126, 234));
        }

        int textWidth = textwidth(text.c_str());
        int rectWidth = textWidth + 20; // 矩形框宽度稍大于文本
        int rectHeight = 50;  // 矩形框高度
        int rectX = leftSide ? (currentX - rectWidth - 30) : (currentX + 30);  // 左侧或右侧矩形框
        int rectY = currentY - rectHeight / 2;

        setfillcolor(RGB(248, 233, 230));
        // 绘制矩形框
        solidrectangle(rectX, rectY, rectX + rectWidth, rectY + rectHeight);

        // 绘制文本
        outtextxy(rectX + 10, rectY + 10, text.c_str());


        // 更新上一个点的位置
        lastX = currentX;
        lastY = currentY;
        leftSide = !leftSide;  // 切换左右方向
    }
    setlinestyle(PS_SOLID, 1);
}

void ViewLayer::DrawButton(int x, int y, int w, int h, const std::string& o, COLORREF backgroundColor, COLORREF textColor)
{
    std::wstring text = ANSIToUnicode(o);

    // 1. 设置按钮背景颜色
    setfillcolor(backgroundColor); // 使用传入的背景颜色
    solidrectangle(x, y, x + w, y + h); // 绘制实心矩形

    // 2. 设置按钮边框颜色，稍微调暗背景颜色
    int r = GetRValue(backgroundColor);
    int g = GetGValue(backgroundColor);
    int b = GetBValue(backgroundColor);

    // 边框颜色比背景颜色深
    COLORREF borderColor = RGB(r - 20 <= 0 ? 0 : r - 20, g - 20 <= 0 ? 0 : g - 20 , b - 20 <= 0 ? 0 : b - 20);

    setlinecolor(borderColor); // 设置边框颜色
    rectangle(x, y, x + w, y + h); // 绘制矩形边框

    // 3. 设置文本颜色
    settextcolor(textColor); // 黑色文本
    setbkmode(TRANSPARENT); // 设置文本背景透明

    // 4. 计算文本居中位置
    int textWidth = textwidth(text.c_str()); // 获取文本宽度
    int textHeight = textheight(text.c_str()); // 获取文本高度
    int textX = x + (w - textWidth) / 2; // 文本水平居中
    int textY = y + (h - textHeight) / 2; // 文本垂直居中

    // 5. 绘制文本
    outtextxy(textX, textY, text.c_str());
}


std::string ViewLayer::WideCharToUTF8(const wchar_t* wideStr) {
    if (!wideStr) return "";

    // 获取转换后的 UTF-8 字符串长度
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0) return "";

    // 分配缓冲区
    std::string utf8Str(bufferSize, 0);

    // 执行转换
    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &utf8Str[0], bufferSize, nullptr, nullptr);

    // 去除末尾的空字符
    utf8Str.pop_back();
    return utf8Str;
}
std::wstring ViewLayer::ANSIToUnicode(const std::string& str)
{
    int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &result[0], size);
    return result;
}

// 导航栏界面
void ViewLayer::navigationView(MOUSEMSG &msg) {

    settextstyle(70, 0, _T("三极萌萌简体"));

    setfillcolor(RGB(250, 255, 240));
    IMAGE img;

    loadimage(&img, L"pic.png", 900, 900);
    putimagePng(300, 0,&img);
    // 导航栏背景
    setfillcolor(RGB(255, 255,134));
    solidrectangle(0, 0, 300, getheight());

    settextcolor(RGB(255, 0, 127));
    outtextxy(70,10 , L"Habbit");
    settextstyle(60, 0, _T("三极萌萌简体"));
    outtextxy(5, 60, L"效率管理软件");

    settextstyle(50, 0, _T("三极萌萌简体"));
    // 定义按钮的坐标和尺寸
    int buttonWidth = 200;  // 按钮宽度
    int buttonHeight = 100;  // 按钮高度
    int buttonSpacing = 50; // 按钮间距
    int startX = 50;        // 按钮起始 x 坐标
    int startY = 150;        // 按钮起始 y 坐标

    // 定义按钮文字
    std::string buttonTexts[5] = {
        "事项管理",
        "习惯管理",
        "番茄钟",
        "时间线",
        "日历"
    };

    // 绘制按钮
    for (int i = 0; i < 5; i++) {
        int buttonY = startY + i * (buttonHeight + buttonSpacing);
        DrawButton(startX, buttonY, buttonWidth, buttonHeight, buttonTexts[i], RGB(255, 255, 204), RGB(121, 238, 227));
        if (Sv_Layer.IfMouseHit(startX, buttonY, buttonWidth, buttonHeight, msg))
        {
            switch (i) {
            case 0: setcurrentview(EVENT_MANAGE_VIEW); return; // 跳转到事项管理界面
            case 1: setcurrentview(HABIT_MANAGE_VIEW); return; // 跳转到习惯管理界面
            case 2: setcurrentview(POMODORO_VIEW); return; // 跳转到番茄钟界面
            case 3: setcurrentview(TIMELINE_VIEW); return; // 跳转到时间线界面
            case 4:setcurrentview(CALENDAR_VIEW); return; // 跳转到日历界面
            }
        }
    }
}

void ViewLayer::returnButtomView(int ExitButtonWidth, int ExitButtonHeight, MOUSEMSG& msg)
{
    int ExitButtonX = 1200 - ExitButtonWidth;
    int ExitButtonY = 0;

    DrawButton(ExitButtonX, ExitButtonY, ExitButtonWidth, ExitButtonHeight, "返回主界面", RGB(255, 255, 204), RGB(121, 238, 227));
    if (Sv_Layer.IfMouseHit(ExitButtonX, ExitButtonY, ExitButtonWidth, ExitButtonHeight, msg)) {
        setcurrentview(NAVIGATION_VIEW);
        setlinestyle(PS_SOLID, 1);
    }
}

void ViewLayer::Init()
{
    Sv_Layer.init();
    curviewtype = NAVIGATION_VIEW;
}

void ViewLayer::draw()
{
    MOUSEMSG msg = Sv_Layer.db_layer.getmouse();
    BeginBatchDraw();
    // 根据 currentView 显示对应的界面
    switch (curviewtype) {
    case NAVIGATION_VIEW:
        setbkcolor(RGB(250,255,240));
        cleardevice();
        navigationView(msg);
        break;
    case HABIT_MANAGE_VIEW:
        setbkcolor(RGB(250, 255, 240));
        cleardevice();
        habitManageView(msg);
        break;
    case EVENT_MANAGE_VIEW:
        setbkcolor(RGB(250, 255, 240));
        cleardevice();
        eventManageView(msg);
        break;
    case POMODORO_VIEW:
        setbkcolor(RGB(250, 255, 240));
        cleardevice();
        pomodoroWorkView(msg);
        break;
    case TIMELINE_VIEW:
        setbkcolor(RGB(250, 255, 240));
        cleardevice();
        timelineView(msg);
        break;
    case CALENDAR_VIEW:
        setbkcolor(RGB(250, 255, 240));
        cleardevice();
        calendarView(msg);
        break;
    }


    FlushBatchDraw();
}

void ViewLayer::setcurrentview(ViewType v)
{
    curviewtype = v;
}


void ViewLayer::handleTimeInput(TCHAR* buffer, int maxLen, TCHAR ch, int maxValue)
{
    size_t len = _tcslen(buffer);

    // 退格处理
    if (ch == _T('\b')) {
        if (len > 0) buffer[len - 1] = _T('\0');
        return;
    }

    // 仅允许数字输入
    if (!_istdigit(ch)) return;

    // 临时验证缓冲区
    TCHAR temp[3] = { 0 };
    _tcscpy_s(temp, buffer);
    temp[len] = ch;

    // 验证数值范围
    int value = _ttoi(temp);
    if (value > maxValue) return;

    // 更新缓冲区
    if (len < static_cast<size_t>(maxLen) - 1) {
        buffer[len] = ch;
        buffer[len + 1] = _T('\0');
    }
}


// 更新总秒数
bool ViewLayer::updateTotalSeconds(const TCHAR* hours, const TCHAR* mins, const TCHAR* secs, Time& time, int& total)
{
    int h = _ttoi(hours);
    int m = _ttoi(mins);
    int s = _ttoi(secs);

    if (h > 23 || m > 59 || s > 59) return false;

    time.hour = h;
    time.minute = m;
    time.second = s;

    total = h * 3600 + m * 60 + s;
    return total > 0;
}

// 时间格式化
void ViewLayer::formatTimeComponents(int seconds, Time& time, TCHAR* hours, TCHAR* mins, TCHAR* secs)
{
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;

    time.hour = h;
    time.minute = m;
    time.second = s;

    _stprintf_s(hours, 3, _T("%02d"), h);
    _stprintf_s(mins, 3, _T("%02d"), m);
    _stprintf_s(secs, 3, _T("%02d"), s);
}

bool ViewLayer::validateAndConvertTime(const TCHAR* hours, const TCHAR* mins,
    const TCHAR* secs, int& total)
{
    int h = _ttoi(hours);
    int m = _ttoi(mins);
    int s = _ttoi(secs);

    if (h > 23 || m > 59 || s > 59) return false;

    total = h * 3600 + m * 60 + s;
    return total > 0;
}
void ViewLayer::putimagePng(int img_x, int img_y, IMAGE* pSrcImg)
{
    DWORD* dst = GetImageBuffer();
    DWORD* src = GetImageBuffer(pSrcImg);
    int src_width = pSrcImg->getwidth();
    int src_height = pSrcImg->getheight();
    int dst_width = getwidth();
    int dst_height = getheight();

    int im_width, im_height;
    if (img_x + src_width > dst_width)
        im_width = dst_width - img_x;
    else
        im_width = src_width;
    if (img_y + src_height > dst_height)
        im_height = dst_height - img_y;
    else
        im_height = src_height;

    if (img_x < 0)
    {
        src = src - img_x;
        im_width = im_width + img_x;
        img_x = 0;
    }
    if (img_y < 0)
    {
        src = src - src_width * img_y;
        im_height = im_height + img_y;
        img_y = 0;
    }
    dst = dst + dst_width * img_y + img_x;

    for (int i = 0; i < im_height; i++)
    {
        for (int j = 0; j < im_width; j++)
        {
            int src_r = ((src[j] & 0xff0000) >> 16);
            int src_g = ((src[j] & 0xff00) >> 8);
            int src_b = src[j] & 0xff;
            int src_a = ((src[j] & 0xff000000) >> 24);
            int dst_r = ((dst[j] & 0xff0000) >> 16);
            int dst_g = ((dst[j] & 0xff00) >> 8);
            int dst_b = dst[j] & 0xff;
            dst[j] = ((src_r + dst_r * (255 - src_a) / 255) << 16)
                | ((src_g + dst_g * (255 - src_a) / 255) << 8)
                | (src_b + dst_b * (255 - src_a) / 255);
        }
        dst = dst + dst_width;
        src = src + src_width;
    }
}

ViewLayer::PomodoroView::PomodoroView()
{
    this->currentDisplayTime = Time{ 0, 0, 0 };
}