#pragma once

#include <QItemDelegate>
namespace barmaley::gui {

class DeviceListDelegat : public QItemDelegate
{
public:
    using QItemDelegate::QItemDelegate;

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

};
} // namespace barmaley::gui
