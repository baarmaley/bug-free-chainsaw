#pragma once

#include <memory>

namespace barmaley {
namespace lib {
template<typename T>
class Context
{
public:
    Context(T* self) : contextPtr(std::make_shared<T*>(self)) {}

    Context(const Context&) = delete;
    Context(Context&&)      = delete;

    ~Context() {}

    std::weak_ptr<T*> ptr()
    {
        return contextPtr;
    }

private:
    std::shared_ptr<T*> contextPtr;
};
} // namespace lib
} // namespace barmaley
