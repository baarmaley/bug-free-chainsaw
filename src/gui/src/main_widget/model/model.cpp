#include "model.hpp"

#include <iomanip>
#include <sstream>

#include <common/local_time.hpp>
#include <current_state_model/current_state_model.hpp>

namespace barmaley::gui {

namespace {
QString toLocalTime(const std::chrono::system_clock::time_point tp)
{
    return QString::fromStdString(lib::localtime(tp, "%d.%m.%y %T"));
}

} // namespace

Model::Model(lib::CurrentStateModelView& modelView) : currentStateModel(modelView.currentStateModel)
{
    for (auto [key, value] : this->currentStateModel.values()) {
        order.push_back(key);
    }
    connectionsContainer += modelView.onAdded([this](lib::DeviceId key) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        order.push_back(key);
        endInsertRows();
    });
    connectionsContainer += modelView.onUpdate([this](lib::DeviceId key) {
        auto itOrder  = std::find(order.begin(), order.end(), key);
        auto rowIndex = std::distance(order.begin(), itOrder);
        dataChanged(index(rowIndex, 0), index(rowIndex, columnCount()));
    });
}

int Model::columnCount(const QModelIndex&) const
{
    return 13;
}

int Model::rowCount(const QModelIndex&) const
{
    return static_cast<int>(order.size());
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    auto key          = order[index.row()];
    const auto& value = *currentStateModel.value(key);
    const auto& state = value.state;
    const auto& debug = value.debug;

    switch (index.column()) {
        case TypeColumn::IP:
            return QString::fromStdString(value.ip);
        case TypeColumn::PACKETS_RECEIVED:
            return value.numberOfPacketsReceived;
        case TypeColumn::LAST_UPDATE:
            return toLocalTime(value.lastUpdate);
        case TypeColumn::ID:
            return QString::number(toUint32(value.state.id));
        case TypeColumn::DEVICE_TYPE:
            return QString::fromStdString(state.deviceType);
        case TypeColumn::STATE:
            return QString::fromStdString(state.state);
        case TypeColumn::LAST_CHANGED:
            return debug ? QString::fromStdString(debug->lastChanged) : QStringLiteral("-");
        case TypeColumn::UPTIME:
            return debug ? toLocalTime(debug->uptime) : QStringLiteral("-");
        case TypeColumn::HEAP:
            return debug ? debug->heap : QVariant(QStringLiteral("-"));
        case TypeColumn::CONNECTION_TIMEPOINT:
            return debug ? toLocalTime(debug->connectionTimepoint) : QStringLiteral("-");
        case TypeColumn::RECONNECT_COUNT:
            return debug ? debug->reconnectCount : QVariant(QStringLiteral("-"));
        case TypeColumn::LAST_REASON_RECONNECTION:
            return (debug && debug->lastReasonReconnection) ? QString::fromStdString(*debug->lastReasonReconnection)
                                                            : QStringLiteral("-");
        case TypeColumn::RSSI:
            return debug ? debug->rssi : QVariant(QStringLiteral("-"));
        default:
            return QVariant();
    }
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Orientation::Horizontal || role != Qt::DisplayRole) {
        return QAbstractTableModel::headerData(section, orientation, role);
    }
    switch (section) {
        case TypeColumn::IP:
            return "IP";
        case TypeColumn::PACKETS_RECEIVED:
            return "PACKETS_RECEIVED";
        case TypeColumn::LAST_UPDATE:
            return "LAST_UPDATE";
        case TypeColumn::ID:
            return "ID";
        case TypeColumn::DEVICE_TYPE:
            return "DEVICE_TYPE";
        case TypeColumn::STATE:
            return "STATE";
        case TypeColumn::LAST_CHANGED:
            return "LAST_CHANGED";
        case TypeColumn::UPTIME:
            return "UPTIME";
        case TypeColumn::HEAP:
            return "HEAP";
        case TypeColumn::CONNECTION_TIMEPOINT:
            return "CONNECTION_TIMEPOINT";
        case TypeColumn::RECONNECT_COUNT:
            return "RECONNECT_COUNT";
        case TypeColumn::LAST_REASON_RECONNECTION:
            return "LAST_REASON_RECONNECTION";
        case TypeColumn::RSSI:
            return "RSSI";
        default:
            return QVariant();
    }
}
} // namespace barmaley::gui
