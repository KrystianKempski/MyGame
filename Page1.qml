
import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQml.Models 2.1
import Qt.labs.qmlmodels 1.0

Page {
    id: page1
    visible: true
    
    property int widthProp
    property int heightProp
    width: widthProp
    height: heightProp
    title: qsTr("Hello World")
    property int cellSize: 64
    Item {
        id: leftToolbar
        width: 200
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 0
        Component.onCompleted: dataSource.writeConsole("nowa gra\r\n")
        // spacing: -1
        Rectangle{
            x: 0
            y:0
            id: leftBox1
            width: leftToolbar.width
            height: 200
            color: "red"
            opacity: 0.6
        }
        Rectangle{
            x: 0
            y: leftBox1.height
            width: leftToolbar.width
            height: leftToolbar.height-leftBox1.height
            color: "black"
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
                        text: dataSource.readConsole
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
        width: page1.width-400
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
        Button {
            anchors.bottom: btnReady.top
            id: btnAddTroops
            text: "add"
            onClicked: {
                dataSource.fetchTroops(true)
            }
        }
        Button {
            anchors.bottom: rightToolBar.bottom
            id: btnReady
            text: "Ready"
            onClicked: {
                gamelogic.playerReady(1)
                gamelogic.nextTurn()
            }
        }

    }

}



