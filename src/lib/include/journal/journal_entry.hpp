#pragma once

#include <journal/journal_entry_id.hpp>

#include <common/common_type.hpp>

#include <chrono>
#include <string>

namespace barmaley {
namespace lib {

enum class JournalEntryType
{
    device_detected,
    connection_lost,
    connection_restored,
    reconnect_detected,
    bad_protocol,
    ip_changed,
    reboot_detect,
    successful_request,
    failed_request
};

std::string toString(JournalEntryType type);

class JournalEntry
{
public:
    explicit JournalEntry(JournalEntryId id, JournalEntryType type, std::string text)
        : id(id), type(type), text(std::move(text))
    {
    }
    ~JournalEntry() {}

    JournalEntryId id;
    JournalEntryType type;
    std::chrono::system_clock::time_point createdDate = std::chrono::system_clock::now();
    std::string text;
};
} // namespace lib
} // namespace barmaley
