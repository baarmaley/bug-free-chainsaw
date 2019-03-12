#pragma once

#include <common/common_type.hpp>

#include <chrono>
#include <string>

namespace barmaley {
namespace lib {

enum class JournalEntryType
{
    DEVICE_DETECTED,
    CONNECTION_LOST,
    CONNECTION_RESTORED,
    RECONNECT_DETECTED,
    BAD_PROTOCOL,
    IP_CHANGED,
    REBOOT_DETECT,
};

std::string toString(JournalEntryType type);

enum class JournalEntryId : std::uint64_t
{
};

class JournalEntry
{
public:
    explicit JournalEntry(JournalEntryId id, JournalEntryType type) : id(id), type(type) {}
    virtual ~JournalEntry() {}

    JournalEntryId id;
    JournalEntryType type;
    std::chrono::system_clock::time_point createdDate = std::chrono::system_clock::now();

    virtual std::string text() const
    {
        return "";
    };
};

class DeviceDetectedJE : public JournalEntry
{
public:
    explicit DeviceDetectedJE(JournalEntryId id, DeviceId deviceId, std::string ip)
        : JournalEntry(id, JournalEntryType::DEVICE_DETECTED), deviceId(deviceId), ip(ip)
    {
    }
    DeviceId deviceId;
    std::string ip;

    std::string text() const override
    {
        return "id: " + std::to_string(toUint32(deviceId)) + " ip: " + ip;
    }
};

class ConnectionLost : public JournalEntry
{
public:
    explicit ConnectionLost(JournalEntryId id, DeviceId deviceId)
        : JournalEntry(id, JournalEntryType::CONNECTION_LOST), deviceId(deviceId)
    {
    }
    DeviceId deviceId;

    std::string text() const override
    {
        return "id: " + std::to_string(toUint32(deviceId));
    }
};

class ConnectionRestoredJE : public JournalEntry
{
public:
    explicit ConnectionRestoredJE(JournalEntryId id, DeviceId deviceId)
        : JournalEntry(id, JournalEntryType::CONNECTION_RESTORED), deviceId(deviceId)
    {
    }
    DeviceId deviceId;

    std::string text() const override
    {
        return "id: " + std::to_string(toUint32(deviceId));
    }
};

class ReconnectDetectedJE : public JournalEntry
{
public:
    explicit ReconnectDetectedJE(JournalEntryId id,
                                 DeviceId deviceId,
                                 std::uint32_t reconnectCount,
                                 std::string lastReason)
        : JournalEntry(id, JournalEntryType::RECONNECT_DETECTED),
          deviceId(deviceId),
          reconnectCount(reconnectCount),
          lastReason(std::move(lastReason))
    {
    }
    DeviceId deviceId;
    std::uint32_t reconnectCount;
    std::string lastReason;

    std::string text() const override
    {
        return "id: " + std::to_string(toUint32(deviceId)) + " reconnectCount: " + std::to_string(reconnectCount) +
               " lastReason: " + lastReason;
    }
};

class BadProtocolJE : public JournalEntry
{
public:
    explicit BadProtocolJE(JournalEntryId id, std::string ip, std::string error)
        : JournalEntry(id, JournalEntryType::BAD_PROTOCOL), ip(std::move(ip)), error(std::move(error))
    {
    }
    std::string ip;
    std::string error;

    std::string text() const override
    {
        return "ip: " + ip + " error: " + error;
    }
};

class IpChangedJE : public JournalEntry
{
public:
    explicit IpChangedJE(JournalEntryId id, DeviceId deviceId, std::string oldIp, std::string newIp)
        : JournalEntry(id, JournalEntryType::IP_CHANGED),
          deviceId(deviceId),
          oldIp(std::move(oldIp)),
          newIp(std::move(newIp))
    {
    }
    DeviceId deviceId;
    std::string oldIp;
    std::string newIp;

    std::string text() const override
    {
        return "id: " + std::to_string(toUint32(deviceId)) + " old ip: " + oldIp + " newIp: " + newIp;
    }
};

class RebootDetectJE : public JournalEntry
{
public:
    explicit RebootDetectJE(JournalEntryId id, DeviceId deviceId)
        : JournalEntry(id, JournalEntryType::REBOOT_DETECT), deviceId(deviceId)
    {
    }
    DeviceId deviceId;

    std::string text() const override
    {
        return "id: " + std::to_string(toUint32(deviceId));
    }
};

} // namespace lib
} // namespace barmaley
