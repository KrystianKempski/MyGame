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
    property var troopRange: model.range
    property string troopIcon: "/images/"+model.type+".png"
    property int trRow:  model.rowTr
    property int trCol: model.colTr
    property int b_rowX: model.rowTr*size
    property int b_columnX: model.colTr*size
    property int hp: model.hp
    property bool troopExists: model.active                                                        //check if troop is on battlefield
    property double barProgress: hp/troopModelBlue.data(troopModelBlue.index(trIndex,2),258)
    property var temp

    opacity: troopExists? 1 : 0.5
    implicitWidth: size
    implicitHeight: size
    radius: size
    y: b_rowX
    x: b_columnX
    Component.onCompleted: {
        battleModel2.setCellAvailability(model.rowTr,model.colTr,true)
    }
    color: "blue"
    Drag.keys: [ colorKey ]
    Drag.active: mouseArea.drag.active
    Drag.hotSpot.x: size/2
    Drag.hotSpot.y: size/2
    Image {
        id: icon
        width: size/2
        height: size/2
        anchors.centerIn: parent
        source: troopIcon
    }

    Text {
        id: text1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        color: "black"
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
            text: hp+"/"+troopModelBlue.data(troopModelBlue.index(trIndex,2),258)
        }
    }
    MouseArea {

        visible: troopExists&& !dataSource.teamTurn? editable : false
        id: mouseArea
        acceptedButtons: Qt.AllButtons
        width: size-2; height: size-2
        anchors.centerIn: token
        drag.target:  (mouseArea.pressedButtons &Qt.LeftButton)& !model.moved ? token: null
        states: State {
            when: mouseArea.drag.active
            onCompleted:  {
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
                battleModel2.setCellAvailability(model.rowTr,model.colTr,false)                              //odznaczenie kafelka jako wolny
                model.rowTr=token.y/size
                model.colTr=token.x/size
                model.moved=true                                                                           //zaznaczanie że oddział się ruszył
                model.attacks=model.attacks-1
                troopModelBlue.updateAll()                                                             //update modelu i serwera
            }
        }
    }
    states: State {
        when: troopExists==false
        onCompleted:  {
           battleModel2.setData(battleModel2.index(model.rowTr,model.colTr),false,258)
           model.colTr=0
           model.rowTr=14
   }
    }
    Menu{
        id: actionMenu
        Menu{
            title: "Atak"
            Repeater {
                model: gamelogic==null? 0: gamelogic.findEnemy(trRow,trCol,troopRange,true)
                MenuItem {
                    text: modelData
                    onClicked: gamelogic.attack(trIndex,modelData,team)
                }
            }
        }
        MenuItem { text: "Obrona" }
    }

}



