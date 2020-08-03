
import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQml.Models 2.1
import Qt.labs.qmlmodels 1.0

Page {
    id: battlefieldBlue
    visible: true

    property int widthProp
    property int heightProp
    width: parent.width//widthProp
    height: parent.height//heightProp
    title: qsTr("Hello World")
    property int cellSize: 64

    Item {
        id: leftToolbar
        width: 200
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 0
        Rectangle{
            id: leftBox1
            width: leftToolbar.width
            height: leftToolbar.height-consoleBox.height
            color: "royalblue"
            Repeater {
                model: troopModelBlue
                delegate: Rectangle {
                    width: parent.width-10
                    height: 60
                    border.color: "black"
                    color: model.active? "blue" : "midnightblue"
                    opacity: model.active? 1 : 0.6
                    y: 5+model.index*(height+5)
                    x: 5
                    Image {
                        id: icon
                        source: "/images/"+model.type+".png"
                        x:5
                        width: parent.height/2
                        height: parent.height/2
                        anchors.verticalCenter: parent.verticalCenter

                    }

                    Text {
                        id: name
                        anchors.left: icon.right
                        anchors.top: parent.top
                        width: parent.width-icon.width-20
                        text: model.name
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 16


                    }
                    property int margin: 3
                    property int fontSize: 12
                    property color statColor: "white"
                    Text {
                        id: attackTxt
                        anchors.top: name.bottom
                        anchors.left: icon.right
                        anchors.margins: margin

                        text: "At: "+model.attackVal
                        font.pixelSize: fontSize
                        color: statColor

                    }
                    Text {
                        id: defenceTxt
                        anchors.top: name.bottom
                        anchors.left: attackTxt.right
                        anchors.margins: margin
                        text: "Def: "+model.defence
                        font.pixelSize: fontSize
                        color: statColor

                    }
                    Text {
                        id: dmgTxt
                        anchors.top: name.bottom
                        anchors.left: defenceTxt.right
                        anchors.margins: margin
                        text: "Dmg: 1k"+ model.dmgDice+"+"+model.dmgVal
                        font.pixelSize: fontSize
                        color: statColor
                    }
                    Text {
                        id: speedTxt
                        anchors.top: attackTxt.bottom
                        anchors.left: icon.right
                        anchors.margins: margin
                        text: "Speed: "+model.speed
                        font.pixelSize: fontSize
                        color: statColor
                    }
                    Text {
                        id: rangeText
                        anchors.top: attackTxt.bottom
                        anchors.left: speedTxt.right
                        anchors.margins: margin
                        text: "Range: "+model.range
                        font.pixelSize: fontSize
                        color: statColor
                    }
                }
            }
        }
        Rectangle{
            id: consoleBox
            x: 0
            y: leftBox1.height
            width: leftToolbar.width
            height:210
            color: "gray"
            Rectangle{
                id: console1
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.bottomMargin: 5
                width: parent.width-10
                height: 200
                radius: 5
                color: "white"
                Flickable {
                    id: flickable
                    anchors.fill: parent
                    contentWidth: parent.width
                    contentHeight: 500
                    flickableDirection: Flickable.VerticalFlick
                    clip: true
                    onContentHeightChanged: Qt.callLater(() => contentY = contentHeight - height)
                    TextArea.flickable: TextArea {
                        id:txtConsole
                        font.pixelSize: 12
                        width: parent.width
                        height: 200
                        color:  "black"
                        wrapMode: TextArea.Wrap
                        text:dataSource.console

                        onTextChanged: {
                            flickable.contentY = txtConsole.height - flickable.height
                        }
                    }
                    contentY: contentHeight-height
                    ScrollBar.vertical: ScrollBar { }
                }
            }


        }
    }
    Item {
        id: root
        anchors.left: leftToolbar.right
        width: battlefieldBlue.width-400
        height: parent.height//-cellSize
        ScrollView{
            id: view
            height: parent.height
            width: parent.width
            anchors.fill: parent
            clip: true
            contentWidth: image.width
            contentHeight: image.height

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            Image {
                id: image
                width: 1000
                height: 1000
                source: "qrc:/images/field.jpg"
            }
            TableView {
                id: tableview
                implicitHeight: cellSize*dataSource.getCellRowCount()
                implicitWidth: cellSize*dataSource.getCellColumnCount()
                clip: true
                model: battleModel2
                delegate: Cell { colorKey: "blue"; size: cellSize }
            }

            Repeater {
                model: troopModelRed
                delegate: TokenRed { colorKey: "red"; size: cellSize; editable: false}
            }
            Repeater {
                model: troopModelBlue
                delegate: TokenBlue { colorKey: "blue"; size: cellSize; editable: true}
            }
        }
    }
    Rectangle {
        id:rightToolBar
        anchors.left: root.right
        width: 200
        height: parent.height
        color: "grey"
        Rectangle{
            id: chat1
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.topMargin: 5
            width: parent.width-10
            height: parent.height-400
            radius: 5
            color: "white"
            Flickable {
                id: flickable2
                anchors.fill: parent
                contentWidth: parent.width
                contentHeight: 500
                flickableDirection: Flickable.VerticalFlick
                clip: true
                onContentHeightChanged: Qt.callLater(() => contentY = contentHeight - height)
                TextArea.flickable: TextArea {
                    id:txtChat
                    font.pixelSize: 12
                    width: parent.width
                    height: 200
                    color:  "black"
                    wrapMode: TextArea.Wrap
                    text: dataSource.chat
                    readOnly: true
                    onTextChanged: {
                        flickable2.contentY = flickable2.height - flickable2.height
                    }
                }
                contentY: contentHeight-height
                ScrollBar.vertical: ScrollBar { }
            }
        }
        TextField {
            id: chatInput
            anchors.top: chat1.bottom
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.topMargin: 5
            width: parent.width-10
            height: 30
            color: "black"

        }
        Text {
            id: txtTeamTurn
            visible: dataSource.turn >=0? true : false
            anchors.bottom: btnAddTroops.top
            anchors.left: rightToolBar.left
            text: dataSource.teamTurn ? "tura czerwonych" : "tura niebieskich"
        }
        Text {
            id: txtTurn
            visible: dataSource.turn >=0? true : false
            anchors.bottom: btnAddTroops.top
            anchors.left: txtTeamTurn.right
            text: dataSource.turn >=0 ? dataSource.turn : "-"
        }
        Button {
            id: btnSend
            anchors.top: chatInput.bottom
            anchors.topMargin: 4
            anchors.right: chatInput.right
            text: "send"
            onClicked: {
                dataSource.writeChat("Blue: " +chatInput.text)
                chatInput.text=""
            }
        }
        Button {
            anchors.bottom: btnReady.top
            id: btnAddTroops
            text: "add"
            onClicked: {
                dataSource.fetchTroops()
            }
        }
        Button {
            anchors.bottom: rightToolBar.bottom
            id: btnReady
            enabled: dataSource.turn >=0 ? !dataSource.teamTurn : false
            text: "End Turn"
            onClicked: {
                gamelogic.endTurn(false)
            }
        }

    }

}



