#pragma once

#include <journal/journal_entry_id.hpp>
#include <journal/journal_fwd.hpp>
#include <journal/journal_manager_fwd.hpp>

#include <common/signals2.hpp>

namespace barmaley::lib {
class JournalManagerView
{
public:
    using SignalType = s2::signal<void(JournalEntryId)>;

    JournalManagerView(const Journal& journal);
    JournalManagerView(const JournalManagerView&) = delete;
    JournalManagerView(JournalManagerView&&)      = delete;
    ~JournalManagerView();

    friend JournalManager;

    const Journal& journal;

    s2::connection onAdded(SignalType::slot_type slot);

protected:
    SignalType itemAddedEvent;
};
} // namespace barmaley::lib
