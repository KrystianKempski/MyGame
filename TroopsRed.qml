import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import Qt.labs.calendar 1.0
import QtQuick.Layouts 1.3
import Qt.labs.qmlmodels 1.0

Page {
    property bool changed: false
    property int widthProp
    property int heightProp
    property int cellHeight: 50
    property int cellWidth: 100
    id:troopsRed
    Item {
        id: row
        width: parent.width//widthProp
        height: parent.height//heightProp
        Column{
            anchors.fill: parent
            Button {
                id: button
                text: "add"
                onClicked: {
                    dataSource.fetchTroops(true)
                }
            }

            ScrollView{
                id: scrollView1
                width: parent.width
                height: parent.height - button.height-button2.height
                clip: true
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                contentWidth: headerRec.width
                contentHeight: 600
                Item{

                    id: headerRec
                    width: cellWidth*26
                    height: cellHeight
                    TableView {
                        interactive: false
                        id: header1
                        anchors.fill: parent
                        model: TableModel {
                            TableModelColumn{ display: "NAME" }
                            TableModelColumn { display: "TYPE" }
                            TableModelColumn { display: "MAX_HP" }
                            TableModelColumn { display: "A_VAL" }
                            TableModelColumn { display: "DEF" }
                            TableModelColumn { display: "D_DICE" }
                            TableModelColumn { display: "DMG" }
                            TableModelColumn { display: "SPEED" }
                            TableModelColumn { display: "RANGE" }

                            rows: [
                                {
                                    "NAME": "nazwa",
                                    "TYPE": "typ",
                                    "MAX_HP": "HP",
                                    "A_VAL": "Atak",
                                    "DEF": "Obrona",
                                    "D_DICE": "kość obr.",
                                    "DMG": "obrażenia",
                                    "SPEED": "prędkość",
                                    "RANGE": "zasięg",
                                }
                            ]
                        }
                        delegate: Rectangle {
                            id:headerCell
                            color: "red"
                            border.color: "black"
                            implicitWidth: cellWidth
                            implicitHeight: cellHeight
                            Text {
                                text: display
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
                    Item{
                        id: troopsTable
                        width: cellWidth*9
                        height: 500
                        anchors.top: headerRec.bottom
                        anchors.left: headerRec.left
                        TableView {
                            interactive: false
                            id:tableView2
                            anchors.fill: parent
                            clip: true
                            model: troopModelRed
                            delegate: Rectangle {
                                color: "red"
                                border.color: "black"
                                implicitWidth: cellWidth
                                implicitHeight: cellHeight
                                TextField {
                                    id:textField
                                    anchors.fill: parent
                                    text: model.troops
                                    onAccepted: {
                                        model.troops = text
                                        troopModelRed.updateServer()
                                    }

                                }
                            }

                        }
                    }
                    Repeater{

                        model: troopModelRed
                        delegate: Button{
                            text: "Resetuj oddział"
                            height: cellHeight-10
                            font.pixelSize: 10
                            width: 100
                            x:cellWidth*9
                            y:cellHeight + model.index * cellHeight +5
                            onClicked: gamelogic.resetTroop(model.index,true)
                        }
                    }
            }
            Button {
                id: button2
                text: "potwierdzam"
                onClicked: {
                }
            }
        }
    }
}


