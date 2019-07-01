
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.2
import QtQuick.Controls.Material 2.12

ItemDelegate{
    signal clickedDelegate(int device_id)
    width: parent.width
    contentItem:
        Row{
            spacing: 4
            Rectangle{
                color: model.is_connection_lost ? Material.color(Material.Grey) : Material.color(Material.LightGreen)
                width: 4
                height: parent.height
            }
            Column{
                Text{
                    text: "Name device: " + model.device_name + " (id: " + model.device_id +")"
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
    onClicked: {
        clickedDelegate(model.device_id)
    }
}
