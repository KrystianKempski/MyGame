import QtQuick 2.12
import QtQml.Models 2.1
import QtQuick.Controls 2.5

Rectangle {
    id: token
    property string colorKey
    property bool team: false
    property int size
    property var enemies
    property bool editable
    property int trIndex: model.row
    property var troopName: model.name
    property var troopSpeed: model.speed
    property int trRow:  model.rowTr
    property int trCol: model.colTr
    property int b_rowX: model.rowTr*size
    property int b_columnX: model.colTr*size
    property int hp: model.hp
    property bool troopExists: model.active//troopModelBlue.data(troopModelBlue.index(model.row,23),258)          //check if troop is on battlefield
    property double barProgress: hp/troopModelBlue.data(troopModelBlue.index(trIndex,24),258)
    property var temp

    opacity: troopExists? 1 : 0.5
    implicitWidth: size
    implicitHeight: size
    radius: size
    y: b_rowX
    x: b_columnX
    Component.onCompleted: {
        //battleModel2.setData(battleModel2.index(model.rowTr,model.colTr),true,258)
        battleModel2.cellAvalible(model.rowTr,model.colTr,true)
    }
    color: "blue"
    Drag.keys: [ colorKey ]
    Drag.active: mouseArea.drag.active
    Drag.hotSpot.x: size/2
    Drag.hotSpot.y: size/2

    Text {

        id: text1
        anchors.fill: parent
        color: "white"
        font.pixelSize: 12
        text: troopName

        horizontalAlignment:Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Rectangle {
        id: hpBar
        x:0
        y:size-15
        color: "lime"
        border.color: "black"
        border.width: 1
        width: parent.width
        height: 15
        Rectangle{
            id: redbar
            x:parent.width-redbar.width
            y:1
            color: "red"
            height: parent.height -2
            width: barProgress>0? parent.width-parent.width*barProgress :parent.width
        }
        Text {
            id: hpValue
            color: "white"
            anchors.centerIn: parent
            text: hp+"/"+troopModelBlue.data(troopModelBlue.index(trIndex,24),258)
        }
    }
    MouseArea {

        visible: troopExists? editable : false
        id: mouseArea
        acceptedButtons: Qt.AllButtons
        width: size-2; height: size-2
        anchors.centerIn: token
        drag.target:  (mouseArea.pressedButtons &Qt.LeftButton) ? token: null
        states: State {
            when: mouseArea.drag.active
            onCompleted:  {
                //  console.log(model.rowTr)
                b_columnX= model.colTr*size
                b_rowX = model.rowTr*size
                token.z=4
                battleModel2.colorCells(model.rowTr,model.colTr,model.speed,1,model.moved)
            }
        }
        onPressed: {
            if(mouseArea.pressedButtons & Qt.RightButton){
                actionMenu.popup()

            }
        }
        onReleased:{
            temp = token.Drag.target                                                                //sprawdzenie czy token ląduje w dozwolonym miejscu
            token.x= temp !== null ?  token.Drag.target.x : b_columnX
            token.y= temp !== null ?  token.Drag.target.y : b_rowX
            token.z=1
            battleModel2.colorCells(model.rowTr,model.colTr,model.speed,0,model.moved)              //pomalowanie kafelków na przezroczyste
            if(temp!== null){
                //battleModel2.setData(battleModel2.index(model.rowTr,model.colTr),false,258)         //odznaczenie kafelka jako wolny
                battleModel2.cellAvalible(model.rowTr,model.colTr,false)
                model.rowTr=token.y/size
                model.colTr=token.x/size
                model.moved=1                                                                           //zaznaczanie że oddział się ruszył
                troopModelBlue.updateAll()                                                             //update modelu i serwera
            }
        }
    }
    states: State {
        when: troopExists==false
        onCompleted:  {
           battleModel2.setData(battleModel2.index(model.rowTr,model.colTr),false,258)
           model.colTr=9
           model.rowTr=9
   }
    }
    Menu{
        id: actionMenu
        Menu{
            title: "Atak"
            Repeater {
                model: troopModelBlue.findEnemy(trRow,trCol,1,true)
                MenuItem {
                    text: modelData
                    onClicked: gamelogic.attack(trIndex,modelData,team)

                }
            }
        }
        MenuItem { text: "Obrona" }
    }

}



