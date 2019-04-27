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

enum class RelayId : std::uint32_t
{
};

constexpr RelayId toRelayId(std::uint32_t v)
{
    return static_cast<RelayId>(v);
}

constexpr std::uint32_t toUint32(RelayId v)
{
    return static_cast<std::uint32_t>(v);
}

enum class RelayStatus
{
    INVALID,
    ON,
    OFF,
};

NLOHMANN_JSON_SERIALIZE_ENUM(RelayStatus,
                             {
                                 {RelayStatus::INVALID, nullptr},
                                 {RelayStatus::OFF, 0},
                                 {RelayStatus::ON, 1},
                             })
// RelayStatus toRelayStatus(int value);

struct Relay
{
    RelayId id{0};
    RelayStatus status{RelayStatus::OFF};
    std::string name;
};

inline bool operator==(const Relay& lhs, const Relay& rhs)
{
    return lhs.id == rhs.id && lhs.status == rhs.status && lhs.name == rhs.name;
}

struct WifiInfo
{
    std::chrono::seconds connectionDuration;
    std::uint32_t reconnectCount{0};
    int rssi{0};
    std::optional<std::string> lastReasonReconnection;
};

struct GpioInfo
{
    std::string lastChanged;
};

struct Status
{
    std::chrono::system_clock::time_point receivedDate;

    DeviceId id = DeviceId{0};
    std::string deviceType;
    std::string name;
    std::vector<Relay> relays;
    std::chrono::seconds uptime;
    std::uint32_t heap;
    WifiInfo wifiInfo;
    GpioInfo gpioInfo;

    inline std::chrono::system_clock::time_point uptimeTp() const
    {
        return receivedDate - uptime;
    }

    inline std::chrono::system_clock::time_point connectionTp() const
    {
        return receivedDate - wifiInfo.connectionDuration;
    }
};

void from_json(const json& j, Relay& r);
void from_json(const json& j, WifiInfo& w);
void from_json(const json& j, GpioInfo& g);
void from_json(const json& j, Status& s);

} // namespace barmaley::lib
