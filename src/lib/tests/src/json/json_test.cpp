#include <catch2/catch.hpp>

#include <common/common_type.hpp>

using namespace barmaley::lib;

TEST_CASE("Convert common type from json")
{
    json state_j = R"({ "device_id": 234, "device_type": "undefined", "output_pin" : { "0": 0, "1": 1} })"_json;
    json debug_j = R"({ "last_change" : "Default",
                        "uptime" : 45678,
                        "heap" : 45646,
                        "connection_timepoint" : 4564,
                        "reconnect_count" : 787,
                        "rssi" : -63,
                        "last_reason_reconnection" : "AP_NOT_FOUND" }
                   )"_json;

    auto s = state_j.get<State>();

    auto d = debug_j.get<Debug>();

    REQUIRE(*d.lastChanged == "Default");
    REQUIRE(*d.uptime == std::chrono::seconds(45678));
    REQUIRE(*d.heap == 45646);
    REQUIRE(*d.connectionTimepoint == std::chrono::seconds(4564));
    REQUIRE(*d.reconnectCount == 787);
    REQUIRE(*d.rssi == -63);
    REQUIRE(*d.lastReasonReconnection == "AP_NOT_FOUND");

    json debug_j_2 = R"({ "last_change" : "Default" }
                   )"_json;

    auto d_2 = debug_j_2.get<Debug>();

    REQUIRE(d_2.uptime == std::nullopt);

    auto debug_j_3 =
        R"({
        "state" : {"device_id" : 245, "device_type" : "undefined", "output_pin" : {"1" : 0, "2" : 0}}, 
		"debug":
			{
				"last_change" : "Default",
				"uptime" : 339,
				"heap" : 19704,
				"connection_timepoint" : 76,
				"reconnect_count" : 2,
				"rssi" : -63,
				"last_reason_reconnection" : "BEACON_TIMEOUT"
			}
		})"_json;

    auto state = debug_j_3["state"].get<State>();
    auto debug = debug_j_3.value("debug", DebugOpt{std::nullopt});
}
