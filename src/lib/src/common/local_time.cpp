#include <common/local_time.hpp>

#include <iomanip>
#include <sstream>

namespace barmaley {
namespace lib {
std::string localtime(const std::chrono::system_clock::time_point& tp, std::string_view pattern)
{
    auto t = std::chrono::system_clock::to_time_t(tp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), pattern.data());

    return ss.str();
}
} // namespace lib
} // namespace barmaley
