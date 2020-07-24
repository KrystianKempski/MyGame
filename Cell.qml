import QtQuick 2.0

DropArea {
    id: cell

    property string colorKey
    property string tokenIn: model.colorRole
    property alias dropProxy: cell
    property int row: model.row
    property int column: model.column
    property int indeks: model.index
   // property bool isAvalible:  model.avalibleRole // dataSource.getCells(row,column)
   // property bool isOccupied: true
    //property bool occColor: model.color
    property int size
    implicitWidth: size
    implicitHeight: size
   // Component.onCompleted: console.log(tokenIn)
    keys: model.colorRole === "grey" ? [colorKey]:  ["null"]
    Rectangle {
       // property bool warunki: cell.containsDrag && tokenIn
        //Component.onCompleted: console.log(isAvalible)
        id: cellRect
        border.color: "black"

        anchors.fill: parent
        color: model.colorRole
        opacity: 0.6
        Text {
            id: text
            text: model.tokenInRole
        }
       // Component.onCompleted: //console.log(model.colorRole)


        states:
            State {
            when: cell.containsDrag //model.tokenIn //=== true
            onCompleted:  {
                 //console.log("token in "+ model.colorRole )
                //   dataSource.setTokenIn(row,column,true)
                //   cell.tokenIn= model.tokenIn
            }
            PropertyChanges {
              //  target: cellRect
               // color: "grey"
            }

        }

    }

}

