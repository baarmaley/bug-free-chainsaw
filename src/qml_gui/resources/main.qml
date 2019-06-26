
import QtQuick 2.0
import QtQuick.Controls 2.12

ApplicationWindow
{
    visible: true
    width: 640
    height: 480
    title: qsTr("Minimal Qml")

    ListView {
        anchors.fill: parent
        anchors.margins: 20

        clip: true

        model: deviceListModel

        spacing: 5

        delegate: DeviceDelegate{}
    }

}
