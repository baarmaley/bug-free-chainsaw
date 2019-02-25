#include "model.hpp"
#include <QJsonObject>
#include <iomanip>
#include <sstream>
namespace {
template<typename U>
U uintFrom(const QJsonValue& value)
{
    if (!value.isDouble()) {
        throw std::logic_error("jsonValue is not Double");
    }
    auto iValue = value.toInt();
    if (iValue < 0) {
        throw std::logic_error("jsonValue is not a number");
    }
    return static_cast<U>(iValue);
}
std::chrono::system_clock::time_point timepointFrom(const QJsonValue& value)
{
    auto dur = std::chrono::seconds(uintFrom<std::uint64_t>(value));
    return std::chrono::system_clock::now() - dur;
}
QString timepointToLocalTime(const std::chrono::system_clock::time_point& tp)
{
    auto t = std::chrono::system_clock::to_time_t(tp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%d.%m.%y %T");
    return QString::fromStdString(ss.str());
}
} // namespace

State stateFrom(const QJsonDocument& jsonDocument)
{
    if (!jsonDocument.isObject()) {
        throw std::logic_error("jsonDocument is not an Object");
    }
    auto mainObject = jsonDocument.object();

    if (mainObject.contains("state") && mainObject["state"].isObject()) {
        auto stateObject = mainObject["state"].toObject();

        auto deviceId = [&] {
            if (stateObject.contains("device_id") && stateObject["device_id"].isString()) {
                bool ok       = false;
                auto deviceId = stateObject["device_id"].toString().toUInt(&ok);
                if (!ok) {
                    throw std::logic_error("device_id is not a number");
                }
                return toDeviceId(deviceId);
            } else {
                throw std::logic_error("does json not have device_id field");
            }
        }();

        State state(deviceId);

        if (stateObject.contains("device_type")) {
            state.deviceType = stateObject["device_type"].toString();
        }

        if (stateObject.contains("output_pin") && stateObject["output_pin"].isObject()) {
            state.state = QJsonDocument(stateObject["output_pin"].toObject()).toJson(QJsonDocument::Compact);
        }

        if (mainObject.contains("debug") && mainObject["debug"].isObject()) {
            state.debug = debugFrom(mainObject["debug"].toObject());
        }

        return state;
    }
    throw std::logic_error("does mainObject not have stateObject");
}

Debug debugFrom(const QJsonObject& jsonObject)
{
    Debug debug;
    if (jsonObject.contains("last_change") && jsonObject["last_change"].isString()) {
        debug.lastChanged = jsonObject["last_change"].toString();
    }
    if (jsonObject.contains("uptime") && jsonObject["uptime"].isDouble()) {
        debug.uptime = timepointFrom(jsonObject["uptime"]);
    }
    if (jsonObject.contains("heap") && jsonObject["heap"].isDouble()) {
        debug.heap = uintFrom<std::uint32_t>(jsonObject["heap"]);
    }
    if (jsonObject.contains("connection_timepoint") && jsonObject["connection_timepoint"].isDouble()) {
        debug.connectionTimepoint = timepointFrom(jsonObject["connection_timepoint"]);
    }
    if (jsonObject.contains("rssi") && jsonObject["rssi"].isString()) {
        bool ok    = false;
        debug.rssi = jsonObject["rssi"].toString().toInt(&ok);
        if (!ok) {
            throw std::logic_error("rssi is not a number");
        }
    }
    if (jsonObject.contains("reconnect_count") && jsonObject["reconnect_count"].isDouble()) {
        debug.reconnectCount = uintFrom<std::uint32_t>(jsonObject["reconnect_count"]);
    }
    if (jsonObject.contains("last_reason_reconnection") && jsonObject["last_reason_reconnection"].isString()) {
        debug.lastReasonReconnection = jsonObject["last_reason_reconnection"].toString();
    }
    return debug;
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
    const auto& value = items.find(key)->second;
    const auto& state = value.state;

    switch (index.column()) {
        case TypeColumn::IP:
            return value.ip;
        case TypeColumn::COUNT_UPDATE:
            return value.countUpdate;
        case TypeColumn::LAST_UPDATE:
            return timepointToLocalTime(value.lastUpdate);
        case TypeColumn::ID:
            return QString::number(toUint32(value.state.id));
        case TypeColumn::DEVICE_TYPE:
            return state.deviceType;
        case TypeColumn::STATE:
            return state.state;
        case TypeColumn::LAST_CHANGED:
            return state.debug ? state.debug->lastChanged : QVariant();
        case TypeColumn::UPTIME:
            return state.debug ? timepointToLocalTime(state.debug->uptime) : QVariant();
        case TypeColumn::HEAP:
            return state.debug ? state.debug->heap : QVariant();
        case TypeColumn::CONNECTION_TIMEPOINT:
            return state.debug ? timepointToLocalTime(state.debug->connectionTimepoint) : QVariant();
        case TypeColumn::RECONNECT_COUNT:
            return state.debug ? state.debug->reconnectCount : QVariant();
        case TypeColumn::LAST_REASON_RECONNECTION:
            return state.debug ? state.debug->lastReasonReconnection : QVariant();
        case TypeColumn::RSSI:
            return state.debug ? state.debug->rssi : QVariant();
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
        case TypeColumn::COUNT_UPDATE:
            return "COUNT_UPDATE";
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

void Model::insertOrUpdate(QString ip, State state)
{
    auto deviceId = state.id;
    auto it       = items.find(deviceId);
    if (it == items.end()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        order.push_back(deviceId);
        items.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(deviceId),
            std::forward_as_tuple(std::move(state), std::move(ip), std::uint32_t{0}, std::chrono::system_clock::now()));
        endInsertRows();
    } else {
        auto itOrder          = std::find(order.begin(), order.end(), deviceId);
        auto rowIndex         = std::distance(order.begin(), itOrder);
        it->second.ip         = ip;
        it->second.state      = std::move(state);
        it->second.lastUpdate = std::chrono::system_clock::now();
        ++it->second.countUpdate;
        dataChanged(index(rowIndex, 0), index(rowIndex, columnCount()));
    }
}
