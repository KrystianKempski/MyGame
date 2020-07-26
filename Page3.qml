
import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQml.Models 2.1
import Qt.labs.qmlmodels 1.0

Page {
    id: page3
    visible: true

    property int widthProp
    property int heightProp
    width: widthProp
    height: heightProp
    title: qsTr("Hello World")
    property int cellSize: 64
    Column{
        id: leftToolbar
        width: 200
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 0
        spacing: -1
        Rectangle{
            x: 0
            y:0
            id: leftBox1
            width: leftToolbar.width
            height: 200
            color: "blue"
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
                //width: parent.width-10
                // height: parent.height-200
                //width: parent.width-10
                color: "white"
                Flickable {
                    id: flickable
                    anchors.fill: parent

                    TextArea.flickable: TextArea {
                        font.pixelSize: 12
                        color:  "black"
                        readOnly: true
                        text: dataSource.readConsole
                        wrapMode: TextArea.Wrap
                    }

                    ScrollBar.vertical: ScrollBar { }
                }
            }


        }
    }
    Item {
        id: root
        anchors.left: leftToolbar.right
        width: page3.width-400
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
                implicitHeight: cellSize*10
                implicitWidth: cellSize*10
                 clip: true

                model: battleModel2
                delegate: Cell { colorKey: "blue"; size: cellSize }
            }


            Repeater {
                z: 1
                model: troopModelRed
                delegate: TokenRed { colorKey: "red"; size: cellSize; editable: false}
            }
            Repeater {
                z: 2
                model: troopModelBlue
                delegate: TokenBlue { colorKey: "blue"; size: cellSize; editable: true}
            }

        }

    }

}



