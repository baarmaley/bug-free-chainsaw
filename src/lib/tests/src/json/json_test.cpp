#include <catch2/catch.hpp>

#include <common/common_type.hpp>
#include <common/local_time.hpp>

using namespace barmaley::lib;

TEST_CASE("Convert common type from json")
{
    json status_j = R"({  
					   "status":{  
					      "device_id":56666541,
					      "device_type":"socket",
					      "device_name":"desk",
					      "relays":[  
					         {  
					            "id":1,
					            "status":0,
					            "name":"relay"
					         },
					         {  
					            "id":2,
					            "status":0,
					            "name":""
					         }
					      ],
					      "uptime":3498,
					      "heap":20528,
					      "wifi_info":{  
					         "connection_timepoint":1440,
					         "reconnect_count":2,
					         "rssi":-70,
					         "last_reason_reconnection":"BEACON_TIMEOUT"
					      },
					      "gpio_info":{  
					         "last_change":"Default"
					      }
					   }})"_json;

    auto s = status_j.at("status").get<Status>();

    REQUIRE(s.id == toDeviceId(56666541));
    REQUIRE(s.deviceType == "socket");
    REQUIRE(s.name == "desk");
    REQUIRE(s.relays.size() == static_cast<std::size_t>(2));
    REQUIRE(s.relays[0].id == toRelayId(1));
    REQUIRE(s.relays[0].status == RelayStatus::OFF);
    REQUIRE(s.relays[0].name == "relay");
    REQUIRE(s.uptime == std::chrono::seconds{3498});
    REQUIRE(s.heap == std::uint32_t{20528});

    REQUIRE(s.wifiInfo.connectionDuration == std::chrono::seconds{1440});
    REQUIRE(s.wifiInfo.reconnectCount == 2);
    REQUIRE(s.wifiInfo.rssi == -70);
    REQUIRE(s.wifiInfo.lastReasonReconnection == "BEACON_TIMEOUT");
    REQUIRE(s.gpioInfo.lastChanged == "Default");
}
