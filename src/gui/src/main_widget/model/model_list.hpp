#pragma once

#include <QAbstractListModel>

#include <common/common_type.hpp>
#include <common/signals2.hpp>
#include <current_state_model/current_state_model_fwd.hpp>
#include <current_state_model/current_state_model_view_fwd.hpp>

namespace barmaley::gui {
class ModelList : public QAbstractListModel
{
public:
    ModelList(lib::CurrentStateModelView& modelView);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    inline lib::DeviceId deviceId(int row) const
    {
        return order.at(row);
    }

private:
    const lib::CurrentStateModel& currentStateModel;
    std::vector<lib::DeviceId> order;
    lib::ConnectionsContainer connectionsContainer;
};
} // namespace barmaley::gui
