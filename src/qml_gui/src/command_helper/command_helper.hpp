#pragma once

#include <facade/facade_fwd.hpp>

#include <QObject>

namespace barmaley::gui {
class CommandHelper : public QObject
{
    Q_OBJECT
public:
    CommandHelper(lib::Facade& facade);

    lib::Facade& facade;

public slots:
    void singleCommand(int device_id, int relay_id);
};
} // namespace barmaley::gui
