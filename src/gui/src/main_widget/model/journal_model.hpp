#pragma once

#include <common/signals2.hpp>

#include <journal/journal_entry_id.hpp>
#include <journal/journal_fwd.hpp>
#include <journal/journal_manager_view_fwd.hpp>

#include <QAbstractTableModel>

#include <vector>
namespace barmaley::gui {
class JournalModel : public QAbstractTableModel
{
public:
    JournalModel(lib::JournalManagerView& journalManagerView);
    ~JournalModel();

    enum TypeColumn : std::uint32_t
    {
        DATE = 0,
        TYPE,
        TEXT,
    };

    int columnCount(const QModelIndex& = QModelIndex()) const override;
    int rowCount(const QModelIndex& = QModelIndex()) const override;

    QVariant data(const QModelIndex&, int) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    const lib::Journal& journal;
    std::vector<lib::JournalEntryId> order;
    lib::ConnectionsContainer connectionsContainer;
};

} // namespace barmaley::gui
