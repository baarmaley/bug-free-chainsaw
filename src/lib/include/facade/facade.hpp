#pragma once

#include <common/signals2.hpp>
#include <current_state_model/current_state_model.hpp>
#include <journal/journal.hpp>
#include <receiver/receiver.h>

namespace barmaley::lib {
class Facade
{
public:
    Facade();
    ~Facade();

    inline CurrentStateModelView& currentStateModelView()
    {
        return model.currentStateModelView;
    }

private:
    Receiver receiver;
    CurrentStateModel model;
    ConnectionsContainer ñonnectionsContainer;
};
} // namespace barmaley::lib
