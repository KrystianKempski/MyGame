import QtQuick 2.0

DropArea {
    id: cell
    property string colorKey
    property string tokenIn: model.colorRole
    property alias dropProxy: cell
    property int row: model.row
    property int column: model.column
    property int indeks: model.index
    property int size
    implicitWidth: size
    implicitHeight: size
    keys: model.colorRole === "grey" ? [colorKey]:  ["null"]
    Rectangle {
        id: cellRect
        border.color: "black"
        anchors.fill: parent
        color: model.colorRole
        opacity: 0.6
        states:
            State {
            when: cell.containsDrag
            PropertyChanges {
                target: cellRect
                border.width: 3
                opacity:1
            }

        }

    }

}

