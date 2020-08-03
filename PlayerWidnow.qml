import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQml.Models 2.1
import Qt.labs.qmlmodels 1.0

        Item{
            Component.onCompleted: dataSource.writeConsole("nowa gra\r\n")
            width: parent.width
            height: parent.height
            id: view
            property bool changed2: false
            property int pageNum: 1
            anchors.fill: parent

            BattlefieldRed {
                widthProp: parent.width
                heightProp: parent.height
                visible: parent.pageNum===1
            }
            TroopsRed {
                widthProp: parent.width
                heightProp: parent.height
                visible: parent.pageNum===2
            }
            BattlefieldBlue {
                widthProp: parent.width
                heightProp: parent.height
                visible: parent.pageNum===3
            }
            TroopsBlue {
                widthProp: parent.width
                heightProp: parent.height
                visible: parent.pageNum===4
            }
        }
