#include <catch2/catch.hpp>

#include <current_state_model/current_state_model.hpp>

using namespace barmaley::lib;

TEST_CASE("Current state model test")
{
    std::string addResult;
    std::string updateResult;

    CurrentStateModel currentStateModel;
    {
        Status newStatus;
		newStatus.id         = DeviceId{20};
		newStatus.deviceType = "something";

        ConnectionsContainer connectionsContainer;

        connectionsContainer +=
            currentStateModel.currentStateModelView.onAdded([&addResult](DeviceId) { addResult += "Ok"; });
        connectionsContainer +=
            currentStateModel.currentStateModelView.onUpdate([&updateResult](DeviceId) { updateResult += "Ok"; });

        currentStateModel.insertOrUpdate("", std::move(newStatus));

        REQUIRE(addResult == "Ok");

		Status updateStatus;
		updateStatus.id         = DeviceId{20};
		updateStatus.deviceType = "something two";

        currentStateModel.insertOrUpdate("", std::move(updateStatus));

        REQUIRE(updateResult == "Ok");

        auto v = currentStateModel.value(DeviceId{20});

        REQUIRE(v);
        REQUIRE(v->status.deviceType == "something two");
    }

	Status updateStatus;
	updateStatus.id         = DeviceId{20};
	updateStatus.deviceType = "something three";
    currentStateModel.insertOrUpdate("", std::move(updateStatus));

    REQUIRE(updateResult == "Ok");
}
