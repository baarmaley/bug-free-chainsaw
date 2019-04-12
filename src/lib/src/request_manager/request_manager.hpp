#pragma once

#include <common/common_type.hpp>
#include <timer/timer.hpp>

#include <pplx/pplxtasks.h>

#include <functional>
#include <vector>

namespace barmaley::lib {
class RequestManager
{
public:
    RequestManager();

    struct Success
    {
        std::string response;
    };
    struct Error
    {
        std::string what;
    };
    struct Request
    {
        Request(pplx::task<std::string> task,
                std::function<void(Success)> onSuccess,
                std::function<void(Error)> onError)
            : task(std::move(task)), onSuccess(std::move(onSuccess)), onError(std::move(onError))
        {
        }
        pplx::task<std::string> task;
        std::function<void(Success)> onSuccess;
        std::function<void(Error)> onError;
    };

    void request(std::string url, std::function<void(Success)> onSuccess, std::function<void(Error)> onError);

private:
    std::vector<Request> requests;
    Timer timer{std::chrono::milliseconds(10)};
    ConnectionsContainer connections;
};
} // namespace barmaley::lib
