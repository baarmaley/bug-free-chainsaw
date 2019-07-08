#pragma once

#include <common/signals2.hpp>

#include <journal/journal_entry_id.hpp>
#include <journal/journal_fwd.hpp>
#include <journal/journal_manager_view_fwd.hpp>

#include <QAbstractListModel>

#include <vector>
namespace barmaley::gui {
class JournalModel : public QAbstractListModel
{
public:
    JournalModel(lib::JournalManagerView& journalManagerView);
    ~JournalModel();

    enum RolesNames
    {
        date = Qt::UserRole,
        type,
        text,
    };
    int rowCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int) const override;

protected:
	QHash<int, QByteArray> roleNames() const override;

private:
    const lib::Journal& journal;
    QHash<int, QByteArray> m_roleNames;
    std::vector<lib::JournalEntryId> order;
    lib::ConnectionsContainer connectionsContainer;
};

} // namespace barmaley::gui
