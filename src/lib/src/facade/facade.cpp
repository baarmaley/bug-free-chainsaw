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
Facade::~Facade() = default;
void Facade::groupCommand(DeviceId id, GroupCommand cmd)
{
    auto url = "http://" + model.value(id)->ip + "/action/";
    switch (cmd) {
        case GroupCommand::on:
            url += "all_on";
            break;
        case GroupCommand::off:
            url += "all_off";
            break;
        default:
            break;
    }
    requestManager->request(
        url,
        [](auto) { qDebug() << "groupCommand: Ok"; },
        [](RequestManager::Error e) { qDebug() << "groupCommand: Error" << QString::fromStdString(e.what); });
}
void Facade::singleCommand(DeviceId id, RelayId relayId, SingleCommand cmd)
{
    auto url = "http://" + model.value(id)->ip + "/action/";
    switch (cmd) {
        case SingleCommand::on:
            url += "on";
            break;
        case SingleCommand::off:
            url += "off";
            break;
        case SingleCommand::inv:
            url += "inversion";
            break;
    }
    url += "/" + std::to_string(toUint32(relayId));
    requestManager->request(
        url,
        [](auto) { qDebug() << "singleCommand: Ok"; },
        [](RequestManager::Error e) { qDebug() << "singleCommand: Error" << QString::fromStdString(e.what); });
}
} // namespace barmaley::lib
