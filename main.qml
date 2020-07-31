
import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQml.Models 2.1
import Qt.labs.qmlmodels 1.0


ApplicationWindow {
    visible: true
    width: 1140
    height: 650
    title: qsTr("Hello World")

    FileDialog{
        id: openDialog
        title: "Please chose a file"
        folder: shortcuts.home
        selectExisting: true
        selectMultiple: false
        onAccepted: {
        }
    }

    FileDialog{
        id: saveDialog
        title: "Please chose a file"
        folder: shortcuts.home
        selectExisting: false
        selectMultiple: false
        onAccepted: {
        }
    }
    Action {
        id: actionNew
        text: "New"
        icon.color: "transparent"
    }
    Action {
        id: actionOpen
        text: "Open"
        icon.color: "transparent"
    }
    Action {
        id: actionSave
        text: "Save"
        icon.color: "transparent"
    }

    Action {
        id: actionTroops1
        text: "Troops"
        icon.color: "transparent"
        onTriggered: view.pageNum = 2
    }
    Action {
        id: actionBattlefield1
        text: "Battlefield"
        icon.color: "transparent"
        onTriggered: view.pageNum = 1
    }
    Action {
        id: actionTroops2
        text: "Troops2"
        icon.color: "transparent"
        onTriggered: view.pageNum = 4
    }
    Action {
        id: actionBattlefield2
        text: "Battlefield"
        icon.color: "transparent"
        onTriggered: view.pageNum = 3
    }

    menuBar: MenuBar{
        Menu{
            id: menuFile
            title: "File"
            MenuItem{action: actionNew }
            MenuItem{action: actionOpen }
            MenuItem{action: actionSave }
            MenuItem{action: actionTroops1 }
            MenuItem{action: actionBattlefield1}
            MenuItem{action: actionTroops2 }
            MenuItem{action: actionBattlefield2}
        }
        Menu{
            id: menuEdit
            title: "Edit"
            MenuItem{  action: actionNew }
        }
    }
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
}
