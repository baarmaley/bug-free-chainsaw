#pragma once

#include <chrono>
#include <optional>
#include <string>

#include <common/json.hpp>

namespace barmaley::lib {
enum class DeviceId : std::uint32_t
{
};

constexpr DeviceId toDeviceId(std::uint32_t v)
{
    return static_cast<DeviceId>(v);
}

constexpr std::uint32_t toUint32(DeviceId v)
{
    return static_cast<std::uint32_t>(v);
}

struct Debug
{
    std::chrono::system_clock::time_point receivedDate;
    std::string lastChanged;
    std::chrono::seconds uptime;
    std::uint32_t heap;
    std::chrono::seconds connectionDuration;
    std::uint32_t reconnectCount;
    int rssi;
    std::optional<std::string> lastReasonReconnection;

    inline std::chrono::system_clock::time_point uptimeTp() const
    {
        return receivedDate - uptime;
    }

    inline std::chrono::system_clock::time_point connectionTp() const
    {
        return receivedDate - connectionDuration;
    }
};

using DebugOpt = std::optional<Debug>;

struct State
{
    DeviceId id = DeviceId{0};
    std::string deviceType;
    std::string state;
};

void from_json(const json& j, State& s);
void from_json(const json& j, Debug& d);

} // namespace barmaley::lib
