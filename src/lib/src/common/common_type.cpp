#include <common/common_type.hpp>

namespace barmaley::lib {
using string_opt  = std::optional<std::string>;
using uint32_opt  = std::optional<std::uint32_t>;
using seconds_opt = std::optional<std::chrono::seconds>;
void from_json(const json& j, State& s)
{
    j.at("device_id").get_to(s.id);
    j.at("device_type").get_to(s.deviceType);
    s.state = j.at("output_pin").dump();
}
void from_json(const json& j, Debug& d)
{
    d.lastChanged            = j.value("last_change", string_opt{std::nullopt});
    d.uptime                 = j.value("uptime", seconds_opt{std::nullopt});
    d.heap                   = j.value("heap", uint32_opt{std::nullopt});
    d.connectionTimepoint    = j.value("connection_timepoint", seconds_opt{std::nullopt});
    d.reconnectCount         = j.value("reconnect_count", uint32_opt{std::nullopt});
    d.rssi                   = j.value("rssi", std::optional<int>{std::nullopt});
    d.lastReasonReconnection = j.value("last_reason_reconnection", string_opt{std::nullopt});
}
} // namespace barmaley::lib
