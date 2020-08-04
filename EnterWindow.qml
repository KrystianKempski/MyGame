import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.2
import QtQml.Models 2.1
import Qt.labs.qmlmodels 1.0

Item {
    Rectangle{
        anchors.fill: parent
        color: "gray"
        Column{
            id: column
            anchors.centerIn: parent
             spacing: 5
        Row {
            spacing: 5
            Button {
                id: teamRed
                text: "Drużyna czerwona"
                height: 200
                //icon.color: "red"
                width: 300
                onClicked: {
                    stackView.push("TroopsRed.qml")
                    mainWindow.chosenTeam=true
                }
            }
            Button {
                id: teamBlue
                text: "Drużyna Niebieska"
                height: 200
                width: 300
                //icon.color: "blue"
                onClicked: {
                     stackView.push("TroopsBlue.qml")
                    mainWindow.chosenTeam=false
                }
            }
        }
        Button {
            id: admin
            height: 200
            width: 605
            text: "Admin"
            anchors.horizontalCenter: parent.horizontalCenter

        }
        }
    }
}
