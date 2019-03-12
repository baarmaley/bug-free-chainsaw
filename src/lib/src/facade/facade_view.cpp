#include <facade/facade_view.hpp>

namespace barmaley {
namespace lib {
FacadeView::FacadeView(const Facade& facade) : facade(facade) {}
FacadeView::~FacadeView() = default;
s2::connection FacadeView::onProtocolError(ProtocolErrorSignal::slot_type slot)
{
    return protocolErrorEvent.connect(std::move(slot));
}
} // namespace lib
} // namespace barmaley
