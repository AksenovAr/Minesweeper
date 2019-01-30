import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import MineSweeperModel 1.0
import QtQuick.Particles 2.12

ApplicationWindow {

    id: app
    title: qsTr("Minesweeper")
    width: 300
    height: 350
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width

    Popup{
        id :mypopup
        x: 0
        y: 0
        width: parent.width
        height: parent.height / 3
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        visible: mineSweeperModel.WIN ? showpopup : false
        background: null
        property bool timeelapsed: false

        GridLayout {
            id: grid
            anchors.fill: parent
            rows: 2
            columns: 2

            Button {
                id: message
                text: mypopup.timeelapsed ? qsTr("You lost!") : qsTr("You win!")
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.rowSpan: 1
                Layout.row: 1
                Layout.column: 1
                font.pixelSize: 20
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    color: "#f6f6f6"
                    border.color: "#26282a"
                    border.width: 1
                    radius: 4
                }
            }

            Button {
                id: restart
                text: "Restart"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                Layout.row: 2
                Layout.column: 1
                onClicked: {
                    mypopup.timeelapsed = false;
                    labelTest.timeinsec=0;
                    mineSweeperModel.clear();
                    gameTimer.restart();
                    mypopup.visible = false;
                }
                font.pixelSize: 20
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    color: restart.down ? "#d6d6d6" : "#f6f6f6"
                    border.color: "#26282a"
                    border.width: 1
                    radius: 4
                }
            }

            Button {
                id: exit
                text: "Close"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                Layout.row: 2
                Layout.column: 2
                onClicked: mypopup.visible = false;
                font.pixelSize: 20
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    color: exit.down ? "#d6d6d6" : "#f6f6f6"
                    border.color: "#26282a"
                    border.width: 1
                    radius: 4
                }
            }
        }
    }

    function showpopup() {
        mypopup.visible = true;
    }

    header: Rectangle {

        id: header
        height: 50
        color : "lightsteelblue"
        Row  {

            id: layout
            spacing: 2
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle {

                id: smileRect
                anchors.horizontalCenter : parent.horizontalCenter
                width: 50
                height: parent.height
                radius: 24
                color: smileRect.down ? "#d6d6d6" : "#f6f6f6"
                Image{

                    id: smile
                    source: showpopupboom()
                    anchors.centerIn: parent

                    function showpopupboom() {

                        if ( mineSweeperModel.BOOM ) {

                            mypopup.timeelapsed = true;
                            return "images/Minesweeper_sad.png";
                        }
                        else {

                            return "images/Minesweeper_smile.png";
                        }
                    }

                    MouseArea {

                        id: touchArea
                        anchors.fill: parent
                        onClicked: {

                            mypopup.timeelapsed = false;
                            labelTest.timeinsec=0;
                            mineSweeperModel.clear();
                            gameTimer.restart();
                        }
                    }
                }
            }
            //-------------------------
            Rectangle {

                id: rectTimer
                width: 70
                height: header.height - 5
                color: "#f6f6f6"
                border.color: "black"
                border.width: 2
                radius: 10
                anchors.leftMargin: 10
                anchors.verticalCenter : parent.verticalCenter
                Text  {
                    id: labelTest
                    font.pixelSize: 30
                    property int timeinsec: 0
                    text: timeinsec
                    anchors.centerIn: parent
                }
            }

            Timer {

                id: gameTimer
                interval: 1000
                repeat: true
                running: checkwin()
                onTriggered: {

                    labelTest.timeinsec++;
                    if ( labelTest.timeinsec == 180 )
                    {
                        //Invoke model time elapsed function
                        mineSweeperModel.timeElapsed();
                        mypopup.timeelapsed = true;
                        mypopup.visible = true;
                    }
                }

                function checkwin() {

                    if ( mineSweeperModel.BOOM === false && mineSweeperModel.WIN === false ) {

                        return true;
                    }
                    else {
                        mypopup.timeelapsed = false;
                        //mypopup.visible = true;
                        return false;
                    }
                }

            }
        }
    }

    Component {

        id: msdelegate
        Rectangle {

            id: context
            implicitWidth: 32
            implicitHeight: 32
            border.color: "black"
            border.width: 2
            radius: 10

            property int ro: row
            property int col: column

            Image {

                id: imagenCentro
                width: parent.implicitWidth
                height: parent.implicitHeight
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                source:  model.value
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.AllButtons;
                onClicked: {

                    var q_model_index = mineSweeperModel.index(ro, col, parent);
                    if (mouse.button === Qt.LeftButton) {

                        mineSweeperModel.setData(q_model_index, 0, 0);
                    }
                    if (mouse.button === Qt.RightButton) {

                        mineSweeperModel.setData(q_model_index, 1, 0);
                    }
                    console.log("data change successful?", q_model_index);
                }
            }
        }
    }

    Component {

        id: boomdelegate
        Rectangle {
            id: context
            implicitWidth: 32
            implicitHeight: 32
            border.color: "black"
            border.width: 2
            radius: 10
            Image {

                id: img
                width: parent.implicitWidth
                height: parent.implicitHeight
                anchors.centerIn: parent // or anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                source:  model.value
            }

            ParticleSystem {
                id : particles
                anchors.fill: parent
                running: model.value === "images/Minesweeper_boom.png" ? true : false
                // ![0]
                ImageParticle {
                    anchors.fill: parent
                    source: "images/star.jpg"
                    alpha: 0
                    alphaVariation: 0.2
                    colorVariation: 1.0
                }
                // ![0]
                Emitter {
                    anchors.centerIn: parent
                    emitRate: 90
                    lifeSpan: 1000
                    size: 12
                    sizeVariation: 8
                    velocity: AngleDirection {angleVariation: 360; magnitude: 12}
                }
            }
        }
    }

    TableView {
        id: tableView
        anchors.fill: parent
        rowSpacing: 1
        columnSpacing: 1
        interactive: false
        model: MineSweeperModel {

            id: mineSweeperModel
        }

        delegate :  mineSweeperModel.BOOM === false ? msdelegate : boomdelegate
    }

    Component.onCompleted: {
        mineSweeperModel.init();
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2
    }
}
