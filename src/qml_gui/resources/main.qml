
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

ApplicationWindow
{
    visible: true
    width: 320
    height: 480
    title: Qt.application.name

    Item{
        id: myPage
        property alias text: internalText.text
        Text{
            id: internalText
            text: "MyText"
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
        id: stackView
        anchors.fill: parent

        initialItem:
            ListView {
                /*anchors.fill: parent*/
                anchors.margins: 1

                clip: true

                /*model: deviceListModel*/

                model: ListModel{
                    ListElement { device_id: 25; device_name: "Desk"; ip: "192.168.1.1";  is_connection_lost: true }
                    ListElement { device_id: 45; device_name: "Route"; ip: "192.168.1.2"; is_connection_lost: false }
                }

                spacing: 5

                delegate: DeviceDelegate{
                      onClickedDelegate: {
                          stackView.push(myPage)
                          internalText.text = "id: " + device_id
                          console.log("id: ", device_id)
                    }
                }

            }
    }

}

