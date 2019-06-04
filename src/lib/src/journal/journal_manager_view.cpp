#include <journal/journal_manager_view.hpp>

namespace barmaley::lib {
JournalManagerView::JournalManagerView(const Journal& journal) : journal(journal) {}
JournalManagerView::~JournalManagerView() = default;
s2::connection JournalManagerView::onAdded(SignalType::slot_type slot)
{
    return itemAddedEvent.connect(std::move(slot));
}
} // namespace barmaley::lib
