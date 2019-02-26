#pragma once

#include <chrono>
#include <optional>

#include <nlohmann/json.hpp>

namespace barmaley::lib {
using nlohmann::json;
} // namespace barmaley::lib

namespace nlohmann {
template<typename T>
struct adl_serializer<std::optional<T>>
{
    static void to_json(json& j, const std::optional<T>& opt)
    {
        if (opt == std::nullopt) {
            j = nullptr;
        } else {
            j = *opt;
        }
    }

    static void from_json(const json& j, std::optional<T>& opt)
    {
        if (j.is_null()) {
            opt = std::nullopt;
        } else {
            opt = j.get<T>();
        }
    }
};
template<>
struct adl_serializer<std::chrono::seconds>
{
    static void to_json(json& j, const std::chrono::seconds& opt)
    {
        j = opt.count();
    }

    static void from_json(const json& j, std::chrono::seconds& opt)
    {
        opt = static_cast<std::chrono::seconds>(j.get<std::uint64_t>());
    }
};
} // namespace nlohmann
