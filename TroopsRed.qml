import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import Qt.labs.calendar 1.0
import QtQuick.Layouts 1.3
import QtQml.Models 2.12
import Qt.labs.qmlmodels 1.0

Page {
    property bool team: true
    property int cellHeight: 50
    property int cellWidth: 120
    property bool newTroops: false
    id:troopsRed

    Rectangle {
        id: row
        width: parent.width
        height: parent.height

        Column{
            anchors.fill: parent
            anchors.topMargin: 3
            spacing: 3
            Row {
                spacing: 3
                Button {
                    id: btnGetTroops
                    text: "Pobierz oddziały"
                    onClicked: {
                        dataSource.fetchTroops(team)
                        btnAddTroop.enabled = true
                        btnReady.enabled=true
                        newTroops=false
                    }
                }
                Button {
                    id: btnClearTroops
                    text: "Stwórz nowe oddziały"
                    onClicked: {
                        gamelogic.clearAllTroops()
                        btnAddTroop.enabled = true
                        btnReady.enabled=true
                        newTroops=true
                    }
                }
                CheckBox{
                    id: newGame
                    text: "Nowa gra?"
                    checked: false
                }
                CheckBox{
                    id: resetPosition
                    text: "Zresetować pozycje?"
                    checked: false
                }
            }
            ScrollView{
                id: scrollView1
                width: parent.width
                height: parent.height
                clip: true
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                contentWidth: headerRec.width
                contentHeight: 600

                Item{

                    id: headerRec
                    width: cellWidth*9
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
                    implicitHeight: tableView2.implicitHeight
                    implicitWidth: tableView2.implicitWidth
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
                        text: "Reset"
                        height: cellHeight-10
                        font.pixelSize: 10
                        width: 60
                        x:cellWidth*9+5
                        y:cellHeight + model.index * cellHeight +5
                        onClicked: gamelogic.resetTroop(model.index,team)
                    }
                }
                Repeater{
                    id: rmvButtons
                    model: troopModelRed
                    delegate: Button{
                        text: "Usuń"
                        height: cellHeight-10
                        font.pixelSize: 10
                        width: 60
                        x:cellWidth*9+70
                        y:cellHeight + model.index * cellHeight +5
                        onClicked: gamelogic.removeTroop(team,model.index)
                    }
                }
                Button {
                    id: btnAddTroop
                    enabled: false
                    y:(rmvButtons.count+1)*cellHeight
                    text: "Dodaj oddział"
                    onClicked: {
                        gamelogic.addTroop(team,rmvButtons.count)
                    }
                }
            }

        }
    }
    Button{
        id: btnReady
        enabled: false
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text:  "GOTOWE"
        width: 150
        height: 150
        font.pixelSize: 18
        onClicked: {
            if(newTroops) gamelogic.resetAllTroops(team)
            if(newGame.checkState) {
                gamelogic.startNewGame()
                gamelogic.resetTroopsPosition(team)
                gamelogic.resetTroopsPosition(!team)
                gamelogic.resetAllTroops(team)
                gamelogic.resetAllTroops(!team)
            }
                if(resetPosition.checkState){
                gamelogic.resetTroopsPosition(team)
                gamelogic.resetAllTroops(team)
            }
            btnReady.enabled=false
            stackView.push("BattlefieldRed.qml")
            gamelogic.waitForTurn(team)
        }
    }
}


