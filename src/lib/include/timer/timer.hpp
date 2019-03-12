#pragma once

#include <chrono>

#include <common/context.hpp>
#include <common/signals2.hpp>

namespace barmaley {
namespace lib {
class Timer
{
public:
    using SignalType = s2::signal<void()>;

    Timer(std::chrono::milliseconds interval);

    void start();

    s2::connection onTimeout(SignalType::slot_type slot);

private:
    void restart();

    std::chrono::milliseconds interval;
    bool isRunning = false;
    SignalType timeoutEvent;
    Context<Timer> ctx{this};
};
} // namespace lib
} // namespace barmaley
