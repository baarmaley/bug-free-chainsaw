#include <journal/journal.hpp>

#include <current_state_model/current_state_model.hpp>

namespace barmaley::lib {
std::vector<const JournalEntry*> Journal::valuesView() const
{
    std::vector<const JournalEntry*> result;
    for (const auto& [key, value] : items) {
        result.push_back(&value);
    }
    return result;
}
std::vector<JournalEntryId> Journal::keysView() const
{
    std::vector<JournalEntryId> result;
    for (const auto& [key, value] : items) {
        result.push_back(key);
    }
    return result;
}
const JournalEntry* Journal::value(JournalEntryId id) const
{
    auto it = items.find(id);
    if (it == items.end()) {
        return nullptr;
    }
    return &it->second;
}
} // namespace barmaley::lib
