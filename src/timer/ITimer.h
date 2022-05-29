#ifndef _I_TIMER_H
#define _I_TIMER_H

#include <string.h>
#include <time.h>
#include <iostream>
#include <memory>
#include <sstream>

#include "../utils/reflector/Reflector.h"

namespace faker_tsn {

class Time {
  public:
    /* time unit not matched exception */
    class TimeUnitNotMatchedException : public std::exception {
      private:
        const char* m_errorTimeUnit;

      public:
        TimeUnitNotMatchedException(const char* timeUnit)
            : m_errorTimeUnit(timeUnit) {
        }

        const char* what() const noexcept {
            std::stringstream ss;
            ss << "TimeUnit: " << this->m_errorTimeUnit << " is not matched";
            return ss.str().c_str();
        }
    };

    /* time interval */
    class TimeInterval {
      public:
        long sec = 0;
        long nsec = 0;

      public:
        TimeInterval() = default;

        TimeInterval(long sec, long nsec) {
            this->sec = sec;
            this->nsec = nsec;
        }

        TimeInterval(const TimeInterval& interval) {
            this->sec = interval.sec;
            this->nsec = interval.nsec;
        }

        TimeInterval operator=(const TimeInterval& interval) {
            this->sec = interval.sec;
            this->nsec = interval.nsec;
            return *this;
        }

        std::string toString() {
            std::string info;
            info += "Sec: " + std::to_string(this->sec);
            info += ", ";
            info += "NSec: " + std::to_string(this->nsec);
            return info;
        }
    };

    /* time point */
    class TimePoint {
      public:
        long sec = 0;
        long nsec = 0;

      public:
        TimePoint() = default;

        TimePoint(long sec, long nsec) {
            this->sec = sec; /*  */
            this->nsec = nsec;
        }

        /* sychronize time point with era time  */
        void setNow() {
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            this->sec = ts.tv_sec;
            this->nsec = ts.tv_nsec;
        }

        friend TimePoint operator+(const TimePoint& timePoint, const TimeInterval& interval) {
            Time::TimePoint _tp;
            _tp.nsec = timePoint.nsec + interval.nsec;
            _tp.sec = timePoint.sec + interval.sec;

            if (_tp.nsec >= 1000000000) {
                _tp.sec += 1;
                _tp.nsec -= 1000000000;
            }
            return _tp;
        }

        friend TimePoint operator-(const TimePoint& timePoint, const TimeInterval& interval) {
            Time::TimePoint _tp;
            long _t = timePoint.nsec - interval.nsec;
            if (_t >= 0) {
                _tp.sec = timePoint.sec - interval.sec;
                _tp.nsec = _t;
            } else {
                _tp.sec = timePoint.sec - interval.sec - 1;
                _tp.nsec = 1000000000 + _t;
            }
            return _tp;
        }

        friend TimeInterval operator-(const TimePoint& timePoint1, const TimePoint& timePoint2) {
            Time::TimeInterval _itv;
            long _t = timePoint1.nsec - timePoint2.nsec;
            if (_t >= 0) {
                _itv.sec = timePoint1.sec - timePoint2.sec;
                _itv.nsec = _t;
            } else {
                _itv.sec = timePoint1.sec - timePoint2.sec - 1;
                _itv.nsec = 1000000000 + _t;
            }
            return _itv;
        }

        friend bool operator<(const TimePoint& timePoint1, const TimePoint& timePoint2) {
            if (timePoint1.sec < timePoint2.sec) {
                return true;
            } else if ((timePoint1.sec == timePoint2.sec) && (timePoint1.nsec < timePoint2.nsec)) {
                return true;
            } else {
                return false;
            }
        }

        friend bool operator>=(const TimePoint& timePoint1, const TimePoint& timePoint2) {
            if (timePoint1.sec < timePoint2.sec) {
                return false;
            } else if ((timePoint1.sec == timePoint2.sec) && (timePoint1.nsec < timePoint2.nsec)) {
                return false;
            } else {
                return true;
            }
        }

        TimePoint& operator=(const TimePoint& timePoint) {
            if (this != &timePoint) {
                this->sec = timePoint.sec;
                this->nsec = timePoint.nsec;
            }
            return *this;
        }

        TimePoint& operator+=(const TimeInterval& timeInterval) {
            this->sec += timeInterval.sec;
            this->nsec += timeInterval.nsec;
            return *this;
        }

