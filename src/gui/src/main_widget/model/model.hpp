#pragma once

#include <QAbstractTableModel>
#include <QString>

#include <chrono>
#include <optional>
#include <unordered_map>
#include <vector>

#include <common/common_type.hpp>
#include <common/signals2.hpp>
#include <current_state_model/current_state_model_fwd.hpp>

namespace barmaley::gui {
class Model : public QAbstractTableModel
{
public:
    Model(lib::CurrentStateModelView& modelView);

    enum TypeColumn : std::uint32_t
    {
        IP = 0,
        PACKETS_RECEIVED,
        LAST_UPDATE,
        ID,
        DEVICE_TYPE,
		DEVICE_NAME,
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

private:
    const lib::CurrentStateModel& currentStateModel;
    std::vector<lib::DeviceId> order;
    lib::ConnectionsContainer connectionsContainer;
};
} // namespace barmaley::gui
