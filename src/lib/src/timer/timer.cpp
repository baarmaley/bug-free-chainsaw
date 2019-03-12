#include <timer/timer.hpp>

#include <QTimer>

namespace barmaley {
namespace lib {
Timer::Timer(std::chrono::milliseconds interval) : interval(interval) {}

void Timer::start()
{
    if (isRunning) {
        return;
    }
    isRunning = true;

    restart();
}

s2::connection Timer::onTimeout(SignalType::slot_type slot)
{
    return timeoutEvent.connect(std::move(slot));
}
void Timer::restart()
{
    QTimer::singleShot(interval, [self = ctx.ptr()] {
        auto ptr = self.lock();
        if (ptr) {
            (*ptr)->timeoutEvent();
            (*ptr)->restart();
        }
    });
}
} // namespace lib
} // namespace barmaley
