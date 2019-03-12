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
    d.receivedDate = std::chrono::system_clock::now();

    j.at("last_change").get_to(d.lastChanged);
    j.at("uptime").get_to(d.uptime);
    j.at("heap").get_to(d.heap);
    j.at("connection_timepoint").get_to(d.connectionDuration);
    j.at("reconnect_count").get_to(d.reconnectCount);
    j.at("rssi").get_to(d.rssi);
    d.lastReasonReconnection = j.value("last_reason_reconnection", string_opt{std::nullopt});
}
} // namespace barmaley::lib
