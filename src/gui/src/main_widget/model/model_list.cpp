#include "model_list.hpp"

#include <current_state_model/current_state_model.hpp>

namespace barmaley::gui {
ModelList::ModelList(lib::CurrentStateModelView& modelView) : currentStateModel(modelView.currentStateModel)
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
        dataChanged(index(rowIndex, 0), index(rowIndex, 0));
    });
}
int ModelList::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(order.size());
}
QVariant ModelList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    auto key           = order[index.row()];
    const auto& value  = *currentStateModel.value(key);
    const auto& status = value.status;

    return QString("IP: %1\r\nID: %2\r\nDEVICE_TYPE: %3\r\nDEVICE_NAME: %4")
        .arg(QString::fromStdString(value.ip))
        .arg(QString::number(toUint32(status.id)))
        .arg(QString::fromStdString(status.deviceType))
        .arg(QString::fromStdString(status.name));
}
} // namespace barmaley::gui
