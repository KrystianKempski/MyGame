import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQml.Models 2.12
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

                visible: parent.pageNum===1
            }
            TroopsRed {

                visible: parent.pageNum===2
            }
            BattlefieldBlue {

                visible: parent.pageNum===3
            }
            TroopsBlue {
                visible: parent.pageNum===4
            }
        }
