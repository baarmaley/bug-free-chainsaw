#include "model.hpp"

#include <iomanip>
#include <sstream>

#include <current_state_model/current_state_model.hpp>

namespace {
QString toLocalTime(const std::chrono::system_clock::time_point now,
                    const std::chrono::seconds& dur = std::chrono::seconds{0})
{
    auto t = std::chrono::system_clock::to_time_t(now - dur);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%d.%m.%y %T");
    return QString::fromStdString(ss.str());
}

} // namespace

namespace barmaley::gui {

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

    auto timepointNow = std::chrono::system_clock::now();

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
            return (debug && debug->lastChanged) ? QString::fromStdString(*debug->lastChanged) : QVariant();
        case TypeColumn::UPTIME:
            return (debug && debug->uptime) ? toLocalTime(timepointNow, *debug->uptime) : QVariant();
        case TypeColumn::HEAP:
            return (debug && debug->heap) ? *debug->heap : QVariant();
        case TypeColumn::CONNECTION_TIMEPOINT:
            return (debug && debug->connectionTimepoint) ? toLocalTime(timepointNow, *debug->connectionTimepoint)
                                                         : QVariant();
        case TypeColumn::RECONNECT_COUNT:
            return (debug && debug->reconnectCount) ? *debug->reconnectCount : QVariant();
        case TypeColumn::LAST_REASON_RECONNECTION:
            return (debug && debug->lastReasonReconnection) ? QString::fromStdString(*debug->lastReasonReconnection)
                                                            : QVariant();
        case TypeColumn::RSSI:
            return (debug && debug->rssi) ? *debug->rssi : QVariant();
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
