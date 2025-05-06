#ifndef VIEWLAYER_H
#define VIEWLAYER_H

#include "ServiceLayer.h"
#include<string>
#include<time.h>

enum ViewType {
    NAVIGATION_VIEW,
    HABIT_MANAGE_VIEW,
    EVENT_MANAGE_VIEW,
    POMODORO_VIEW,
    TIMELINE_VIEW,
    CALENDAR_VIEW
};

class ViewLayer
{
private:


    struct ButtomPos
    {
        int x;
        int y;
        int w;
        int h;

        ButtomPos(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    };

    class PomodoroView {

        ServiceLayer Sv_Layer;
        // 按钮区域
        ButtomPos timeBoxes[3] = { {470, 300, 60, 40}, {580, 300, 60, 40}, {690, 300, 60, 40} };
        ButtomPos controlButtom = { 500, 450, 200, 60 };

        // 状态枚举
        enum PomodoroState { IDLE, RUNNING, PAUSED };
        PomodoroState state = IDLE;

        // 时间参数
        TCHAR hoursStr[3] = _T("00");
        TCHAR minsStr[3] = _T("00");
        TCHAR secsStr[3] = _T("00");
        Time currentDisplayTime;           // 界面显示时间
        int totalSeconds = 0;              // 总倒计时秒数
        int remainingSeconds = 0;          // 剩余秒数
        clock_t timerStart = 0;            // 计时器开始时间
        clock_t pauseStart = 0;            // 暂停开始时间
        bool editing[3] = { false };       // 编辑状态 [小时, 分钟, 秒]

        // 番茄钟记录相关
        Time pomoStartTime;                // 番茄钟实际开始时间
        bool timeUpTriggered = false;      // 防止重复触发
        Pomodoro completedPomo{ 0 };

        MOUSEMSG msg;

        friend class ViewLayer;
    public:
        PomodoroView();
    };

    /*
    负责人: 十七
    功能: DrawButton: 绘制按钮
    参数：
        x   左上角x坐标
        y   左上角y坐标
        w   宽度
        h   高度
        name 按钮名称
    参数: void
    返回值: void
    */
    void DrawButton(int x, int y, int w, int h, const std::string& o, COLORREF backgroundColor, COLORREF);



    // 在当前设备上绘制带透明通道的png图片
    // img_x	绘制位置的x坐标
    // img_y	绘制位置的y坐标
    // pSrcImg	要绘制的IMAGE对象指针
    void putimagePng(int img_x, int img_y, IMAGE* pSrcImg);


    //将ANST字符（string）转换为Unicode对象
    std::wstring ANSIToUnicode(const std::string& str);

    //将宽字符转为UTF8
    std::string WideCharToUTF8(const wchar_t* wideStr);

    ServiceLayer Sv_Layer;

    //当前显示的界面
    ViewType curviewtype;

    //存储输入结果的变量
    std::string eventNameInput;     //event name
    std::string habitNameInput;     // 习惯名称
    int habitTargetCountInput = 0;  // 目标打卡次数
    int start_year_input = 0;       // 开始年份
    int start_month_input = 0;      // 开始月份
    int start_day_input = 0;        // 开始日期
    int end_year_input = 0;         // 结束年份
    int end_month_input = 0;        // 结束月份
    int end_day_input = 0;          // 结束日期
    bool flag;

    PomodoroView pomodoro_view;
public:
    /*
    负责人: 十七
    功能: habitManageView: 习惯管理主界面
    调用数据层的getactivehabit()函数获取活跃习惯列表
    调用数据层的getactivehabit()函数获取非活跃习惯列表
        显示活跃中的习惯
            习惯上有修改按钮
            习惯上有删除按钮
            习惯上有打卡按钮
        添加习惯
        显示已停用的习惯
            习惯上有修改按钮
            习惯上有删除按钮
            习惯上有重新启用按钮
    参数: void
    返回值: void
    */
    void habitManageView(MOUSEMSG&);
    /*
    负责人: 十七
    功能: habitInsertView: 新建习惯界面
    调用服务层gethabitIDMax()函数来获取目前事项表中的最大id值
    在这个id的基础上+1就是我们要创建的habit对象的对应ID，
    然后与其他用户传入的数据封装成habit对象
        调用新建习惯函数
            填写习惯名称
            开始日期
            结束如期
            每日打卡次数(默认 1)
        确定/取消
    参数: void
    返回值: void
    */
    void habitInsertView();
    /*
    负责人: 十七
    功能: habitUpdateView: 修改习惯界面
    调用服务层函数updateEvent()并传递用户输入的数据
        调用修改习惯函数
            修改开始日期
            结束日期
            每日打卡次数
            是否启用习惯
        确定/取消
    参数: 对应点开的habit的ID，用来在数据库中进行操作
    返回值: void
    */
    void habitUpdateView(Habit);
    /*
    负责人: 十七
    功能: habitDeleteView: 删除习惯界面
    调用服务层deleteHabit()函数，传递需要删除的习惯主键
        调用删除习惯函数
        确定/取消
    参数: 对应点开的habit的ID，用来在数据库中进行操作
    返回值: void
    */
    void habitDeleteView(Habit);

