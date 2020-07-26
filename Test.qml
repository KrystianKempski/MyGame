
import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQml.Models 2.1
import Qt.labs.qmlmodels 1.0

Item {
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
                contentWidth: txtConsole.width
                contentHeight: txtConsole.height
                flickableDirection: Flickable.VerticalFlick
                clip: true

                TextArea.flickable: TextArea {
                    anchors.fill: parent
                    id:txtConsole
                    font.pixelSize: 12
                    color:  "black"
                   // focus: true
                    //readOnly: true
                    // text: dataSource.readConsole
                    wrapMode: TextArea.Wrap
                    text:" dataSource.readConsole"

                    onTextChanged: {
                        txtConsole.append(dataSource.readConsole)
                        //y: txtConsole.contentHeight-flickable.height
                        //      flickable.contentHeight= txtConsole.contentHeight
                        //flickable.contentY= txtConsole.contentHeight-flickable.height//console.log(txtConsole.contentHeight) //{flickable.contentY=flickable.contentHeight-txtConsole.w; console.log(flickable.contentY)} //{ console.log(flickable.contentHeight);console.log(txtConsole.height)}// flickable.flick(0, 100)//txtConsole.height-flickable.contentHeight//{ console.log(flickable.contentHeight);console.log(txtConsole.height)}//
                        if (txtConsole.height > flickable.height) {
                            flickable.contentY = txtConsole.height - flickable.height
                            console.log("OnTextChanged - contentY changed to: " + flickable.contentY)
                        }
                    }
                }
                contentY: txtConsole.height

                ScrollBar.vertical: ScrollBar { }
            }
        }
    }
}
