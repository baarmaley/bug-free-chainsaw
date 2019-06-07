#pragma once

#include <common/common_type.hpp>

namespace barmaley::lib {

struct CurrentState
{
    CurrentState(Status status, std::string ip, std::chrono::system_clock::time_point lastUpdate)
        : status(std::move(status)), ip(std::move(ip)), lastUpdate(std::move(lastUpdate)){};
    Status status;
    std::string ip;
    std::uint32_t numberOfPacketsReceived = 0;
    std::chrono::system_clock::time_point lastUpdate;
    bool connectionLost = false;
    bool isBusy         = false;
};

} // namespace barmaley::lib
