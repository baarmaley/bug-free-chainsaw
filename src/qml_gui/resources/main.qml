
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12


ApplicationWindow
{
    visible: true
    width: 320
    height: 480
    title: Qt.application.name

    header: ToolBar{
        Material.background: Material.Blue
        RowLayout {
            anchors.fill: parent
            Item {
                height: parent.height
                width: menuButton.width
                visible: menuButton.visible
            }
            ToolButton {
                id: backToolButton
                icon.name: "back"
                visible: stackView.depth > 1
                onClicked: stackView.pop()
            }
            Label {
                id: titleLabel
                text: Qt.application.name
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
            Item {
                height: parent.height
                width: backToolButton.width
                visible: backToolButton.visible
            }
            ToolButton {
                id: menuButton
                icon.name: "menu"
                visible: !(stackView.depth > 1)
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Journal"
                    }
                    MenuItem {
                        text: "About"
                    }
                }
            }
        }
    }

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: {
            stackView.pop()
        }
    }

    StackView{
        signal clickedRelay(int device_id, int relay_id)
        id: stackView
        objectName: "stackView"
        anchors.fill: parent
        anchors.topMargin: 10

        initialItem:
            ListView {
            clip: true

            model: deviceListModel

            /*model: ListModel{
                    ListElement { device_id: 25; device_name: "Desk"; ip: "192.168.1.1";  is_connection_lost: true }
                    ListElement { device_id: 45; device_name: "Route"; ip: "192.168.1.2"; is_connection_lost: false }
                }*/

            delegate: DeviceDelegate{
                onClickedDelegate: {
                    deviceItem.currentDevice = device_id
                    var devicePage = stackView.push("DevicePage.qml")
                    devicePage.clickedRelay.connect(stackView.clickedRelay)
                    console.log("id: ", device_id)
                }
            }
        }
    }
}
