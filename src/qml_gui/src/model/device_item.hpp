#pragma once

#include <QAbstractListModel>

#include <common/common_type.hpp>
#include <common/signals2.hpp>
#include <current_state_model/current_state_model_fwd.hpp>
#include <current_state_model/current_state_model_view_fwd.hpp>

namespace barmaley::gui {
class DeviceItem : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString deviceName READ getDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(int currentDevice READ getCurrentDevice WRITE setCurrentDevice NOTIFY currentDeviceChanged)
    Q_PROPERTY(QString countPacket READ getCountPackets NOTIFY countPacketsChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)

public:
    struct RelayItem
    {
        lib::RelayId id;
        QString name;
        bool isTurnedOn = false;
    };

    enum RoleNames
    {
		relay_id = Qt::UserRole,
        is_turned_on,
        name,
    };

    DeviceItem(lib::CurrentStateModelView& modelView);

    QString getDeviceName() const;
    QString getCountPackets() const;
    bool isBusy() const;

    int getCurrentDevice() const;
    void setCurrentDevice(int);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

signals:
    void deviceNameChanged(QString);
    void currentDeviceChanged(int);
    void countPacketsChanged(QString);
    void busyChanged(bool);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    void update(std::optional<lib::DeviceId> id);

    const lib::CurrentStateModel& currentStateModel;
    std::optional<lib::DeviceId> currentDevice;
    QString currentDeviceName;
    QString countPackets;
    bool m_busy = false;
    std::vector<RelayItem> m_items;
    QHash<int, QByteArray> m_roleNames;
    lib::ConnectionsContainer connectionsContainer;
};

inline bool operator==(const DeviceItem::RelayItem& lhs, const DeviceItem::RelayItem& rhs)
{
    return lhs.id == rhs.id && lhs.isTurnedOn == rhs.isTurnedOn && lhs.name == rhs.name;
}
} // namespace barmaley::gui
