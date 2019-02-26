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
Facade::Facade()
{
    ñonnectionsContainer += receiver.onReceived([this] {
        while (!receiver.empty()) {
            auto p  = receiver.packet();
            auto ip = p.ip;
            try {
                auto [state, debug] = parser(std::move(p.data));
                model.insertOrUpdate(std::move(p.ip), std::move(state), std::move(debug));
            } catch (const json::exception& e) {
                qWarning() << QString::fromStdString(ip) << e.what();
            }
        }
    });
}
Facade::~Facade() = default;
} // namespace barmaley::lib
