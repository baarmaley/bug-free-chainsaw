#include "model.hpp"

#include <common/local_time.hpp>
#include <current_state_model/current_state_model.hpp>

namespace barmaley::gui {
namespace {
QString toLocalTime(const std::chrono::system_clock::time_point tp)
{
    return QString::fromStdString(lib::localtime(tp, "%d.%m.%y %T"));
}
} // namespace

DeviceList::DeviceList(lib::CurrentStateModelView& modelView) : currentStateModel(modelView.currentStateModel)
{
    m_roleNames[ip_role]                  = "ip";
    m_roleNames[packets_received]         = "packets_received";
    m_roleNames[last_update]              = "last_update";
    m_roleNames[id]                       = "device_id";
    m_roleNames[device_type]              = "device_type";
    m_roleNames[device_name]              = "device_name";
    m_roleNames[last_changed]             = "last_changed";
    m_roleNames[uptime]                   = "uptime";
    m_roleNames[heap]                     = "heap";
    m_roleNames[connection_timepoint]     = "connection_timepoint";
    m_roleNames[reconnect_count]          = "reconnect_count";
    m_roleNames[last_reason_reconnection] = "last_reason_reconnection";
    m_roleNames[rssi]                     = "rssi";
    m_roleNames[is_connection_lost]       = "is_connection_lost";

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
        dataChanged(index(rowIndex, 0), index(rowIndex, 0));
    });
}

int DeviceList::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(order.size());
}

QVariant DeviceList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role != Qt::DisplayRole && !m_roleNames.contains(role)) {
        return QVariant();
    }

    auto key             = order[index.row()];
    const auto& value    = *currentStateModel.value(key);
    const auto& status   = value.status;
    const auto& wifiInfo = status.wifiInfo;

    switch (role) {
        case ip_role:
            return QString::fromStdString(value.ip);
        case packets_received:
            return value.numberOfPacketsReceived;
        case last_update:
            return toLocalTime(value.lastUpdate);
        case id:
            return QString::number(toUint32(status.id));
        case device_type:
            return QString::fromStdString(status.deviceType);
        case device_name:
            return QString::fromStdString(status.name);
        case last_changed:
            return QString::fromStdString(status.gpioInfo.lastChanged);
        case uptime:
            return toLocalTime(status.uptimeTp());
        case heap:
            return status.heap;
        case connection_timepoint:
            return toLocalTime(status.connectionTp());
        case reconnect_count:
            return wifiInfo.reconnectCount;
        case last_reason_reconnection:
            return (wifiInfo.lastReasonReconnection) ? QString::fromStdString(*wifiInfo.lastReasonReconnection)
                                                     : QStringLiteral("-");
        case rssi:
            return wifiInfo.rssi;
        case is_connection_lost:
            return value.connectionLost;
        default:
            return QVariant();
    }
}
QHash<int, QByteArray> DeviceList::roleNames() const
{
    return m_roleNames;
}
} // namespace barmaley::gui
