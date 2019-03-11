#pragma once

#include <string>
#include <chrono>

namespace barmaley {
namespace lib {
std::string localtime(const std::chrono::system_clock::time_point& tp, std::string_view pattern);
}
}