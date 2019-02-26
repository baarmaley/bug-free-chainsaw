#pragma once

#include <boost/signals2.hpp>
#include <vector>

namespace barmaley::lib {
namespace s2 = boost::signals2;
struct ConnectionsContainer
{
public:
    ConnectionsContainer() = default;

    ConnectionsContainer(const ConnectionsContainer&) = delete;
    ConnectionsContainer(ConnectionsContainer&&)      = default;
    ConnectionsContainer& operator=(const ConnectionsContainer&) = delete;
    ConnectionsContainer& operator=(ConnectionsContainer&&) = default;

    ~ConnectionsContainer() = default;

    void append(s2::scoped_connection&& scoped_connection)
    {
        connection.push_back(std::move(scoped_connection));
    }

    ConnectionsContainer& operator+=(s2::scoped_connection&& scoped_connection)
    {
        append(std::move(scoped_connection));
        return *this;
    }

private:
    std::vector<s2::scoped_connection> connection;
};

} // namespace barmaley::lib
