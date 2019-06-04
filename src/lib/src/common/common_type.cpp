#include <common/common_type.hpp>

namespace barmaley::lib {
using string_opt = std::optional<std::string>;

void from_json(const json& j, Relay& r)
{
    j.at("id").get_to(r.id);
    j.at("status").get_to(r.status);
    if (r.status == RelayStatus::INVALID) {
        throw std::logic_error("Invalid relay state");
    }
    j.at("name").get_to(r.name);
}
void from_json(const json& j, WifiInfo& w)
{
    j.at("connection_timepoint").get_to(w.connectionDuration);
    j.at("reconnect_count").get_to(w.reconnectCount);
    j.at("rssi").get_to(w.rssi);

    // Todo: update after apply pr "Feat/explicit conversion operator"
    // w.lastReasonReconnection = j.value("last_reason_reconnection", string_opt{std::nullopt});
    w.lastReasonReconnection = [&]() -> std::optional<std::string> {
        if (j.contains("last_reason_reconnection")) {
            return j.at("last_reason_reconnection").get<std::string>();
        } else {
            return std::nullopt;
        }
    }();
}
void from_json(const json& j, GpioInfo& g)
{
    j.at("last_change").get_to(g.lastChanged);
}
void from_json(const json& j, Status& s)
{
    s.receivedDate = std::chrono::system_clock::now();

    j.at("device_id").get_to(s.id);
    j.at("device_type").get_to(s.deviceType);
    j.at("device_name").get_to(s.name);
    for (auto& item : j.at("relays").items()) {
        s.relays.push_back(item.value().get<Relay>());
    }
    j.at("uptime").get_to(s.uptime);
    j.at("heap").get_to(s.heap);
    j.at("wifi_info").get_to(s.wifiInfo);
    j.at("gpio_info").get_to(s.gpioInfo);
}
} // namespace barmaley::lib
