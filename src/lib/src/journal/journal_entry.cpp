#include <journal/journal_entry.hpp>

namespace barmaley {
namespace lib {
std::string toString(JournalEntryType type)
{
    switch (type) {
        case JournalEntryType::DEVICE_DETECTED:
            return "DEVICE_DETECTED";
        case JournalEntryType::CONNECTION_LOST:
            return "CONNECTION_LOST";
        case JournalEntryType::CONNECTION_RESTORED:
            return "CONNECTION_RESTORED";
        case JournalEntryType::RECONNECT_DETECTED:
            return "RECONNECT_DETECTED";
        case JournalEntryType::BAD_PROTOCOL:
            return "BAD_PROTOCOL";
        case JournalEntryType::IP_CHANGED:
            return "IP_CHANGED";
        case JournalEntryType::REBOOT_DETECT:
            return "REBOOT_DETECT";
        default:
            throw std::logic_error("JournalEntryType no conversions available");
    }
}
} // namespace lib
} // namespace barmaley
