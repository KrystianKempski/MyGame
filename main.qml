
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.2
import QtQml.Models 2.12
import Qt.labs.qmlmodels 1.0


ApplicationWindow {
    id:mainWindow
    property bool chosenTeam
    visible: true
    width: 1140
    height: 650
    title: qsTr("Taktyczny symulator bitwy")

    Action {
        id: actionTroops1
        text: "Oddziały"
        icon.color: "transparent"
        onTriggered: chosenTeam ? stackView.push("TroopsRed.qml"):stackView.push("TroopsBlue.qml")
    }
    Action {
        id: actionBattlefield1
        text: "Pole bitwy"
        icon.color: "transparent"
        onTriggered: chosenTeam ? stackView.push("BattlefieldRed.qml"):stackView.push("BattlefieldBlue.qml")
    }

    menuBar: MenuBar{
        Menu{
            id: menuFile
            title: "Widok"
            MenuItem{action: actionTroops1 }
            MenuItem{action: actionBattlefield1}
            enabled: false
        }
    }
    StackView {
        id: stackView
        initialItem: "EnterWindow.qml"
        anchors.fill: parent
    }


}
