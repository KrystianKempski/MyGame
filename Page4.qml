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
    id:page4
    Item {
        id: row
        width: widthProp
        height: heightProp
        Column{
            anchors.fill: parent
            Button {
                id: button
                text: "add"
                onClicked: {
                    dataSource.fetchTroops(true)
                }
            }
          //  Flickable {

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
                            TableModelColumn { display: "HP" }
                            TableModelColumn { display: "A_VAL" }
                            TableModelColumn { display: "DEF" }
                            TableModelColumn { display: "D_DICE" }
                            TableModelColumn { display: "DMG" }
                            TableModelColumn { display: "A_TYPE" }
                            TableModelColumn { display: "SPEED" }
                            TableModelColumn { display: "A_CHARGE" }
                            TableModelColumn { display: "A_CROWD" }
                            TableModelColumn { display: "MORALE" }
                            TableModelColumn { display: "RANGE" }
                            TableModelColumn { display: "STR" }
                            TableModelColumn { display: "AGI" }
                            TableModelColumn { display: "END" }
                            TableModelColumn { display: "WILL" }
                            TableModelColumn { display: "ROW" }
                            TableModelColumn { display: "COL" }
                            TableModelColumn { display: "T_ID" }
                            TableModelColumn { display: "TEAM" }
                            TableModelColumn { display: "MOVED" }
                            TableModelColumn { display: "ATTACKED" }
                            TableModelColumn { display: "ACTIVE" }
                            TableModelColumn { display: "MAX_HP" }
                            TableModelColumn { display: "BLANK3" }

                            rows: [
                                {
                                    "NAME": "nazwa",
                                    "TYPE": "typ",
                                    "HP": "HP",
                                    "A_VAL": "Atak",
                                    "DEF": "Obrona",
                                    "D_DICE": "kość obr.",
                                    "DMG": "obrażenia",
                                    "A_TYPE": "typ obr.",
                                    "SPEED": "prędkość",
                                    "A_CHARGE": "bonus z szarży",
                                    "A_CROWD": "atak w ścisku",
                                    "MORALE": "morale",
                                    "RANGE": "zasięg",
                                    "STR": "siła",
                                    "AGI": "zręczność",
                                    "END": "wytrzymałość",
                                    "WILL": "wola",
                                    "ROW": "rząd",
                                    "COL": "kolumna",
                                    "T_ID": "id",
                                    "TEAM": "drużyna",
                                    "MOVED": "poruszona",
                                    "ATTACKED": "zaatakowano",
                                    "ACTIVE": "aktywna",
                                    "MAX_HP": "max hp",
                                    "BLANK3": "puste"
                                }
                            ]
                        }
                        delegate: Rectangle {
                            id:headerCell
                            color: "blue"
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
                        width: cellWidth*26
                        height: 500
                        anchors.top: headerRec.bottom
                        anchors.left: headerRec.left
                        TableView {
                            interactive: false
                            id:tableView2
                            anchors.fill: parent
                            clip: true
                            model: troopModelBlue
                            delegate: Rectangle {
                                color: "blue"
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
             //  }





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


