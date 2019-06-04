#pragma once

#include <common/common_type.hpp>
#include <common/signals2.hpp>
#include <current_state_model/current_state_model_fwd.hpp>
#include <facade/facade_fwd.hpp>
#include <journal/journal_entry.hpp>
#include <timer/timer.hpp>

#include <memory>

namespace barmaley::lib {

struct Journal
{
    using JournalType = std::unordered_map<JournalEntryId, JournalEntry>;
	
    std::vector<const JournalEntry*> valuesView() const;

    std::vector<JournalEntryId> keysView() const;
    
    const JournalType& values() const
    {
        return items;
    }

    const JournalEntry* value(JournalEntryId id) const;
    
    JournalType items;
};

} // namespace barmaley::lib
