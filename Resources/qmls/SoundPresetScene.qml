import QtQuick 2.5
import push2.enums 1.0

Item {
    id: display
    width: 960
    height: 160

    Push2DeviceOverlay {
        id: push2DeciceOverlay

        Component.onCompleted:{
            push2DeciceOverlay.onReserveButton(Button.E_BtnRecord)
            push2DeciceOverlay.onReserveLedOfBtn("red", Button.E_BtnRecord)
            push2DeciceOverlay.onReserveButton(Button.E_BtnDelete)
            push2DeciceOverlay.onReserveLedOfBtn("red", Button.E_BtnPlay)
            push2DeciceOverlay.onReserveButton(Button.E_BtnPlay)
            push2DeciceOverlay.onReserveLedOfBtn("red", Button.E_BtnDelete)
            push2DeciceOverlay.onReserveEncoder(Encoder.E_EncoderTempo)
        }
        onButtonPressed: {
            switch(id) {
                case Button.E_BtnRecord:{
                    soundDeviceParameter.soundPresetModel.storeAsSoundPreset("")
                    break
                }
                case Button.E_BtnPlay:{
                    soundDeviceParameter.soundPresetModel.resetToActualSoundPreset();
                    break
                }
                case Button.E_BtnDelete:{
                    soundDeviceParameter.soundPresetModel.deleteSoundPreset()
                    break
                }
            }
        }
        onEncoderIncrementChanged: {
            switch(id) {
                case Encoder.E_EncoderTempo: {
                    var newIdx = soundPresetListView.currentIndex + increment
                    if(newIdx < 0 || newIdx >= soundPresetListView.count) return
                    soundDeviceParameter.soundPresetModel.selectSoundPreset(newIdx)
                    soundPresetListView.currentIndex = newIdx; 
                    break
                }
            }
        }
        onEncoderTouchStateChanged: {
            if(EncoderTouchState.Released == state) {
                switch(id)
                {
                    case Encoder.E_EncoderTempo:
                    {
                        display.destroy()
                        break
                    }
                }
            }
        }
    }

    Rectangle {
        height: display.height
        width: display.width / 4
        color: "black"
        border.color: "#151515"
        border.width: 3
        radius: 3
        opacity: 0.0

        Behavior on opacity { PropertyAnimation { properties: "opacity"; easing.type: Easing.InOutQuad } }
        Component.onCompleted:{
            opacity = 0.95
        }
        Component.onDestruction:{
            opacity = 0.0
        }
        Text {
            anchors.fill: parent
            visible: soundPresetListView.count == 0
            text: "No Sound-device presets"
            color: "white"
            wrapMode: Text.WordWrap
            font.pointSize: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        ListView {
            id: soundPresetListView
            anchors.fill: parent
            visible: count != false
            anchors.margins: 5
            model: soundDeviceParameter.soundPresetModel
            orientation: Qt.Vertical
            spacing: 0
            currentIndex: soundDeviceParameter.soundPresetModel.currentIndex
            clip: true
            delegate: Component{
                Rectangle {
                    width: parent.width
                    height: soundPresetListViewText.implicitHeight
                    color: "black"
                    Text {
                        id: soundPresetListViewText
                        anchors.fill: parent
                        text: name
                        color: parent.ListView.isCurrentItem ? "white" : "gray"
                        font.pointSize: parent.ListView.isCurrentItem ? 20 : 16
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                } 
            }
        }
    }
}