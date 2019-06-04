#include <journal/journal_entry.hpp>

namespace barmaley {
namespace lib {
std::string toString(JournalEntryType type)
{
    switch (type) {
        case JournalEntryType::device_detected:
            return "device_detected";
        case JournalEntryType::connection_lost:
            return "connection_lost";
        case JournalEntryType::connection_restored:
            return "connection_restored";
        case JournalEntryType::reconnect_detected:
            return "reconnect_detected";
        case JournalEntryType::bad_protocol:
            return "bad_protocol";
        case JournalEntryType::ip_changed:
            return "ip_changed";
        case JournalEntryType::reboot_detect:
            return "reboot_detect";
        case JournalEntryType::successful_request:
            return "successful_request";
        case JournalEntryType::failed_request:
            return "failed_request";
        default:
            throw std::logic_error("JournalEntryType no conversions available");
    }
}
} // namespace lib
} // namespace barmaley