        std::string toString() {
            std::stringstream ss;
            ss << "Sec: ";
            ss << this->sec;
            ss << " NSec: ";
            ss << this->nsec;
            return ss.str();
        }
    };

    static TimeInterval converIntegerToTimeInterval(long long interval, const char* timeUnit) {
        long sec = 0;
        long nsec = 0;
        if (!strcmp(timeUnit, "s")) {
            sec = interval;
        } else if (!strcmp(timeUnit, "ms")) {
            sec = interval / 1000;
            interval -= sec * 1000;
            nsec = interval * 1000000;
        } else if (!strcmp(timeUnit, "us")) {
            sec = interval / 1000000;
            interval -= sec * 1000000;
            nsec = interval * 1000;
        } else if (!strcmp(timeUnit, "ns")) {
            sec = interval / 1000000000;
            interval -= sec * 1000000000;
            nsec = interval;
        } else {
            throw TimeUnitNotMatchedException(timeUnit);
        }
        return TimeInterval(sec, nsec);
    }

    static TimePoint converIntegerToTimePoint(long long time, const char* timeUnit) {
        long sec = 0;
        long nsec = 0;
        if (strcmp(timeUnit, "s")) {
            sec = time;
        } else if (strcmp(timeUnit, "ms")) {
            sec = time / 1000;
            time -= sec * 1000;
            nsec = time * 1000000;
        } else if (strcmp(timeUnit, "us")) {
            sec = time / 1000000;
            time -= sec * 1000000;
            nsec = time * 1000;
        } else if (strcmp(timeUnit, "ns")) {
            sec = time / 1000000000;
            time -= sec * 1000000000;
            nsec = time;
        } else {
            throw TimeUnitNotMatchedException(timeUnit);
        }
        return TimePoint(sec, nsec);
    }
};

class Ticker {
  private:
    static unsigned long s_id; /* ticker id */
    bool m_isPeriodic;         /* periodic flag */
    Time::TimePoint start;     /* start time */
    Time::TimeInterval expire; /* expire time */
    Time::TimeInterval period; /* period time */
  public:
    Ticker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period) {
        Ticker::s_id++;
        (period.sec == 0 && period.nsec == 0) ? this->m_isPeriodic = false : this->m_isPeriodic = true;
        this->start = start;
        this->expire = expire;
        this->period = period;
    }

    /* functor needed to be rewrite by user */
    virtual void operator()(){};

    /* overload operator < */
    friend bool operator<(const Ticker& ticker1, const Ticker& ticker2) {
        if (ticker1.start < ticker2.start) {
            return true;
        } else {
            return false;
        }
    }

    inline unsigned long getId() {
        return Ticker::s_id;
    }

    inline bool isPeriodic() {
        return this->m_isPeriodic;
    }

    inline void setExpire(Time::TimeInterval expire) {
        this->expire = expire;
    }

    inline void setStart(Time::TimePoint start) {
        this->start = start;
    }

    inline void setPeriod(Time::TimeInterval period) {
        this->period = period;
    }

    inline Time::TimeInterval getExpire() const {
        return this->expire;
    };

    inline Time::TimePoint getStart() const {
        return this->start;
    }

    inline Time::TimeInterval getPeriod() const {
        return this->period;
    }

    std::string toString() {
        std::stringstream ss;
        ss << "StartTime" << this->start.toString() << " ";
        ss << "ExpireTime" << this->expire.toString() << " ";
        ss << "PeriodTime" << this->period.toString();
        return ss.str();
    }
};

class ITimer : public ReflectObject {
  public:
    virtual ~ITimer() = default;

    /* enable timer */
    virtual void start() = 0;

    /* stop timer */
    virtual void stop() = 0;

    /* get ticker */
    virtual std::shared_ptr<Ticker> getTicker() = 0;

    /* get ticker by id */
    virtual std::shared_ptr<Ticker> getTicker(unsigned long id) = 0;

    /* add ticker */
    virtual void addTicker(std::shared_ptr<Ticker>& ticker) = 0;

    /* remove ticker */
    virtual void removeTicker() = 0;

    /* remove ticker by id*/
    virtual void removeTicker(unsigned long id) = 0;
};

}  // namespace faker_tsn

#endif  // _I_TIMER_H