    // 事项管理界面
    /*
    负责人: XTUG
    功能: eventManageView: 事项管理主界面
    调用服务层getactiveevent()函数和getExpiredevent()函数
    分别打印活跃事项和过期事项
        显示活跃中的事项
            显示倒计时
            事项上有修改按钮
            事项上有删除按钮
        添加事项
        显示已过期的事项
            事项上有删除按钮
    参数: void
    返回值: void
    */
    void eventManageView(MOUSEMSG&);

    /*
    负责人: XTUG
    功能: eventInsertView: 新建事项界面
    调用服务层geteventIDMax()函数来获取目前事项表中的最大id值
    在这个id的基础上+1就是我们要创建的event对象的对应ID，
    然后与其他用户传入的数据封装成event对象
        调用新建事项函数
            填写事项名称
            日期
            时间
            是否开启提醒
            提前提醒的时间
        确定/取消
    参数: void
    返回值: Event 新建的事项
    */
    void eventInsertView();

    /*
    负责人: XTUG
    功能: eventUpdateView: 修改事项界面
    调用服务层函数updateEvent()并传递用户输入的数据
    通过
        调用修改事项函数
            修改事项日期
            事项时间
            是否开启提醒
            提前提醒的时间
        确定/取消
    参数: eventID，即点开的事项对应的ID，用来在数据库中查找对应的数据项
    返回值: Event 修改后的事项
    */
    void eventUpdateView(Event event);

    /*
    负责人: XTUG
    功能: eventDeleteView: 删除事项界面
    调用服务层函数Deleteevent()函数
        调用删除事项函数
        确定/取消
    参数: eventID，即点开的事项对应的ID，用来在数据库中查找对应的数据项
    返回值: void
    */
    void eventDeleteView(Event event);

    // 番茄钟界面
    /*
    负责人: YY
    功能: pomodoroWorkView: 番茄钟计时界面
    这个界面输入番茄钟的倒计时时间和番茄钟记录名称，然后点击开始按钮会开始计时
    将数据和倒计时的时间封装成pomodoro对象
    调用tick()函数来判断倒计时是否结束
    时间会不断减少，然后这段时间开始按钮会变成暂停按钮，如果点击可以停止倒计时
    如果倒计时结束，那么按钮会变成确认按钮，点击则会清除目前已输入的内容
        开始后
            调用番茄钟计时函数
            显示剩余时间
            暂停按钮
    参数: void
    返回值: void
    */
    void pomodoroWorkView(MOUSEMSG&);


    // 日历界面
    /*
    负责人: YY
    功能: calendarView: 日历视图界面
    首先通过让用户输入一个月份，封装成Date对象
    调用服务层函数GetHabitRecordbyDate(Date)来获取
    对应那一月份每一天的对应习惯打卡记录，并显示在日历表格中
        调用日历函数
        日历 每日打卡数
        日历 月份显示/切换
        时间线入口
    参数: void
    返回值: void
    */
    void calendarView(MOUSEMSG&);

    // 时间线界面
    /*
    负责人: YY
    要求用户输入一个日期，封装成Date对象
    调用GetAllRecordbyDate(Date)，获取那一天的记录
    输出vector中的记录
    功能: timelineView: 时间线界面
        调用时间线函数
        时间线 当日的习惯、事项、番茄钟事件 事件顺序排列
        时间线 日期显示/切换
        日历视图入口
    参数: void
    返回值: void
    */
    void timelineView(MOUSEMSG&);

    /*
    * 负责人: 十七
     * 导航栏
     */
    void navigationView(MOUSEMSG&);

    void returnButtomView(int ExitButtonWidth, int ExitButtonHeight, MOUSEMSG& msg);

    void Init();

    void draw();

    void setcurrentview(ViewType);

    // 处理时间输入
    void handleTimeInput(TCHAR* buffer, int maxLen, TCHAR ch, int maxValue);

    // 更新总秒数
    bool updateTotalSeconds(const TCHAR* hours, const TCHAR* mins, const TCHAR* secs, Time& time, int& total);

    // 时间格式化
    void formatTimeComponents(int seconds, Time& time, TCHAR* hours, TCHAR* mins, TCHAR* secs);


    // 验证、转换时间
    bool validateAndConvertTime(const TCHAR* hours, const TCHAR* mins,
        const TCHAR* secs, int& total);
};

#endif // VIEWLAYER_H4

