#pragma once

#include <common/signals2.hpp>

namespace barmaley {
namespace lib {
class Facade;
class FacadeView
{
public:
    using ProtocolErrorSignal = s2::signal<void(std::string ip, std::string errorString)>;

    FacadeView(const Facade& facade);
    FacadeView(const FacadeView&) = delete;
    FacadeView(FacadeView&&)      = delete;
    ~FacadeView();

    friend Facade;

    s2::connection onProtocolError(ProtocolErrorSignal::slot_type slot);

    const Facade& facade;

protected:
    ProtocolErrorSignal protocolErrorEvent;
};
} // namespace lib
} // namespace barmaley
