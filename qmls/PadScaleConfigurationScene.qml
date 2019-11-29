import QtQuick 2.5
import push2.enums 1.0

Item {
    id: display
    width: 960
    height: 160

    Component.onCompleted:{
        push2Device.onSetLedOfBtn("white", Button.E_BtnUpArrow)
        push2Device.onSetLedOfBtn("white", Button.E_BtnDownArrow)
        push2Device.onSetLedOfBtn("white", Button.E_BtnLeftArrow)
        push2Device.onSetLedOfBtn("white", Button.E_BtnRightArrow)
    }
    Component.onDestruction:{
        push2Device.onSetLedOfBtn("black", Button.E_BtnUpArrow)
        push2Device.onSetLedOfBtn("black", Button.E_BtnDownArrow)
        push2Device.onSetLedOfBtn("black", Button.E_BtnLeftArrow)
        push2Device.onSetLedOfBtn("black", Button.E_BtnRightArrow)
    }

    Connections {
        target: push2Device
        onButtonPressed: {
            switch(id)
            {
                case Button.E_BtnT:
                {
                    if(1 <= x && x <= 6)
                    {
                        baseNotesGridView.currentIndex = x - 1
                    }
                    break
                }
                case Button.E_BtnB:
                {
                    if(1 <= x && x <= 6)
                    {
                        baseNotesGridView.currentIndex = x - 1 + 6
                    }
                    else if(x == 0)
                    {
                        push2Pads.toggleMode();
                    }
                    break
                }
                case Button.E_BtnUpArrow:
                {
                    musicScaleSelectGridView.moveCurrentIndexUp()
                    break
                }
                case Button.E_BtnDownArrow:
                {
                    musicScaleSelectGridView.moveCurrentIndexDown()
                    break
                }
                case Button.E_BtnLeftArrow:
                {
                    musicScaleSelectGridView.moveCurrentIndexLeft()
                    break
                }
                case Button.E_BtnRightArrow:
                {
                    musicScaleSelectGridView.moveCurrentIndexRight()
                    break
                }
                case Button.E_BtnOctaveUp:
                {
                    break
                }
                case Button.E_BtnOctaveDown:
                {
                    break
                }
                default:
                {
                    break
                }
            }
        }
        onEncoderIncrementChanged: {
            switch(id)
            {
                case Encoder.E_EncoderTempo:
                {
                    if(increment > 0)
                    {
                        musicScaleSelectGridView.moveCurrentIndexDown()
                    }
                    else if(increment < 0)
                    {
                        musicScaleSelectGridView.moveCurrentIndexUp()
                    }
                    break
                }
            }
        }
    }


    Rectangle{
        anchors.fill: parent
        color: "black"
        GridView {
            id: baseNotesGridView
            x: display.width / 8
            y: 0
            width: display.width * 6 / 8
            height: display.height
            cellWidth: display.width / 8
            cellHeight: display.height / 2

            flow: GridView.FlowLeftToRight

            model: musicScalesBaseNoteModel
            delegate: baseNotesDelegate
            currentIndex: -1
            highlightFollowsCurrentItem: true

            onCurrentIndexChanged:{
                push2Pads.setRootNote(currentIndex)
            }
            Component.onCompleted:{
                currentIndex = push2Pads.rootNote
            }

        }
        Component {
            id: baseNotesDelegate
            Rectangle {
                id: baseNotesDelegateRect
                color: "black"
                height: display.height / 2
                width: display.width / 8
                radius: 5
                Text {
                    anchors.fill: parent
                    text: name
                    color: baseNotesDelegateRect.GridView.isCurrentItem ? "white" : "#222222"
                    font.pointSize: baseNotesDelegateRect.GridView.isCurrentItem ? 14 : 10
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: index < 6 ? Text.AlignTop : Text.AlignBottom
                }
            }
        }

        GridView {
            id: musicScaleSelectGridView
            //anchors.fill: parent
            x: display.width / 8
            y: display.height / 6
            width: display.width * 7 / 8
            height: display.height * 4 / 6
            cellWidth: display.width / 8
            cellHeight: display.height / 6

            model: musicScalesModel
            flow: GridView.FlowTopToBottom
            layoutDirection: Qt.LeftToRight
            verticalLayoutDirection: GridView.TopToBottom
            currentIndex: -1
            highlightFollowsCurrentItem: true
            //highlight: musicScaleSelectDelegateHighlight
            delegate: musicScaleSelectDelegate

            onCurrentIndexChanged:{
                push2Pads.setMusicScale(currentIndex)
            }
            Component.onCompleted:{
                currentIndex = push2Pads.musicScale
            }

        }
        Component{
            id: musicScaleSelectDelegate
            Rectangle {
                id: wrapper
                height: display.height / 6
                width: display.width / 8
                //opacity: 0.8
                color: GridView.isCurrentItem?"white":"black"
                radius: 5
                Text {
                    anchors.fill: parent
                    text: name
                    color: wrapper.GridView.isCurrentItem?"black":"white"
                    //color: selected?"yellow":"white"
                    font.pointSize: 10
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
        Text {
            id: inKeyTxt
            x:10
            y:display.height * 0.8
            text: "InKey"
            color: push2Pads.mode ? "#222222" : "white" 
        }
        Text {
            id: chromaticTxt
            x: inKeyTxt.x + inKeyTxt.implicitWidth + 3
            y:display.height * 0.8
            text: "Chromatic"
            color: push2Pads.mode ? "white" : "#222222"
        }

        Column{
            anchors.right: parent.right
            Repeater {
                id: octaveRepeater
                model: 128 / MusicNote.NUM_NOTES_PER_OCTAVE
                Rectangle {
                    width: 50; height: display.height / octaveRepeater.count
                    border.width: 1
                    color: "black"
                    Rectangle{
                        anchors.fill: parent
                        anchors.margins : 6
                        color: push2Pads.octave == (octaveRepeater.count - index -1) ? "white" : "#222222"
                    }
                }
            }       
        }
    }
}