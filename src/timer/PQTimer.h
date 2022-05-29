#ifndef _TIMER_H
#define _TIMER_H

#include <signal.h>
#include <time.h>
#include <cstdint>
#include <memory>
#include <queue>
#include <vector>

#include "../utils/log/Log.h"
#include "IClock.h"
#include "ITimer.h"

namespace faker_tsn {

class PQTimer : public ITimer {
  private:
    std::priority_queue<
        std::shared_ptr<Ticker>,
        std::vector<std::shared_ptr<Ticker>>,
        std::greater<std::shared_ptr<Ticker>>>
        m_tickers;                   /* ticker heap */
    timer_t m_timerHandle;           /* timer handle */
    struct sigevent m_evp;           /* signal event struct */
    struct timespec m_spec;          /* time specification struct */
    struct itimerspec m_timeValue;   /* initial timer specification struct */
    struct sigaction m_sigAction;    /* signal action struct */
    std::shared_ptr<IClock> m_clock; /* clock */

    friend void onAlarm(int signo);

  public:
    PQTimer();

    virtual ~PQTimer() override;

    /* enable timer */
    virtual void start() override;

    /* stop timer */
    virtual void stop() override;

    /* set timer */
    virtual void setTimer(std::shared_ptr<Ticker>& ticker);

    /* get ticker */
    virtual std::shared_ptr<Ticker> getTicker() override;

    /* get ticker by id */
    virtual std::shared_ptr<Ticker> getTicker(unsigned long id) override;

    /* add ticker */
    virtual void addTicker(std::shared_ptr<Ticker>& ticker) override;

    /* remove ticker */
    virtual void removeTicker() override;

    /* remove ticker by id*/
    virtual void removeTicker(unsigned long id) override;

    void showTickers() {
        INFO("No. of tickes = " + std::to_string(this->m_tickers.size()));
        while (!this->m_tickers.empty()) {
            INFO(this->m_tickers.top()->toString());
            this->m_tickers.pop();
        }
    }
};

}  // namespace faker_tsn

#endif  // _TIMER_H