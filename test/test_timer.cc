#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <memory.h>
#include <time.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "../src/timer/ITimer.h"

using namespace std;
using namespace faker_tsn;

static void TestClockGetTime() {
    struct timespec ts;
    memset(&ts, 0, sizeof(ts));

    clock_gettime(CLOCK_REALTIME, &ts);
    cout << "[CLOCK_REALTIME] ";
    cout << "sec:" << ts.tv_sec << " nsec:" << ts.tv_nsec << endl;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    cout << "[CLOCK_MONOTONIC] ";
    cout << "sec:" << ts.tv_sec << " nsec:" << ts.tv_nsec << endl;
}

static void TestTimeClass() {
    Time::TimePoint timePoint;
    timePoint.setNow();
    INFO(timePoint.toString());

    Time::TimeInterval timeInterval1(1, 0);
    timePoint = timePoint + timeInterval1;
    INFO(timePoint.toString());

    Time::TimeInterval timeInterval2(0, 800000000);
    timePoint = timePoint - timeInterval2;
    INFO(timePoint.toString());

    Time::TimePoint _timePoint1;
    _timePoint1.setNow();
    INFO(_timePoint1.toString());

    // sleep 2.8s
    chrono::nanoseconds sleepTime(2800000000);
    this_thread::sleep_for(sleepTime);  // millisecond deviation
    cout << "sleep 2.8 s" << endl;

    Time::TimePoint _timePoint2;
    _timePoint2.setNow();
    INFO(_timePoint2.toString());
    Time::TimeInterval timeInterval3 = _timePoint2 - _timePoint1;
    INFO(timeInterval3.toString());
}

int sec = 0;
int nsec = 120000;

Time::TimePoint g_timePoint1;
int g_flag1 = 1;
static void HandlerSignal(int signo) {
    INFO("ON SIGNAL");
    Time::TimePoint timePoint;
    timePoint.setNow();
    Time::TimeInterval interval = timePoint - g_timePoint1;
    INFO(interval.toString());
    g_flag1 = 0;
}

Time::TimePoint g_timePoint2;
int g_flag2 = 1;
static void HandlerThread(union sigval) {
    INFO("ON THREAD");
    Time::TimePoint timePoint;
    timePoint.setNow();
    Time::TimeInterval interval = timePoint - g_timePoint2;
    INFO(interval.toString());
    g_flag2 = 0;
}

static void TestTimerSetTimeSignal() {
    /* create timer */
    timer_t timer;
    struct sigevent evp;
    memset(&evp, 0, sizeof(evp));
    /* use signal */
    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGRTMAX - 1;
    signal(SIGRTMAX - 1, HandlerSignal);
    if (timer_create(CLOCK_REALTIME, &evp, &timer) == -1) {
        ERROR("create timer failed");
    }

    /* set timer */
    Time::TimePoint timePoint;
    timePoint.setNow();
    g_timePoint1 = timePoint;
    Time::TimeInterval interval(sec, nsec);
    timePoint = timePoint + interval;

    struct itimerspec its;
    memset(&its, 0, sizeof(its));
    its.it_value.tv_sec = timePoint.sec;
    its.it_value.tv_nsec = timePoint.nsec;

    if (timer_settime(timer, TIMER_ABSTIME, &its, NULL) == -1) {
        ERROR("set timer failed");
    }

    while (g_flag1) {
        // INFO("...");
    }
}

static void TestTimerSetTimeThread() {
    /* create timer */
    timer_t timer;
    struct sigevent evp;
    memset(&evp, 0, sizeof(evp));
    /* use thread */
    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = HandlerThread;
    evp.sigev_notify_attributes = NULL;
    if (timer_create(CLOCK_REALTIME, &evp, &timer) == -1) {
        ERROR("create timer failed");
    }

    /* set timer */
    Time::TimePoint timePoint;
    timePoint.setNow();
    g_timePoint2 = timePoint;
    Time::TimeInterval interval(sec, nsec);
    timePoint = timePoint + interval;

    struct itimerspec its;
    memset(&its, 0, sizeof(its));
    its.it_value.tv_sec = timePoint.sec;
    its.it_value.tv_nsec = timePoint.nsec;

    if (timer_settime(timer, TIMER_ABSTIME, &its, NULL) == -1) {
        ERROR("set timer failed");
    }

    while (g_flag2) {
        // INFO("...");
    }
}

static void TestPQTimer() {
    PQTimer timer;
    IClock* clock = Reflector::getNewInstance<IClock>("RealTimeClock");
    INFO(clock->now().toString() + "\n");

    // ticker1
    Time::TimePoint start = clock->now();
    Time::TimeInterval expire = Time::TimeInterval(4, 0);  // +4s
    Time::TimeInterval period(0, 0);
    INFO("Start Time = " + start.toString());
    INFO("Expire Time = " + expire.toString());
    INFO("Period Time = " + period.toString());
    std::shared_ptr<Ticker> ticker1 = std::make_shared<Ticker>(start, expire, period);
    INFO("Ticker ID: " + std::to_string(ticker1->getId()));  // 其属性 id 每创建一个自动加 1
    INFO("is periodic: " + std::to_string(ticker1->isPeriodic()) + "\n");

    // ticker2
    Time::TimeInterval expire2 = Time::TimeInterval(2, 800000000);  // +2.8s
    INFO("Start Time = " + start.toString());
    INFO("Expire Time = " + expire2.toString());
    INFO("Period Time = " + period.toString());
    std::shared_ptr<Ticker> ticker2 = std::make_shared<Ticker>(start, expire2, period);
    INFO("Ticker ID: " + std::to_string(ticker2->getId()));
    INFO("is periodic: " + std::to_string(ticker2->isPeriodic()) + "\n");

    timer.addTicker(ticker1);
    timer.addTicker(ticker2);
    timer.start();

    // while (true) {
    // }

    timer.stop();
}

static void TestTimeContext() {
    TimeContext::getInstance().getTimer();
}

TEST(TEST_TIMER, TEST_CLOCK_GET_TIME) {
    TestClockGetTime();
}

TEST(TEST_TIMER, TIME_CLASS) {
    TestTimeClass();
}

TEST(TEST_TIMER, TEST_TIMER_SET_TIME) {
    /* real-time signal is better than thread */
    TestTimerSetTimeSignal();
    TestTimerSetTimeThread();
}

TEST(TEST_TIMER, TEST_PQTIMER) {
    TestPQTimer();
}

TEST(TEST_TIMER, TEST_TIME_CONTEXT) {
    TestTimeContext();
}