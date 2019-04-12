#include <facade/facade.hpp>

#include <request_manager/request_manager.hpp>

#include <QDebug>

namespace barmaley::lib {

Facade::Facade()
    : facadeView(*this),
      requestManager(std::make_unique<RequestManager>()),
      journalManager(facadeView, model.currentStateModelView)
{
    connectionsContainer += receiver.onReceived([this] {
        while (!receiver.empty()) {
            auto p  = receiver.packet();
            auto ip = p.ip;
            try {
                model.insertOrUpdate(std::move(p.ip), json::parse(std::move(p.data)).at("status"));
            } catch (const json::exception& e) {
                facadeView.protocolErrorEvent(std::move(ip), e.what());
            }
        }
    });
}
} // namespace barmaley::lib
Facade::~Facade() = default;
} // namespace barmaley::lib
