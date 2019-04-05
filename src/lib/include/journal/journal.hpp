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
    using JournalType = std::unordered_map<JournalEntryId, std::unique_ptr<JournalEntry>>;

    template<typename T, typename... Args>
    auto createEntry(JournalEntryId id, Args&&... args)
    {
        return items.emplace(id, std::make_unique<T>(id, std::forward<Args>(args)...));
    }

    std::vector<JournalEntry*> valuesView() const
    {
        std::vector<JournalEntry*> result;
        for (const auto& [key, value] : items) {
            result.push_back(value.get());
        }
        return result;
    }

    std::vector<JournalEntryId> keysView() const
    {
        std::vector<JournalEntryId> result;
        for (const auto& [key, value] : items) {
            result.push_back(key);
        }
        return result;
    }

    const JournalType& values() const
    {
        return items;
    }

    const JournalEntry* value(JournalEntryId id) const
    {
        auto it = items.find(id);
        if (it == items.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    JournalType items;
};

struct LastState
{
	LastState(const CurrentState& currentState);

    std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();
    std::string ip;
    std::uint32_t heap;
    std::chrono::seconds uptime;
    std::chrono::seconds connectionDuration;

    bool connectionLost = false;
};
class JournalManager;
struct AddedItemGuard;

class JournalManagerView
{
public:
    using SignalType = s2::signal<void(JournalEntryId)>;

    JournalManagerView(const Journal& journal);
    JournalManagerView(const JournalManagerView&) = delete;
    JournalManagerView(JournalManagerView&&)      = delete;
    ~JournalManagerView();

    friend JournalManager;
    friend AddedItemGuard;

    const Journal& journal;

    s2::connection onAdded(SignalType::slot_type slot);

protected:
    SignalType itemAddedEvent;
};

struct AddedItemGuard
{
    AddedItemGuard(JournalManagerView& view, JournalEntryId id) : view(view), id(id) {}

    ~AddedItemGuard()
    {
        view.itemAddedEvent(id);
    }

    JournalManagerView& view;
    JournalEntryId id;
};

class JournalManager
{
public:
    JournalManager(FacadeView& facadeView, CurrentStateModelView& currentStateModelView);

    inline JournalManagerView& view()
    {
        return journalManagerView;
    }

private:
    inline AddedItemGuard beginAddedEntry()
    {
        counterJEI = static_cast<JournalEntryId>(static_cast<std::uint64_t>(counterJEI) + 1);

        return AddedItemGuard(journalManagerView, counterJEI);
    }

    JournalEntryId counterJEI{0};
    Journal journal;
    JournalManagerView journalManagerView{journal};
    std::chrono::seconds intervalUdpdate{10};
    Timer udateTimer;
    ConnectionsContainer connectionsContainer;
    std::unordered_map<DeviceId, LastState> lastStates;
	std::unordered_map<std::string, std::string> recentBadProtocolEvents;
};

} // namespace barmaley::lib
