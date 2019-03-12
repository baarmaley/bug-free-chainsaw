#pragma once

#include <common/signals2.hpp>
#include <current_state_model/current_state_model.hpp>
#include <facade/facade_view.hpp>
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

    inline JournalManagerView& journalManagerView()
    {
        return journalManager.view();
    }

private:
    FacadeView facadeView;
    Receiver receiver;
    CurrentStateModel model;
    JournalManager journalManager;
    ConnectionsContainer connectionsContainer;
};
} // namespace barmaley::lib
