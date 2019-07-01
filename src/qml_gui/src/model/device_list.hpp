#pragma once

#include <QAbstractListModel>

#include <common/common_type.hpp>
#include <common/signals2.hpp>
#include <current_state_model/current_state_model_fwd.hpp>
#include <current_state_model/current_state_model_view_fwd.hpp>

namespace barmaley::gui {
class DeviceList : public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleNames
    {
        ip_role = Qt::UserRole,
        packets_received,
        last_update,
        id,
        device_type,
        device_name,
        last_changed,
        uptime,
        heap,
        connection_timepoint,
        reconnect_count,
        last_reason_reconnection,
        rssi,
		is_connection_lost,
    };

    DeviceList(lib::CurrentStateModelView& modelView);

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const lib::CurrentStateModel& currentStateModel;
    QHash<int, QByteArray> m_roleNames;
    std::vector<lib::DeviceId> order;
    lib::ConnectionsContainer connectionsContainer;
};
} // namespace barmaley::gui
