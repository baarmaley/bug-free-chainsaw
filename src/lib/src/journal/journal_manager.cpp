#include <journal/journal_manager.hpp>

namespace barmaley::lib {
JournalManager::JournalManager() = default;

void JournalManager::addEntry(JournalEntryType type, std::string msg)
{
    journal.items.emplace(
        std::piecewise_construct, std::forward_as_tuple(counter), std::forward_as_tuple(counter, type, std::move(msg)));
    journalManagerView.itemAddedEvent(counter);
    counter = static_cast<JournalEntryId>(static_cast<std::uint64_t>(counter) + 1);
}
} // namespace barmaley::lib
