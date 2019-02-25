#pragma once

#include <QAbstractTableModel>
#include <QJsonDocument>
#include <QString>

#include <chrono>
#include <optional>
#include <unordered_map>
#include <vector>

enum class DeviceId : std::uint32_t
{
};

constexpr DeviceId toDeviceId(std::uint32_t v)
{
    return static_cast<DeviceId>(v);
}

constexpr std::uint32_t toUint32(DeviceId v)
{
    return static_cast<std::uint32_t>(v);
}

struct Debug
{
    QString lastChanged;
    std::chrono::system_clock::time_point uptime;
    std::uint32_t heap = 0;
    std::chrono::system_clock::time_point connectionTimepoint;
    std::uint32_t reconnectCount = 0;
    int rssi                     = 0;
    QString lastReasonReconnection;
};

struct State
{
    State(DeviceId id) : id(id) {}
    DeviceId id;
    QString deviceType;
    QString state;
    std::optional<Debug> debug;
};

struct Row
{
    Row(State state, QString ip, std::uint32_t countUpdate, std::chrono::system_clock::time_point lastUpdate)
        : state(std::move(state)), ip(std::move(ip)), countUpdate(countUpdate), lastUpdate(std::move(lastUpdate)){};
    State state;
    QString ip;
    std::uint32_t countUpdate;
    std::chrono::system_clock::time_point lastUpdate;
};

State stateFrom(const QJsonDocument& jsonDocument);
Debug debugFrom(const QJsonObject& jsonObject);

class Model : public QAbstractTableModel
{
public:
    enum TypeColumn
    {
        IP = 0,
        COUNT_UPDATE,
        LAST_UPDATE,
        ID,
        DEVICE_TYPE,
        STATE,
        LAST_CHANGED,
        UPTIME,
        HEAP,
        CONNECTION_TIMEPOINT,
        RECONNECT_COUNT,
        LAST_REASON_RECONNECTION,
        RSSI
    };

    int columnCount(const QModelIndex& = QModelIndex()) const override;
    int rowCount(const QModelIndex& = QModelIndex()) const override;

    QVariant data(const QModelIndex&, int) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void insertOrUpdate(QString ip, State state);

private:
    std::vector<DeviceId> order;
    std::unordered_map<DeviceId, Row> items;
};
