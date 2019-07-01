#include "device_item.hpp"

#include <current_state_model/current_state_model.hpp>

#include <QDebug>

namespace barmaley::gui {
DeviceItem::DeviceItem(lib::CurrentStateModelView& modelView) : currentStateModel(modelView.currentStateModel)
{
    m_roleNames[relay_id]     = "relay_id";
    m_roleNames[is_turned_on] = "is_turned_on";
    m_roleNames[name]         = "name";

    connectionsContainer += modelView.onUpdate([this](lib::DeviceId id) {
        if (currentDevice && *currentDevice == id) {
            update(id);
        }
    });
}
QString DeviceItem::getDeviceName() const
{
    return currentDeviceName;
}
QString DeviceItem::getCountPackets() const
{
    return countPackets;
}
bool DeviceItem::isBusy() const
{
    return m_busy;
}
int DeviceItem::getCurrentDevice() const
{
    return currentDevice ? toUint32(*currentDevice) : -1;
}
void DeviceItem::setCurrentDevice(int deviceId)
{
    if (deviceId > 0 && currentStateModel.hasValue(lib::toDeviceId(deviceId))) {
        update(lib::toDeviceId(deviceId));
        qDebug() << "DeviceId:" << deviceId << "count:" << rowCount();

        return;
    }
    update(std::nullopt);
}
int DeviceItem::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_items.size());
}
QVariant DeviceItem::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role != Qt::DisplayRole && !m_roleNames.contains(role)) {
        return QVariant();
    }
    const auto& item = m_items[index.row()];
    switch (role) {
        case Qt::DisplayRole:
            return "Relay";
        case RoleNames::relay_id:
            return QString::number(toUint32(item.id));
        case RoleNames::is_turned_on:
            return item.isTurnedOn;
        case RoleNames::name:
            return item.name.isEmpty() ? "Unknown" : item.name;
        default:
            return QVariant();
    }
}

void DeviceItem::update(std::optional<lib::DeviceId> id)
{
    if (!id) {
        currentDevice     = std::nullopt;
        currentDeviceName = "";
        countPackets      = "";
        m_busy            = false;

        beginRemoveRows(QModelIndex(), 0, rowCount());
        if (!m_items.empty()) {
            m_items.clear();
        }
        endRemoveRows();

    } else {
        auto device       = currentStateModel.value(*id);
        currentDevice     = id;
        currentDeviceName = QString::fromStdString(device->status.name);
        countPackets      = QString::number(device->numberOfPacketsReceived);
        m_busy            = device->isBusy;

        const auto& relays = device->status.relays;
        for (std::size_t i = 0; i < relays.size(); ++i) {
            if (i >= m_items.size()) {
                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                m_items.emplace_back();
                endInsertRows();
            }
            auto item_old = m_items[i];
            auto& item    = m_items[i];

            item.id         = relays[i].id;
            item.isTurnedOn = relays[i].status == lib::RelayStatus::ON;
            item.name       = QString::fromStdString(relays[i].name);

            if (!(item_old == item)) {
                auto row = static_cast<int>(i);
                dataChanged(index(row), index(row));
            }
        }
        if (m_items.size() > relays.size()) {
            auto removeWith = static_cast<int>(relays.size());
            beginRemoveRows(QModelIndex(), removeWith, rowCount());
            m_items.erase(m_items.begin() + removeWith, m_items.end());
            endInsertRows();
        }
    }

    emit deviceNameChanged(getDeviceName());
    emit countPacketsChanged(getCountPackets());
    emit busyChanged(isBusy());
}

QHash<int, QByteArray> DeviceItem::roleNames() const
{
    return m_roleNames;
}
} // namespace barmaley::gui
