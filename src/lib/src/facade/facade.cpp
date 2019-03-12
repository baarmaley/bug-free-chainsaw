#include <facade/facade.hpp>

#include <QDebug>

namespace barmaley::lib {
namespace {
std::tuple<State, std::optional<Debug>> parser(std::string&& data)
{
    auto j = json::parse(std::move(data));

    auto state = j["state"].get<State>();
    auto debug = j.value("debug", DebugOpt{std::nullopt});

    return std::make_tuple(std::move(state), std::move(debug));
}
} // namespace
Facade::Facade() : facadeView(*this), journalManager(facadeView, model.currentStateModelView)
{
    connectionsContainer += receiver.onReceived([this] {
        while (!receiver.empty()) {
            auto p  = receiver.packet();
            auto ip = p.ip;
            try {
                auto [state, debug] = parser(std::move(p.data));
                model.insertOrUpdate(std::move(p.ip), std::move(state), std::move(debug));
            } catch (const json::exception& e) {
                facadeView.protocolErrorEvent(std::move(ip), e.what());
            }
        }
    });
}
Facade::~Facade() = default;
} // namespace barmaley::lib
