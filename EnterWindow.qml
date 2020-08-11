import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
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
                    text: "Drużyna Czerwona"
                    height: 200
                    width: 300
                    onClicked: {
                        stackView.push("TroopsRed.qml")
                        mainWindow.chosenTeam=true
                        menuFile.enabled=true
                    }
                    contentItem: Text{
                        text: teamRed.text
                        color: "black"
                        font.pixelSize: 20
                         font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    background: Rectangle {
                        anchors.fill: teamRed
                        color: "red"
                    }
                }
                Button {
                    id: teamBlue
                    text: "Drużyna Niebieska"
                    height: 200
                    width: 300
                    contentItem: Text{
                        text: teamBlue.text
                        color: "black"
                        font.bold: true
                        font.pixelSize: 20
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    background: Rectangle {
                        anchors.fill: teamBlue
                        color: "blue"
                    }
                    onClicked: {
                        stackView.push("TroopsBlue.qml")
                        mainWindow.chosenTeam=false
                        menuFile.enabled=true
                    }
                }
            }
            //        Button {
            //            id: admin
            //            height: 200
            //            width: 605
            //            text: "Admin"
            //            anchors.horizontalCenter: parent.horizontalCenter

            //        }
        }
    }
}
