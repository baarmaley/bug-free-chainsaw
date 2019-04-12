#include <request_manager/request_manager.hpp>

#include <cpprest/http_client.h>

#include <QCoreApplication>
#include <QDebug>
#include <QMetaObject>
#include <QThread>
#include <QTimer>

namespace barmaley::lib {
namespace {
#ifdef _UTF16_STRINGS
utility::string_t toUtilityString(std::string value)
{
    return {value.begin(), value.end()};
}
#else
constexpr utility::string_t toUtilityString(std::string value)
{
    return value;
}
#endif

} // namespace

RequestManager::RequestManager()
{
    requests.reserve(20);
    connections += timer.onTimeout([this] {
        if (!requests.empty()) {
            for (auto it = requests.begin(); it != requests.end();) {
                if (it->task.is_done()) {
                    try {
                        it->onSuccess({it->task.get()});
                    } catch (const web::http::http_exception& e) {
                        it->onError({e.what()});
                    }
                    it = requests.erase(it);
                    continue;
                }
                ++it;
            }
        }
    });
    timer.start();
}
void RequestManager::request(std::string url,
                             std::function<void(Success)> onSuccess,
                             std::function<void(Error)> onError)
{
    pplx::task<std::string> task;
    try {
        web::http::client::http_client client(web::uri(toUtilityString(url)));
        task = client.request(web::http::methods::GET).then([](web::http::http_response response) {
            return response.extract_utf8string();
        });
    } catch (const std::invalid_argument& e) {
        onError({e.what()});
        return;
    }
    requests.emplace_back(std::move(task), std::move(onSuccess), std::move(onError));
}
} // namespace barmaley::lib
