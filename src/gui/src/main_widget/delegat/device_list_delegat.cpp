#include "device_list_delegat.hpp"

#include <QPainter>

namespace barmaley::gui {
void DeviceListDelegat::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    //drawBackground(painter, option, index);
    //drawDecoration(painter, option, index);
    //painter->drawText(0, 40, "Hello world");

    QItemDelegate::paint(painter, option, index);
}
QSize DeviceListDelegat::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QItemDelegate::sizeHint(option, index);
}
} // namespace barmaley::gui
