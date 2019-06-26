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

    auto key             = order[index.row()];
    const auto& value    = *currentStateModel.value(key);
    const auto& status   = value.status;
    const auto& wifiInfo = status.wifiInfo;

    switch (index.column()) {
        case TypeColumn::IP:
            return QString::fromStdString(value.ip);
        case TypeColumn::PACKETS_RECEIVED:
            return value.numberOfPacketsReceived;
        case TypeColumn::LAST_UPDATE:
            return toLocalTime(value.lastUpdate);
        case TypeColumn::ID:
            return QString::number(toUint32(status.id));
        case TypeColumn::DEVICE_TYPE:
            return QString::fromStdString(status.deviceType);
        case TypeColumn::DEVICE_NAME:
            return QString::fromStdString(status.name);
        case TypeColumn::LAST_CHANGED:
            return QString::fromStdString(status.gpioInfo.lastChanged);
        case TypeColumn::UPTIME:
            return toLocalTime(status.uptimeTp());
        case TypeColumn::HEAP:
            return status.heap;
        case TypeColumn::CONNECTION_TIMEPOINT:
            return toLocalTime(status.connectionTp());
        case TypeColumn::RECONNECT_COUNT:
            return wifiInfo.reconnectCount;
        case TypeColumn::LAST_REASON_RECONNECTION:
            return (wifiInfo.lastReasonReconnection) ? QString::fromStdString(*wifiInfo.lastReasonReconnection)
                                                     : QStringLiteral("-");
        case TypeColumn::RSSI:
            return wifiInfo.rssi;
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
		case TypeColumn::DEVICE_NAME:
            return "DEVICE_NAME";
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
