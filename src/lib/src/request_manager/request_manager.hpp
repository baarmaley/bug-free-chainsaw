#pragma once

#include <common/common_type.hpp>
#include <request_manager/failed_request.hpp>
#include <request_manager/successful_request.hpp>
#include <timer/timer.hpp>

#include <pplx/pplxtasks.h>

#include <functional>
#include <vector>

namespace barmaley::lib {
class RequestManager
{
public:
    RequestManager();

    struct Request
    {
        Request(pplx::task<std::string> task,
                std::function<void(SuccessfulRequest)> onSuccess,
                std::function<void(FailedRequest)> onError)
            : task(std::move(task)), onSuccess(std::move(onSuccess)), onError(std::move(onError))
        {
        }
        pplx::task<std::string> task;
        std::function<void(SuccessfulRequest)> onSuccess;
        std::function<void(FailedRequest)> onError;
    };

    void request(std::string url,
                 std::function<void(SuccessfulRequest)> onSuccess,
                 std::function<void(FailedRequest)> onError);

private:
    std::vector<Request> requests;
    Timer timer{std::chrono::milliseconds(10)};
    ConnectionsContainer connections;
};
} // namespace barmaley::lib
