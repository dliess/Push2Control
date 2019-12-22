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
                    var idx = deviceSelectionListView.indexAt(deviceSelectionListView.contentX,1)
                    var _x = x + idx
                    if(0 <= _x &&_x < deviceSelectionListView.count)
                    {
                        deviceSelectionListView.currentIndex = _x
                    }
                    break
                }
                case Button.E_BtnB:
                {
                    var idx = presetSelectionListView.indexAt(presetSelectionListView.contentX,1)
                    var _x = x + idx
                    if(0 <= _x &&_x < presetSelectionListView.count)
                    {
                        controllerDeviceAndPresetSelection.setCurrentPresetIndex(x + presetSelectionListView.indexAt(0,0))
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
                    var idx = deviceSelectionListView.indexAt(deviceSelectionListView.contentX,1)
                    idx += increment
                    idx = Math.max(0, idx)
                    idx = Math.min(deviceSelectionListView.count - 9, idx)
                    deviceSelectionListView.positionViewAtIndex(idx, ListView.Beginning);
                    break
                }
                case Encoder.E_EncoderMetronome:
                {
                    var idx = presetSelectionListView.indexAt(presetSelectionListView.contentX,1)
                    idx += increment
                    idx = Math.max(0, idx)
                    idx = Math.min(presetSelectionListView.count - 9, idx)
                    presetSelectionListView.positionViewAtIndex(idx, ListView.Beginning);
                    break
                }
            }
        }
    }


    Rectangle {
        anchors.fill: parent
        color: "black"
        
        ListView {
            id: deviceSelectionListView
            width: parent.width
            height: display.height / 2
            anchors.top: parent.top
            model: controllerDeviceAndPresetSelection.controllerDeviceSelectionModel
            orientation: Qt.Horizontal
            spacing: 4
            delegate: deviceDelegate
            currentIndex: -1
            onCurrentIndexChanged:{
                controllerDeviceAndPresetSelection.setCurrentDeviceIndex(currentIndex)
            }
            Component.onCompleted:{
                currentIndex = controllerDeviceAndPresetSelection.currentDeviceIndex
            }
        }
        Component{
            id: deviceDelegate
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
            id: presetSelectionListView
            width: display.width
            height: display.height / 6
            anchors.bottom: parent.bottom
            model: controllerDeviceAndPresetSelection.controllerPresetSelectionModel
            orientation: Qt.Horizontal
            spacing: 4
            delegate: presetSelectionDelegate
            currentIndex: controllerDeviceAndPresetSelection.currentPresetIndex
        }
        Component{
            id: presetSelectionDelegate
            Rectangle{
                id: presetSelectionDelegateRectangle
                width: display.width / 8
                height : display.height / 6
                color: ListView.isCurrentItem ?"white":"black"
                radius : 3
                Text {
                    anchors.fill: parent
                    text: name
                    color: presetSelectionDelegateRectangle.ListView.isCurrentItem ?"black":"grey"
                    font.pointSize: 10
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
