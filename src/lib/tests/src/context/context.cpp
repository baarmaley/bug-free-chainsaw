#include <catch2/catch.hpp>

#include <common/context.hpp>

using namespace barmaley::lib;

struct Foo
{
    Context<Foo> ctx{this};
};

TEST_CASE("Context test")
{
    std::weak_ptr<Foo*> wPtr;
    {
        Foo foo;
        wPtr = foo.ctx.ptr();

        auto sharedPtr = wPtr.lock();

        REQUIRE(sharedPtr);
    }
    auto sharedPtr = wPtr.lock();
    REQUIRE_FALSE(sharedPtr);
}
