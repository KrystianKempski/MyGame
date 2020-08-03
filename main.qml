
import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQml.Models 2.1
import Qt.labs.qmlmodels 1.0


ApplicationWindow {
    id:mainWindow
    property bool chosenTeam
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
        onTriggered: chosenTeam ? stackView.push("TroopsRed.qml"):stackView.push("TroopsBlue.qml")
    }
    Action {
        id: actionBattlefield1
        text: "Battlefield"
        icon.color: "transparent"
        onTriggered: chosenTeam ? stackView.push("BattlefieldRed.qml"):stackView.push("BattlefieldBlue.qml")
    }
//    Action {
//        id: actionTroops2
//        text: "Troops2"
//        icon.color: "transparent"
//        onTriggered: view.pageNum = 4
//    }
//    Action {
//        id: actionBattlefield2
//        text: "Battlefield"
//        icon.color: "transparent"
//        onTriggered: view.pageNum = 3
//    }

    menuBar: MenuBar{
        Menu{
            id: menuFile
            title: "File"
            MenuItem{action: actionNew }
            MenuItem{action: actionOpen }
            MenuItem{action: actionSave }
            MenuItem{action: actionTroops1 }
            MenuItem{action: actionBattlefield1}
        }
        Menu{
            id: menuEdit
            title: "Edit"
            MenuItem{  action: actionNew }
        }
    }
    StackView {
        id: stackView
        initialItem: "EnterWindow.qml"
        anchors.fill: parent
    }


}
