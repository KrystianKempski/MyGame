
import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.2
import QtQml.Models 2.1
import Qt.labs.qmlmodels 1.0

Page {
    id: battlefieldRed
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

        // spacing: -1
        Rectangle{
            id: troopView
            width: leftToolbar.width
            height: leftToolbar.height-consoleBox.height
            color: "lightcoral"
            Repeater {
                model: troopModelRed
                delegate: Rectangle {
                    width: parent.width-10
                    height: 60
                    border.color: "black"
                    color: model.active? "red" : "maroon"
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
        //  }
        Rectangle{
            id: consoleBox
            x: 0
            y: troopView.height
            width: leftToolbar.width
            height: 210
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
                        //width: parent.width
                        readOnly: true
                        height: 200
                        color:  "black"
                        wrapMode: TextArea.Wrap
                        text: dataSource.console
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
        width: battlefieldRed.width-400
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
                delegate: Cell { colorKey: "red"; size: cellSize }
            }
            Repeater {
                model: troopModelBlue
                delegate: TokenBlue{ colorKey: "blue"; size: cellSize; editable: false}
            } Repeater {

                model: troopModelRed
                delegate: TokenRed { colorKey: "red"; size: cellSize; editable: true}
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
            //readOnly: false
            anchors.top: chat1.bottom
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.topMargin: 5
            width: parent.width-10
            //placeholderText: "wpisz wiadomość"
            height: 30
            //radius: 5
            color: "black"

        }

          Rectangle{
            anchors.bottom: endTurn.top
            anchors.horizontalCenter: rightToolBar.horizontalCenter
            anchors.margins: 5
           color: "beige"
           border.color: "black"
           border.width: 1
           width: 154
           height: 60

              Text {
                id: txtTeamTurn
                 wrapMode:  Text.WordWrap
                 width: parent.width
                 height: parent.height
                 horizontalAlignment:  Text.AlignHCenter
                 verticalAlignment: Text.AlignVCenter
                 font.bold: true
                visible: dataSource.turn >=0? true : false
                color: dataSource.teamTurn ? "red" : "blue"
                text: dataSource.teamTurn ? "TURA CZERWONYCH" : "TURA NIEBIESKICH"
            }
          }
          Row {
              id: endTurn
              anchors.bottom: parent.bottom
              anchors.horizontalCenter: rightToolBar.horizontalCenter
              spacing: 4
              anchors.bottomMargin: 5
          Rectangle{
              id: rectTurn
              anchors.margins: 2
              color: "beige"
              border.color: "black"
              border.width: 1

              width: 50
              height: btnReady.height
            Text {
                id: txtTurn
                wrapMode:  Text.WordWrap
                width: parent.width
                height: parent.height
                horizontalAlignment:  Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                visible: dataSource.turn >=0? true : false
                font.bold: true
                text: dataSource.turn >=0 ? "TURA \r\n"+dataSource.turn :"TURA \r\n -"
            }
          }
          Button {
              enabled: dataSource.turn >=0 ? dataSource.teamTurn : false
              id: btnReady
              height: 70
              text: "End turn"
              onClicked: {
                  gamelogic.endTurn(true)
              }

          }
        }
        Button {
            id: btnSend
            anchors.top: chatInput.bottom
            anchors.topMargin: 4
            anchors.right: chatInput.right
            text: "send"
            onClicked: {
                dataSource.writeChat("Red: " +chatInput.text)
                chatInput.text=""
            }
        }


    }

}



