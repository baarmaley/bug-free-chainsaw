
import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.2

Component{
    id: delegatDevice

        Column{
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            Text{
                        id: element
                        text: "Name device: " + model.device_name
            }

            Text{
                        id: element1
                        text: "Ip:" + model.ip
            }
        }
}






