#include "journal_model.hpp"

#include <common/local_time.hpp>
#include <journal/journal.hpp>
#include <journal/journal_entry.hpp>
#include <journal/journal_manager_view.hpp>

#include <QDebug>

namespace barmaley::gui {

JournalModel::JournalModel(lib::JournalManagerView& journalManagerView)
    : journal(journalManagerView.journal), order(journal.keysView())
{
    m_roleNames[date] = "date";
    m_roleNames[type] = "type";
    m_roleNames[text] = "text";

    connectionsContainer += journalManagerView.onAdded([this](lib::JournalEntryId key) {
        beginInsertRows(QModelIndex(), 0, 0);
        order.push_back(key);
        endInsertRows();
    });
}

JournalModel::~JournalModel() = default;

int JournalModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(order.size());
}
QVariant JournalModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role != Qt::DisplayRole && !m_roleNames.contains(role)) {
        return QVariant();
    }

    auto key          = order.rbegin() + index.row();
    const auto& value = journal.value(*key);

    switch (role) {
        case Qt::DisplayRole:
            return "Journal entry";
        case date: {
            return QString::fromStdString(lib::localtime(value->createdDate, "%d.%m.%y %T"));
        }
        case type:
            try {
                return QString::fromStdString(toString(value->type));
            } catch (const std::logic_error& e) {
                qWarning() << e.what();
                return "NONE";
            }
        case text:
            return QString::fromStdString(value->text);
        default:
            return QVariant();
    }
}
QHash<int, QByteArray> JournalModel::roleNames() const
{
    return m_roleNames;
}
} // namespace barmaley::gui
