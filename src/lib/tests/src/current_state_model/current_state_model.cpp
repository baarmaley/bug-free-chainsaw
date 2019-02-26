#include <catch2/catch.hpp>

#include <current_state_model/current_state_model.hpp>

using namespace barmaley::lib;

TEST_CASE("Current state model test")
{
    std::string addResult;
    std::string updateResult;

    CurrentStateModel currentStateModel;
    {
        State newState;
        newState.id         = DeviceId{20};
        newState.deviceType = "something";
        newState.state      = "state";

        ConnectionsContainer connectionsContainer;

        connectionsContainer +=
            currentStateModel.currentStateModelView.onAdded([&addResult](DeviceId) { addResult += "Ok"; });
        connectionsContainer +=
            currentStateModel.currentStateModelView.onUpdate([&updateResult](DeviceId) { updateResult += "Ok"; });

        currentStateModel.insertOrUpdate("", std::move(newState), std::nullopt);

        REQUIRE(addResult == "Ok");

        State updateState;
        updateState.id         = DeviceId{20};
        updateState.deviceType = "something two";
        updateState.state      = "state 2";

        currentStateModel.insertOrUpdate("", std::move(updateState), std::nullopt);

        REQUIRE(updateResult == "Ok");

        auto v = currentStateModel.value(DeviceId{20});

        REQUIRE(v);
        REQUIRE(v->state.state == "state 2");
    }

    State updateState;
    updateState.id         = DeviceId{20};
    updateState.deviceType = "something two";
    updateState.state      = "state 2";
    currentStateModel.insertOrUpdate("", std::move(updateState), std::nullopt);

    REQUIRE(updateResult == "Ok");
}
