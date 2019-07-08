import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

Item{
    id: journalPage
    objectName: "journalPage"
    ListView{
       anchors.fill: parent
       clip: true

       model: journalModel
       ScrollBar.vertical: ScrollBar {
         id: journalScrollBar
         interactive: false
       }

       delegate: ItemDelegate{
           id: journalDelegate
           width: parent.width
           contentItem:
               Column{
                   Text{
                       text: "Date: " + model.date
                   }
                   Text{
                       text: "Type: " + model.type
                   }
                   Text{
                       text: "Text: " + model.text
                       width: parent.width - journalDelegate.anchors.leftMargin
                       wrapMode: Text.Wrap
                   }
               }
       }
    }
}
