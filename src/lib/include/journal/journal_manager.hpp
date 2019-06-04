#pragma once

#include <journal/journal.hpp>
#include <journal/journal_manager_view.hpp>

namespace barmaley::lib {
class JournalManager
{
public:
    JournalManager();
    JournalManager(JournalManager&&)      = delete;
    JournalManager(const JournalManager&) = delete;

    inline JournalManagerView& view()
    {
        return journalManagerView;
    }

    void addEntry(JournalEntryType type, std::string msg);

private:
    JournalEntryId counter{0};
    Journal journal;
    JournalManagerView journalManagerView{journal};
    ConnectionsContainer connectionsContainer;
};
} // namespace barmaley::lib
