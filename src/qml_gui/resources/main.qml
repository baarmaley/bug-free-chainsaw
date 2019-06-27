
import QtQuick 2.0
import QtQuick.Controls 2.12

ApplicationWindow
{
    visible: true
    width: 320
    height: 480
    title: Qt.application.name

    ListView {
        anchors.fill: parent
        anchors.margins: 1

        clip: true

        model: deviceListModel
        /*
        model: ListModel{
            ListElement { device_name: "Desk"; ip: "192.168.1.1";  is_connection_lost: true }
            ListElement { device_name: "Route"; ip: "192.168.1.2"; is_connection_lost: false }
        }*/

        spacing: 5

        delegate: DeviceDelegate{}

    }

}
