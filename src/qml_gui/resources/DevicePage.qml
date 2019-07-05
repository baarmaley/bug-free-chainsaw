import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

Item{
    signal clickedRelay(int device_id, int relay_id)

    id: devicePage
    Text{
        id: nameDevice
        text: deviceItem.deviceName
        font.bold: true
        font.pointSize: 14
        anchors.horizontalCenter: parent.horizontalCenter
    }
    ListView{
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: nameDevice.bottom
        anchors.bottom: devicePage.bottom
        anchors.margins: 1
        clip: true

        /*model: ListModel{
            ListElement { name: "Desk"; ip: "192.168.1.1";  is_connection_lost: true }
            ListElement { name: "Route"; ip: "192.168.1.2"; is_connection_lost: false }
        }*/
        model: deviceItem

        delegate: ItemDelegate{
            id: control
            width: parent.width
            enabled: !deviceItem.busy && deviceItem.connected
            onClicked: {
                console.log("current_device: " + deviceItem.currentDevice + " relay_id " + model.relay_id)
                clickedRelay(deviceItem.currentDevice, model.relay_id)
            }

            contentItem:RowLayout {
                Text {
                    Layout.fillWidth: true
                    text: model.name
                    font.family: control.font.family
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    color: control.enabled ? control.Material.foreground : control.Material.hintTextColor
                }
                Item{
                    id: indicator
                    property bool checked: model.is_turned_on
                    property real visualPosition: checked ? 1.0 : 0

                    implicitWidth: 38
                    implicitHeight: 32

                    Material.elevation: 1

                    Rectangle {
                        width: parent.width
                        height: 14
                        radius: height / 2
                        y: parent.height / 2 - height / 2
                        color: control.enabled ? (indicator.checked ? control.Material.switchCheckedTrackColor : control.Material.switchUncheckedTrackColor)
                                               : control.Material.switchDisabledTrackColor
                    }
                    Rectangle {
                        id: handle
                        x: Math.max(0, Math.min(parent.width - width, indicator.visualPosition * parent.width - (width / 2)))
                        y: (parent.height - height) / 2
                        width: 20
                        height: 20
                        radius: width / 2
                        color: control.enabled ? (indicator.checked ? control.Material.switchCheckedHandleColor : control.Material.switchUncheckedHandleColor)
                                               : control.Material.switchDisabledHandleColor

                        Behavior on x {
                            enabled: !control.pressed
                            SmoothedAnimation {
                                duration: 300
                            }
                        }
                        layer.enabled: indicator.Material.elevation > 0
                        layer.effect: ElevationEffect {
                            elevation: indicator.Material.elevation
                        }
                    }
                }
            }
        }
    }
}























/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
