import QtQuick 2.5
import push2.enums 1.0

Item {
    id: display
    width: 960
    height: 160

    Connections {
        target: push2Device
        onButtonPressed: {
            switch(id)
            {
                case Button.E_BtnT:
                {
                    var _x = x + soundDeviceSelectionListView.indexAt(0,0)
                    if(0 <= _x &&_x < soundDeviceSelectionListView.count)
                    {
                        soundDeviceSelectionListView.currentIndex = _x
                    }
                    break
                }
                case Button.E_BtnB:
                {
                    var _x = x + voiceSelectionListView.indexAt(0,0)
                    if(0 <= _x &&_x < voiceSelectionListView.count)
                    {
                        soundDeviceAndVoiceSelection.setCurrentVoiceIndex(_x)
                    }
                    break
                }
            }
        }
        onEncoderIncrementChanged: {
            switch(id)
            {
                case Encoder.E_EncoderTempo:
                {
                    var idx = soundDeviceSelectionListView.indexAt(soundDeviceSelectionListView.contentX,1)
                    soundDeviceSelectionListView.positionViewAtIndex(idx + increment, ListView.Beginning);
                    break
                }
                case Encoder.E_EncoderMetronome:
                {
                    var idx = voiceSelectionListView.indexAt(voiceSelectionListView.contentX,1)
                    voiceSelectionListView.positionViewAtIndex(idx + increment, ListView.Beginning);
                    break
                }
            }
        }
    }


    Rectangle {
        anchors.fill: parent
        color: "black"
        
        ListView {
            id: soundDeviceSelectionListView
            width: parent.width
            height: display.height / 2
            anchors.top: parent.top
            model: soundDeviceAndVoiceSelection.soundDeviceSelectionModel
            orientation: Qt.Horizontal
            spacing: 4
            delegate: soundDeviceDelegate
            currentIndex: -1
            onCurrentIndexChanged:{
                soundDeviceAndVoiceSelection.setCurrentDeviceIndex(currentIndex)
            }
            Component.onCompleted:{
                currentIndex = soundDeviceAndVoiceSelection.currentDeviceIndex
            }
        }
        Component{
            id: soundDeviceDelegate
            Rectangle {
                id: wrapper
                height: display.height / 2
                width: (display.width / 8) - ListView.view.spacing
                color: ListView.isCurrentItem ?"white":"black"
                radius: 5
                
                Text {
                    anchors.fill: parent
                    text: name
                    color: wrapper.ListView.isCurrentItem ?"white":"grey"
                    font.pointSize: 10
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Image {
                    source: picturepath
                    anchors.fill: parent
                    anchors.margins: 3
                    opacity: wrapper.ListView.isCurrentItem ? 1.0 : 0.1
                }
            }
        }
        ListView {
            id: voiceSelectionListView
            width: display.width
            height: display.height / 6
            anchors.bottom: parent.bottom
            model: soundDeviceAndVoiceSelection.voiceSelectionModel
            orientation: Qt.Horizontal
            delegate: voiceSelectionDelegate
            currentIndex: soundDeviceAndVoiceSelection.currentVoiceIndex
        }
        Component{
            id: voiceSelectionDelegate
            Rectangle{
                id: voiceSelectionDelegateRectangle
                width: display.width / 8
                height : display.height / 6
                color: ListView.isCurrentItem ?"white":"black"
                radius : 3
                Text {
                    anchors.fill: parent
                    text: name
                    color: voiceSelectionDelegateRectangle.ListView.isCurrentItem ?"black":"grey"
                    font.pointSize: 10
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}