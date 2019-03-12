#include "journal_model.hpp"

#include <journal/journal.hpp>
#include <common/local_time.hpp>

#include <QDebug>

namespace barmaley::gui {

JournalModel::JournalModel(lib::JournalManagerView& journalManagerView)
    : journal(journalManagerView.journal), order(journal.keysView())
{
    connectionsContainer += journalManagerView.onAdded([this](lib::JournalEntryId key) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        order.push_back(key);
        endInsertRows();
    });
}

JournalModel::~JournalModel() = default;
int JournalModel::columnCount(const QModelIndex&) const
{
    return 3;
}

int JournalModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(order.size());
}
QVariant JournalModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    auto key          = order[index.row()];
    const auto& value = journal.value(key);

    switch (index.column()) {
        case TypeColumn::DATE: {
            return QString::fromStdString(lib::localtime(value->createdDate, "%d.%m.%y %T"));
        }

        case TypeColumn::TYPE:
            try {
                return QString::fromStdString(toString(value->type));
            } catch (const std::logic_error& e) {
                qWarning() << e.what();
                return "NONE";
            }
        case TypeColumn::TEXT:
            return QString::fromStdString(value->text());
        default:
            return QVariant();
    }
}

QVariant JournalModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Orientation::Horizontal || role != Qt::DisplayRole) {
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    switch (section) {
        case TypeColumn::DATE:
            return "DATE";
        case TypeColumn::TYPE:
            return "TYPE";
        case TypeColumn::TEXT:
            return "TEXT";
        default:
            return QVariant();
    }
}
} // namespace barmaley::gui
