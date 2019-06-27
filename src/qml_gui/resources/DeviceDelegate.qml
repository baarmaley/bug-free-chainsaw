
import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.2

Component{
    Rectangle{
        color: model.is_connection_lost ? "#eeeeee" : "#d7ffd9"
        border.width: 1
        border.color: Qt.darker(color)
        width: parent.width
        height: childrenRect.height
        property var margin: 8
        Rectangle{
            width: parent.width
            height: childrenRect.height + margin * 2
            color: "transparent"
            Column{
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: margin
                Text{
                    text: "Name device: " + model.device_name + " (id: " + model.id +")"
                }
                Text{
                    text: "Ip: " + model.ip
                }
                Text{
                    text: "Rssi: " + model.rssi
                }
                Text{
                    text: "Last update: " + model.last_update + " (packets: " + model.packets_received + ")"
                }
                Text{
                    text: "Uptime: " + model.uptime
                }
                Text{
                    text: "Heap: " + model.heap
                }
                Text{
                    text: "Connecttion timepoint: " + model.connection_timepoint
                }
                Text {
                    text: "Reconnect count: " + model.reconnect_count
                }
                Text {
                    text: "Last reason reconnection: " + model.last_reason_reconnection
                }
            }
        }
    }
}


